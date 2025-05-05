#include "postgres.h"
#include "fmgr.h"
#include "utils/pg_locale.h"
#include "utils/builtins.h"
#include "utils/varlena.h"
#include "varatt.h"

PG_MODULE_MAGIC;

Datum iban_in(PG_FUNCTION_ARGS);
Datum iban_out(PG_FUNCTION_ARGS);
Datum iban_eq(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(iban_in);
PG_FUNCTION_INFO_V1(iban_out);
PG_FUNCTION_INFO_V1(iban_eq);

typedef struct {
    char data[34];
} IBAN;

static int char_to_num(char c) {
    if (isdigit(c)) return c - '0';
    if (isupper(c)) return c - 'A' + 10;
    if (islower(c)) return c - 'a' + 10;
    return -1;
}

static bool validate_iban_checksum(const char *iban_clean) {
    char temp[256];
    int len = strlen(iban_clean);

    strncpy(temp, iban_clean + 4, len - 4);
    strncpy(temp + len - 4, iban_clean, 4);
    temp[len] = '\0';

    char numeric[256];
    for (int i = 0; i < len; i++) {
        int num = char_to_num(temp[i]);
        if (num < 0) return false;
        sprintf(numeric + i * 2, "%02d", num);
    }

    unsigned long long mod = 0;
    for (int i = 0; i < strlen(numeric); i++) {
        mod = (mod * 10 + (numeric[i] - '0')) % 97;
    }

    return (mod == 1);
}

Datum iban_in(PG_FUNCTION_ARGS) {
    char *str = PG_GETARG_CSTRING(0);
    IBAN *result = palloc(sizeof(IBAN));

    char cleaned[256];
    int j = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] != ' ') cleaned[j++] = toupper(str[i]);
    }
    cleaned[j] = '\0';

    if (strlen(cleaned) < 15 || strlen(cleaned) > 34) {
        ereport(ERROR, (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION), errmsg("Invalid IBAN length: %s", cleaned)));
    }

    if (!isalpha(cleaned[0]) || !isalpha(cleaned[1])) {
        ereport(ERROR, (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION), errmsg("Invalid country code in IBAN: %s", cleaned)));
    }

    if (!isdigit(cleaned[2]) || !isdigit(cleaned[3])) {
        ereport(ERROR, (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION), errmsg("Invalid checksum digits in IBAN: %s", cleaned)));
    }

    if (!validate_iban_checksum(cleaned)) {
        ereport(ERROR, (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION), errmsg("Invalid IBAN checksum: %s", cleaned)));
    }

    strncpy(result->data, cleaned, sizeof(result->data));
    PG_RETURN_POINTER(result);
}

Datum iban_out(PG_FUNCTION_ARGS) {
    IBAN *iban = (IBAN *) PG_GETARG_POINTER(0);
    char *result = pstrdup(iban->data);
    PG_RETURN_CSTRING(result);
}

Datum iban_eq(PG_FUNCTION_ARGS) {
    IBAN *a = (IBAN *) PG_GETARG_POINTER(0);
    IBAN *b = (IBAN *) PG_GETARG_POINTER(1);
    PG_RETURN_BOOL(strcmp(a->data, b->data) == 0);
}

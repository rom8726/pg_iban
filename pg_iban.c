#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "utils/lsyscache.h"
#include "access/hash.h"
#include "utils/varlena.h"
#include "varatt.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

PG_MODULE_MAGIC;

#define MAX_IBAN_LENGTH 34
#define MIN_IBAN_LENGTH 15

Datum pg_iban_in(PG_FUNCTION_ARGS);
Datum pg_iban_out(PG_FUNCTION_ARGS);
Datum pg_iban_eq(PG_FUNCTION_ARGS);
Datum pg_iban_lt(PG_FUNCTION_ARGS);
Datum pg_iban_le(PG_FUNCTION_ARGS);
Datum pg_iban_gt(PG_FUNCTION_ARGS);
Datum pg_iban_ge(PG_FUNCTION_ARGS);
Datum pg_iban_cmp(PG_FUNCTION_ARGS);
Datum pg_iban_hash(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(pg_iban_in);
PG_FUNCTION_INFO_V1(pg_iban_out);
PG_FUNCTION_INFO_V1(pg_iban_eq);
PG_FUNCTION_INFO_V1(pg_iban_lt);
PG_FUNCTION_INFO_V1(pg_iban_le);
PG_FUNCTION_INFO_V1(pg_iban_gt);
PG_FUNCTION_INFO_V1(pg_iban_ge);
PG_FUNCTION_INFO_V1(pg_iban_cmp);
PG_FUNCTION_INFO_V1(pg_iban_hash);

extern Datum bttextcmp(PG_FUNCTION_ARGS);

static bool validate_pg_iban(const char *iban);

Datum pg_iban_in(PG_FUNCTION_ARGS)
{
    char *str = PG_GETARG_CSTRING(0);
    char clean[MAX_IBAN_LENGTH + 1];
    int j = 0;
    for (int i = 0; str[i] != '\0' && j < MAX_IBAN_LENGTH; i++) {
        if (!isspace((unsigned char)str[i])) {
            if (!isalnum((unsigned char)str[i])) {
                ereport(ERROR,
                        (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                         errmsg("IBAN must contain only alphanumeric characters")));
            }
            clean[j++] = toupper((unsigned char)str[i]);
        }
    }
    clean[j] = '\0';

    if (!validate_pg_iban(clean)) {
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                 errmsg("invalid IBAN format or checksum")));
    }

    PG_RETURN_TEXT_P(cstring_to_text(clean));
}

Datum pg_iban_out(PG_FUNCTION_ARGS)
{
    text *iban_text = PG_GETARG_TEXT_P(0);
    char *result = text_to_cstring(iban_text);
    PG_RETURN_CSTRING(result);
}

static bool validate_pg_iban(const char *iban)
{
    int len = strlen(iban);
    if (len < MIN_IBAN_LENGTH || len > MAX_IBAN_LENGTH)
        return false;

    char rearranged[MAX_IBAN_LENGTH * 2 + 1];
    snprintf(rearranged, sizeof(rearranged), "%s%s", iban + 4, iban);

    char digits[MAX_IBAN_LENGTH * 4 + 1];
    int pos = 0;
    for (int i = 0; rearranged[i] != '\0'; i++) {
        if (isdigit((unsigned char)rearranged[i])) {
            digits[pos++] = rearranged[i];
        } else if (isupper((unsigned char)rearranged[i])) {
            int val = rearranged[i] - 'A' + 10;
            pos += snprintf(digits + pos, sizeof(digits) - pos, "%d", val);
        } else {
            return false;
        }
    }
    digits[pos] = '\0';

    char *p = digits;
    long long rem = 0;
    while (*p) {
        char chunk[10] = {0};
        snprintf(chunk, sizeof(chunk), "%lld%.9s", rem, p);
        rem = strtoll(chunk, NULL, 10) % 97;
        p += 9;
    }

    return rem == 1;
}

Datum pg_iban_eq(PG_FUNCTION_ARGS)
{
    text *a = PG_GETARG_TEXT_PP(0);
    text *b = PG_GETARG_TEXT_PP(1);
    PG_RETURN_BOOL(DatumGetInt32(DirectFunctionCall3(bttextcmp, PointerGetDatum(a), PointerGetDatum(b), ObjectIdGetDatum(PG_GET_COLLATION()))) == 0);
}

Datum pg_iban_lt(PG_FUNCTION_ARGS)
{
    text *a = PG_GETARG_TEXT_PP(0);
    text *b = PG_GETARG_TEXT_PP(1);
    PG_RETURN_BOOL(DatumGetInt32(DirectFunctionCall3(bttextcmp, PointerGetDatum(a), PointerGetDatum(b), ObjectIdGetDatum(PG_GET_COLLATION()))) < 0);
}

Datum pg_iban_le(PG_FUNCTION_ARGS)
{
    text *a = PG_GETARG_TEXT_PP(0);
    text *b = PG_GETARG_TEXT_PP(1);
    PG_RETURN_BOOL(DatumGetInt32(DirectFunctionCall3(bttextcmp, PointerGetDatum(a), PointerGetDatum(b), ObjectIdGetDatum(PG_GET_COLLATION()))) <= 0);
}

Datum pg_iban_gt(PG_FUNCTION_ARGS)
{
    text *a = PG_GETARG_TEXT_PP(0);
    text *b = PG_GETARG_TEXT_PP(1);
    PG_RETURN_BOOL(DatumGetInt32(DirectFunctionCall3(bttextcmp, PointerGetDatum(a), PointerGetDatum(b), ObjectIdGetDatum(PG_GET_COLLATION()))) > 0);
}

Datum pg_iban_ge(PG_FUNCTION_ARGS)
{
    text *a = PG_GETARG_TEXT_PP(0);
    text *b = PG_GETARG_TEXT_PP(1);
    PG_RETURN_BOOL(DatumGetInt32(DirectFunctionCall3(bttextcmp, PointerGetDatum(a), PointerGetDatum(b), ObjectIdGetDatum(PG_GET_COLLATION()))) >= 0);
}

Datum pg_iban_cmp(PG_FUNCTION_ARGS)
{
    text *a = PG_GETARG_TEXT_PP(0);
    text *b = PG_GETARG_TEXT_PP(1);
    PG_RETURN_INT32(DatumGetInt32(DirectFunctionCall3(bttextcmp, PointerGetDatum(a), PointerGetDatum(b), ObjectIdGetDatum(PG_GET_COLLATION()))));
}

Datum pg_iban_hash(PG_FUNCTION_ARGS)
{
    text *iban = PG_GETARG_TEXT_PP(0);
    PG_RETURN_UINT32(DatumGetUInt32(hash_any((const unsigned char *) VARDATA_ANY(iban),
                                             VARSIZE_ANY_EXHDR(iban))));
}

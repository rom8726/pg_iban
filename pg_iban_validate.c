#include "pg_iban_validate.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

bool validate_pg_iban(const char *iban)
{
    int len = strlen(iban);
    if (len < MIN_IBAN_LENGTH || len > MAX_IBAN_LENGTH)
        return false;

    if (!isalpha(iban[0]) || !isalpha(iban[1])) {
        return false;
    }

    if (!isdigit(iban[2]) || !isdigit(iban[3])) {
        return false;
    }

    if (!validate_iban_checksum(iban)) {
        return false;
    }

    return true;
}

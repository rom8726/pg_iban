#include "pg_iban_validate.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static bool validate_iban_checksum(const char *iban_clean)
{
    const int len = (int)strlen(iban_clean);
    char temp[256];
    memcpy(temp, iban_clean + 4, len - 4);
    memcpy(temp + len - 4, iban_clean, 4);
    temp[len] = '\0';

    char numeric[512];
    size_t pos = 0;
    for (int i = 0; i < len; ++i) {
        char c = temp[i];
        if (isdigit((unsigned char)c)) {
            numeric[pos++] = c;
        } else if (isalpha((unsigned char)c)) {
            int val = toupper(c) - 'A' + 10;
            numeric[pos++] = '0' + (val / 10);
            numeric[pos++] = '0' + (val % 10);
        } else {
            return false;
        }
    }
    numeric[pos] = '\0';

    unsigned int mod = 0;
    for (size_t i = 0; i < pos; ++i) {
        mod = (mod * 10 + (numeric[i] - '0')) % 97;
    }

    return mod == 1;
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

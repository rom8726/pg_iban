#include "pg_iban_validate.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

bool validate_pg_iban(const char *iban)
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

    const char *p = digits;
    long long rem = 0;
    while (*p) {
        char chunk[20];
        int n = snprintf(chunk, sizeof(chunk), "%lld%.9s", rem, p);
        chunk[n] = '\0';
        rem = strtoll(chunk, NULL, 10) % 97;
        p += 9;
    }
    return rem == 1;
}

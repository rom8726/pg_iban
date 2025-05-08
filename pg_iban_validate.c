#include "pg_iban_validate.h"
#include "def.h"
#include <ctype.h>
#include <string.h>

bool validate_iban_checksum(const char* iban_clean) {
    const int len = (int)strlen(iban_clean);

    if (len < 4 || len < MIN_IBAN_LENGTH || len > MAX_IBAN_LENGTH)
        return false;

    unsigned int mod = 0;

    for (int i = 4; i < len + 4; ++i) {
        unsigned char ch = (unsigned char)iban_clean[(i < len) ? i : (i - len)];

        if (isdigit(ch)) {
            mod = (mod * 10 + (ch - '0')) % 97;
        } else if (isalpha(ch)) {
            int val = toupper(ch) - 'A' + 10;
            mod = (mod * 10 + (val / 10)) % 97;
            mod = (mod * 10 + (val % 10)) % 97;
        } else {
            return false;
        }
    }

    return mod == 1;
}

bool validate_iban(const char* iban) {
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

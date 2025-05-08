#include "pg_iban_clean.h"
#include "def.h"

bool clean_iban(const char *src, char *dst) {
    int j = 0;

    for (int i = 0; src[i] != '\0' && j < MAX_IBAN_LENGTH; i++) {
        unsigned char ch = (unsigned char)src[i];

        if (isspace(ch))
            continue;
        
        if (!isalnum(ch))
            return false;

        dst[j++] = (char)toupper(ch);
    }

    dst[j] = '\0';

    return true;
}

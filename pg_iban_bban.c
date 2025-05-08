#include "def.h"
#include "pg_iban_preprocess.h"
#include <stdio.h>
#include <string.h>

bool iban_bban(const char* iban, char* bban) {
    char cleaned_iban[MAX_IBAN_LENGTH + 1];

    if (!preprocess_iban(iban, cleaned_iban)) {
        bban[0] = '\0';

        return false;
    }

    // BBAN starts after the first 4 characters (country code + checksum)
    strncpy(bban, cleaned_iban + 4, strlen(cleaned_iban) - 1);
    bban[strlen(iban) - 4] = '\0'; // Null-terminate for safety

    return true;
}

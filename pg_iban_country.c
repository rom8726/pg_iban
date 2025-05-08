#include <string.h>
#include <stdbool.h>
#include "def.h"
#include "pg_iban_preprocess.h"

bool iban_country(const char *iban, char *country) {
    char cleaned_iban[MAX_IBAN_LENGTH + 1];

    if (!preprocess_iban(iban, cleaned_iban)) {
        country[0] = '\0';

        return false;
    }

    // Copy the country code (first 2 characters)
    strncpy(country, cleaned_iban, 2);
    country[2] = '\0'; // Null-terminate for safety

    return true;
}

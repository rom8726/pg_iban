#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "def.h"
#include "pg_iban_preprocess.h"

bool iban_format(const char* iban, char* formatted_iban) {
    char cleaned_iban[MAX_IBAN_LENGTH + 1];

    if (!preprocess_iban(iban, cleaned_iban)) {
        formatted_iban[0] = '\0';

        return false;
    }

    size_t cleaned_length = strlen(cleaned_iban);
    size_t formatted_index = 0;

    // Group the cleaned IBAN into blocks of 4 characters
    for (size_t i = 0; i < cleaned_length; i++) {
        if (i > 0 && i % 4 == 0) {
            formatted_iban[formatted_index++] = ' '; // Add space after every 4 characters
        }

        formatted_iban[formatted_index++] = cleaned_iban[i];
    }

    formatted_iban[formatted_index] = '\0';

    return true;
}
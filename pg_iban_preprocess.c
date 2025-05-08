#include "pg_iban_clean.h"
#include "pg_iban_validate.h"

bool preprocess_iban(const char *src, char *cleaned_iban) {
    if (!clean_iban(src, cleaned_iban)) {
        return false;
    }

    return validate_iban(cleaned_iban);
}

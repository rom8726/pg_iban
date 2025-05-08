#ifndef PG_IBAN_PREPROCESS_H
#define PG_IBAN_PREPROCESS_H

#include <stdbool.h>

bool preprocess_iban(const char *src, char *cleaned_iban);

#endif

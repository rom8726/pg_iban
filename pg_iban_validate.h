#ifndef PG_IBAN_VALIDATE_H
#define PG_IBAN_VALIDATE_H

#include <stdbool.h>

bool validate_iban_checksum(const char* iban_clean);
bool validate_iban(const char* iban);

#endif

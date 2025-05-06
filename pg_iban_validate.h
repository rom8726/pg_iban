#ifndef PG_IBAN_VALIDATE_H
#define PG_IBAN_VALIDATE_H

#include <stdbool.h>

#define MAX_IBAN_LENGTH 34
#define MIN_IBAN_LENGTH 15

bool validate_pg_iban(const char *iban);

#endif

#ifndef PG_IBAN_CLEAN_H
#define PG_IBAN_CLEAN_H

#include <stdbool.h>

bool clean_iban(const char* src, char* dst);

#endif

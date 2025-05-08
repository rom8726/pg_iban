#ifndef PG_IBAN_FORMAT_H
#define PG_IBAN_FORMAT_H

#include <stdbool.h>

bool iban_format(const char *iban, char *formatted_iban);

#endif

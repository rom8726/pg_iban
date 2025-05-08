#ifndef PG_IBAN_COUNTRY_H
#define PG_IBAN_COUNTRY_H

#include <stdbool.h>

bool iban_country(const char *iban, char *country);

#endif

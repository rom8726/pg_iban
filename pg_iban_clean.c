#include "pg_iban_clean.h"
#include "def.h"
#include <ctype.h>
#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"

void clean_iban(const char *src, char *dst)
{
    int j = 0;

    for (int i = 0; src[i] != '\0' && j < MAX_IBAN_LENGTH; i++) {
        if (!isspace((unsigned char)src[i])) {
            if (!isalnum((unsigned char)src[i])) {
                ereport(ERROR,
                        (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                         errmsg("IBAN must contain only alphanumeric characters")));
            }

            dst[j++] = (char)toupper((unsigned char)src[i]);
        }
    }

    dst[j] = '\0';
}

#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "utils/lsyscache.h"
#include "access/hash.h"
#include "utils/varlena.h"
#include "varatt.h"

#include "def.h"
#include "pg_iban_clean.h"
#include "pg_iban_validate.h"

PG_MODULE_MAGIC;

Datum pg_iban_in(PG_FUNCTION_ARGS);
Datum pg_iban_out(PG_FUNCTION_ARGS);
Datum pg_iban_eq(PG_FUNCTION_ARGS);
Datum pg_iban_lt(PG_FUNCTION_ARGS);
Datum pg_iban_le(PG_FUNCTION_ARGS);
Datum pg_iban_gt(PG_FUNCTION_ARGS);
Datum pg_iban_ge(PG_FUNCTION_ARGS);
Datum pg_iban_cmp(PG_FUNCTION_ARGS);
Datum pg_iban_hash(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(pg_iban_in);
PG_FUNCTION_INFO_V1(pg_iban_out);
PG_FUNCTION_INFO_V1(pg_iban_eq);
PG_FUNCTION_INFO_V1(pg_iban_lt);
PG_FUNCTION_INFO_V1(pg_iban_le);
PG_FUNCTION_INFO_V1(pg_iban_gt);
PG_FUNCTION_INFO_V1(pg_iban_ge);
PG_FUNCTION_INFO_V1(pg_iban_cmp);
PG_FUNCTION_INFO_V1(pg_iban_hash);

extern Datum bttextcmp(PG_FUNCTION_ARGS);

Datum pg_iban_in(PG_FUNCTION_ARGS)
{
    char *str = PG_GETARG_CSTRING(0);
    char clean[MAX_IBAN_LENGTH + 1];

    if (!clean_iban(str, clean)) {
        ereport(ERROR,
            (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
             errmsg("IBAN must contain only alphabetic characters and spaces")));
    }

    if (!validate_iban(clean)) {
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                 errmsg("invalid IBAN format or checksum")));
    }

    PG_RETURN_TEXT_P(cstring_to_text(clean));
}

Datum pg_iban_out(PG_FUNCTION_ARGS)
{
    text *iban_text = PG_GETARG_TEXT_P(0);
    char *result = text_to_cstring(iban_text);
    PG_RETURN_CSTRING(result);
}

Datum pg_iban_eq(PG_FUNCTION_ARGS)
{
    text *a = PG_GETARG_TEXT_PP(0);
    text *b = PG_GETARG_TEXT_PP(1);
    PG_RETURN_BOOL(DatumGetInt32(DirectFunctionCall3(bttextcmp, PointerGetDatum(a), PointerGetDatum(b), ObjectIdGetDatum(PG_GET_COLLATION()))) == 0);
}

Datum pg_iban_lt(PG_FUNCTION_ARGS)
{
    text *a = PG_GETARG_TEXT_PP(0);
    text *b = PG_GETARG_TEXT_PP(1);
    PG_RETURN_BOOL(DatumGetInt32(DirectFunctionCall3(bttextcmp, PointerGetDatum(a), PointerGetDatum(b), ObjectIdGetDatum(PG_GET_COLLATION()))) < 0);
}

Datum pg_iban_le(PG_FUNCTION_ARGS)
{
    text *a = PG_GETARG_TEXT_PP(0);
    text *b = PG_GETARG_TEXT_PP(1);
    PG_RETURN_BOOL(DatumGetInt32(DirectFunctionCall3(bttextcmp, PointerGetDatum(a), PointerGetDatum(b), ObjectIdGetDatum(PG_GET_COLLATION()))) <= 0);
}

Datum pg_iban_gt(PG_FUNCTION_ARGS)
{
    text *a = PG_GETARG_TEXT_PP(0);
    text *b = PG_GETARG_TEXT_PP(1);
    PG_RETURN_BOOL(DatumGetInt32(DirectFunctionCall3(bttextcmp, PointerGetDatum(a), PointerGetDatum(b), ObjectIdGetDatum(PG_GET_COLLATION()))) > 0);
}

Datum pg_iban_ge(PG_FUNCTION_ARGS)
{
    text *a = PG_GETARG_TEXT_PP(0);
    text *b = PG_GETARG_TEXT_PP(1);
    PG_RETURN_BOOL(DatumGetInt32(DirectFunctionCall3(bttextcmp, PointerGetDatum(a), PointerGetDatum(b), ObjectIdGetDatum(PG_GET_COLLATION()))) >= 0);
}

Datum pg_iban_cmp(PG_FUNCTION_ARGS)
{
    text *a = PG_GETARG_TEXT_PP(0);
    text *b = PG_GETARG_TEXT_PP(1);
    PG_RETURN_INT32(DatumGetInt32(DirectFunctionCall3(bttextcmp, PointerGetDatum(a), PointerGetDatum(b), ObjectIdGetDatum(PG_GET_COLLATION()))));
}

Datum pg_iban_hash(PG_FUNCTION_ARGS)
{
    text *iban = PG_GETARG_TEXT_PP(0);
    PG_RETURN_UINT32(DatumGetUInt32(hash_any((const unsigned char *) VARDATA_ANY(iban),
                                             VARSIZE_ANY_EXHDR(iban))));
}

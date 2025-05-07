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
Datum pg_iban_is_valid(PG_FUNCTION_ARGS);
Datum pg_iban_country(PG_FUNCTION_ARGS);
Datum pg_iban_bban(PG_FUNCTION_ARGS);
Datum pg_iban_format(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(pg_iban_in);
PG_FUNCTION_INFO_V1(pg_iban_out);
PG_FUNCTION_INFO_V1(pg_iban_eq);
PG_FUNCTION_INFO_V1(pg_iban_lt);
PG_FUNCTION_INFO_V1(pg_iban_le);
PG_FUNCTION_INFO_V1(pg_iban_gt);
PG_FUNCTION_INFO_V1(pg_iban_ge);
PG_FUNCTION_INFO_V1(pg_iban_cmp);
PG_FUNCTION_INFO_V1(pg_iban_hash);
PG_FUNCTION_INFO_V1(pg_iban_is_valid);
PG_FUNCTION_INFO_V1(pg_iban_country);
PG_FUNCTION_INFO_V1(pg_iban_bban);
PG_FUNCTION_INFO_V1(pg_iban_format);

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

Datum pg_iban_is_valid(PG_FUNCTION_ARGS)
{
    text *iban_text = PG_GETARG_TEXT_PP(0);
    char *raw       = text_to_cstring(iban_text);
    char  clean[MAX_IBAN_LENGTH + 1];

    bool ok = (clean_iban(raw, clean) && validate_iban(clean));

    pfree(raw);
    PG_RETURN_BOOL(ok);
}

Datum pg_iban_country(PG_FUNCTION_ARGS)
{
    text *iban_text = PG_GETARG_TEXT_PP(0);
    int iban_length = VARSIZE_ANY_EXHDR(iban_text);

    if (iban_length < 2) {
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                 errmsg("IBAN is too short to contain a country code")));
    }

    char* iban_raw = VARDATA_ANY(iban_text);
    char country_code[3];
    country_code[0] = iban_raw[0];
    country_code[1] = iban_raw[1];
    country_code[2] = '\0';

    PG_RETURN_TEXT_P(cstring_to_text(country_code));
}

Datum pg_iban_bban(PG_FUNCTION_ARGS)
{
    text *iban_text = PG_GETARG_TEXT_PP(0);
    int iban_length = VARSIZE_ANY_EXHDR(iban_text);

    if (iban_length < 5) {
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                 errmsg("IBAN is too short to contain a BBAN")));
    }

    char *iban_raw = VARDATA_ANY(iban_text);
    char *bban = &(iban_raw[4]);

    int bban_length = iban_length - 4;

    PG_RETURN_TEXT_P(cstring_to_text_with_len(bban, bban_length));
}

Datum pg_iban_format(PG_FUNCTION_ARGS)
{
    text *iban_text = PG_GETARG_TEXT_PP(0);
    int iban_length = VARSIZE_ANY_EXHDR(iban_text);

    if (iban_length < MIN_IBAN_LENGTH || iban_length > MAX_IBAN_LENGTH) {
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                 errmsg("IBAN length is invalid. Must be between %d and %d characters", MIN_IBAN_LENGTH, MAX_IBAN_LENGTH)));
    }

    const char *iban_raw = VARDATA_ANY(iban_text);
    int formatted_length = iban_length + (iban_length - 1) / 4;

    char *formatted = palloc0(formatted_length + 1);
    int src_idx = 0, dest_idx = 0;

    for (src_idx = 0; src_idx < iban_length; src_idx++) {
        if (src_idx > 0 && src_idx % 4 == 0) {
            formatted[dest_idx++] = ' ';
        }

        formatted[dest_idx++] = iban_raw[src_idx];
    }

    formatted[dest_idx] = '\0';

    PG_RETURN_TEXT_P(cstring_to_text(formatted));
}

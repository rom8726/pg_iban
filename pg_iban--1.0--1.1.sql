CREATE FUNCTION iban_country(iban) RETURNS text
AS 'pg_iban', 'pg_iban_country'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION iban_bban(iban) RETURNS text
AS 'pg_iban', 'pg_iban_bban'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION iban_format(iban) RETURNS text
AS 'pg_iban', 'pg_iban_format'
    LANGUAGE C IMMUTABLE STRICT;

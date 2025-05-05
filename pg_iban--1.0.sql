CREATE TYPE iban (
    INPUT = iban_in,
    OUTPUT = iban_out,
    LIKE = TEXT
);

CREATE OPERATOR = (
    LEFTARG = iban,
    RIGHTARG = iban,
    PROCEDURE = iban_eq,
    COMMUTATOR = =
);

CREATE OR REPLACE FUNCTION format_iban(iban)
RETURNS text AS $$
SELECT string_agg(substring($1::text FROM i*4+1 FOR 4), ' ')
FROM generate_series(0, floor((length($1::text)-1)/4)::int) AS i;
$$ LANGUAGE sql IMMUTABLE;

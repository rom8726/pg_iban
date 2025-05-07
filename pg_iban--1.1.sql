-- 1.0
-- Input/output functions
CREATE FUNCTION iban_in(cstring) RETURNS iban AS 'pg_iban', 'pg_iban_in'
    LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION iban_out(iban) RETURNS cstring AS 'pg_iban', 'pg_iban_out'
    LANGUAGE C IMMUTABLE STRICT;

-- Type definition
CREATE TYPE iban (
    internallength = -1,
    input = iban_in,
    output = iban_out,
    alignment = int4,
    storage = extended
);

-- Comparison and hash functions
CREATE FUNCTION iban_eq(iban, iban) RETURNS bool AS 'pg_iban', 'pg_iban_eq'
    LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION iban_lt(iban, iban) RETURNS bool AS 'pg_iban', 'pg_iban_lt'
    LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION iban_le(iban, iban) RETURNS bool AS 'pg_iban', 'pg_iban_le'
    LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION iban_gt(iban, iban) RETURNS bool AS 'pg_iban', 'pg_iban_gt'
    LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION iban_ge(iban, iban) RETURNS bool AS 'pg_iban', 'pg_iban_ge'
    LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION iban_cmp(iban, iban) RETURNS int4 AS 'pg_iban', 'pg_iban_cmp'
    LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION iban_hash(iban) RETURNS int4 AS 'pg_iban', 'pg_iban_hash'
    LANGUAGE C IMMUTABLE STRICT;

-- Operators
CREATE OPERATOR = (
    LEFTARG = iban, RIGHTARG = iban,
    PROCEDURE = iban_eq, COMMUTATOR = =, NEGATOR = <>, RESTRICT = eqsel, JOIN = eqjoinsel
);
CREATE OPERATOR < (
    LEFTARG = iban, RIGHTARG = iban,
    PROCEDURE = iban_lt, NEGATOR = >=, RESTRICT = scalarltsel, JOIN = scalarltjoinsel
);
CREATE OPERATOR <= (
    LEFTARG = iban, RIGHTARG = iban,
    PROCEDURE = iban_le, NEGATOR = >, RESTRICT = scalarltsel, JOIN = scalarltjoinsel
);
CREATE OPERATOR > (
    LEFTARG = iban, RIGHTARG = iban,
    PROCEDURE = iban_gt, NEGATOR = <=, RESTRICT = scalargtsel, JOIN = scalargtjoinsel
);
CREATE OPERATOR >= (
    LEFTARG = iban, RIGHTARG = iban,
    PROCEDURE = iban_ge, NEGATOR = <, RESTRICT = scalargtsel, JOIN = scalargtjoinsel
);

-- B-tree and hash opclasses
CREATE OPERATOR CLASS iban_ops DEFAULT FOR TYPE iban USING btree AS
    OPERATOR 1 <,
    OPERATOR 2 <=,
    OPERATOR 3 =,
    OPERATOR 4 >=,
    OPERATOR 5 >,
    FUNCTION 1 iban_cmp(iban, iban);

CREATE OPERATOR CLASS iban_hash_ops FOR TYPE iban USING hash AS
    OPERATOR 1 =,
    FUNCTION 1 iban_hash(iban);

-- User's functions
CREATE FUNCTION iban_valid(text) RETURNS bool
AS 'pg_iban', 'pg_iban_is_valid'
    LANGUAGE C IMMUTABLE STRICT;

-- 1.1
CREATE FUNCTION iban_country(iban) RETURNS text
AS 'pg_iban', 'pg_iban_country'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION iban_bban(iban) RETURNS text
AS 'pg_iban', 'pg_iban_bban'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION iban_format(iban) RETURNS text
AS 'pg_iban', 'pg_iban_format'
    LANGUAGE C IMMUTABLE STRICT;

\connect test_db

-- 1) CREATE EXTENSION
CREATE EXTENSION pg_iban;

-- 2) CHECK iban_valid() FUNCTION
DO $$ BEGIN
    IF NOT iban_valid('GB82WEST12345698765432') THEN
        RAISE EXCEPTION 'Test failed: iban_valid should return true for a valid IBAN';
    END IF;

    IF iban_valid('INVALIDIBAN') THEN
        RAISE EXCEPTION 'Test failed: iban_valid should return false for an invalid IBAN';
    END IF;

    RAISE NOTICE 'IBAN validation tests passed';
END $$;

-- 3) CREATE TABLE WITH iban DATA TYPE FIELD
CREATE TABLE test_iban_table (
    iban_field iban
);

-- 4) INSERT ROW INTO TABLE
INSERT INTO test_iban_table VALUES ('GB82WEST12345698765432');

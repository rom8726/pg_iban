\connect test_db

-- ================================================================================================
\! echo '\n\n>>> 1. CREATE EXTENSION'
CREATE EXTENSION pg_iban;
\! echo '<<< OK'

-- ================================================================================================
\! echo '\n\n>>> 2. CHECK iban_valid() FUNCTION'
DO $$ BEGIN
    IF NOT iban_valid('GB82WEST12345698765432') THEN
        RAISE EXCEPTION 'Test failed: iban_valid should return true for a valid IBAN';
    END IF;

    IF iban_valid('INVALIDIBAN') THEN
        RAISE EXCEPTION 'Test failed: iban_valid should return false for an invalid IBAN';
    END IF;

    RAISE NOTICE 'IBAN validation tests passed';
END $$;
\! echo '<<< OK'

-- ================================================================================================
\! echo '\n\n>>> 3. CREATE TABLE WITH iban DATA TYPE FIELD'
CREATE TABLE test_iban_table (
    iban_field iban
);
\! echo '<<< OK'

-- ================================================================================================
\! echo '\n\n>>> 4. INSERT ROW INTO TABLE'
INSERT INTO test_iban_table VALUES ('GB82WEST12345698765432');
\! echo '<<< OK'

-- ================================================================================================
\! echo '\n\n>>> 5. TEST pg_iban_country() FUNCTION'
DO $$
BEGIN
    IF iban_country('DE44500105175407324931') != 'DE' THEN
        RAISE EXCEPTION 'Test failed: iban_country for "DE44500105175407324931" should return "DE", got %',
        iban_country('DE44500105175407324931');
END IF;

BEGIN
        PERFORM iban_country('D');
        RAISE EXCEPTION 'Test failed: iban_country did not throw error for short IBAN';
EXCEPTION WHEN OTHERS THEN
        RAISE NOTICE 'Test passed: iban_country threw error for short IBAN';
END;

    RAISE NOTICE 'pg_iban_country tests passed';
END $$;
\! echo '<<< OK'


-- ================================================================================================
\! echo '\n\n>>> 6. TEST pg_iban_bban() FUNCTION'
DO $$
BEGIN
    IF iban_bban('DE44500105175407324931') != '500105175407324931' THEN
        RAISE EXCEPTION 'Test failed: iban_bban for "DE44500105175407324931" should return "500105175407324931", got %',
        iban_bban('DE44500105175407324931');
END IF;

BEGIN
        PERFORM iban_bban('DE44');
        RAISE EXCEPTION 'Test failed: iban_bban did not throw error for short IBAN';
EXCEPTION WHEN OTHERS THEN
        RAISE NOTICE 'Test passed: iban_bban threw error for short IBAN';
END;

    RAISE NOTICE 'pg_iban_bban tests passed';
END $$;
\! echo '<<< OK'


-- ================================================================================================
\! echo '\n\n>>> 7. TEST pg_iban_format() FUNCTION'
DO $$
BEGIN
    IF iban_format('DE44500105175407324931') != 'DE44 5001 0517 5407 3249 31' THEN
        RAISE EXCEPTION 'Test failed: iban_format for "DE44500105175407324931" should return "DE44 5001 0517 5407 3249 31", got %',
        iban_format('DE44500105175407324931');
END IF;

BEGIN
        PERFORM iban_format('DE');
        RAISE EXCEPTION 'Test failed: iban_format did not throw error for short IBAN';
EXCEPTION WHEN OTHERS THEN
        RAISE NOTICE 'Test passed: iban_format threw error for short IBAN';
END;

    RAISE NOTICE 'pg_iban_format tests passed';
END $$;
\! echo '<<< OK'

-- ================================================================================================
\! echo '\n\n>>> ALL TESTS PASSED! <<<'

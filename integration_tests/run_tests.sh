#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

initdb -D /var/lib/postgresql/16/data

pg_ctl start -D /var/lib/postgresql/16/data
sleep 5

psql --set ON_ERROR_STOP=on -U $POSTGRES_USER -d postgres -f /opt/pg_iban/integration_tests/01_create_database.sql
TEST_RESULT=$?
if [ $TEST_RESULT -eq 0 ]; then
    psql --set ON_ERROR_STOP=on -U $POSTGRES_USER -d test_db -f /opt/pg_iban/integration_tests/02_integration_tests.sql
    TEST_RESULT=$?
fi

pg_ctl stop -D /var/lib/postgresql/16/data

if [ $TEST_RESULT -eq 0 ]; then
    echo -e "${GREEN}TESTS PASSED${NC}"
else
    echo -e "${RED}TESTS FAILED${NC}"
fi

exit $TEST_RESULT

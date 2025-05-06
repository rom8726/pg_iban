EXTENSION = pg_iban
MODULES   = pg_iban
DATA      = pg_iban--1.0.sql
PG_CONFIG = pg_config
MODULE_big = pg_iban
OBJS       = pg_iban.o pg_iban_validate.o pg_iban_clean.o

TESTS = test_pg_iban

PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

$(TESTS): $(TESTS).c pg_iban_validate.c pg_iban_clean.c
	$(CC) -o $@ $^ -I. -I/usr/include -I/usr/include/postgresql/16/server -Wall -Wextra -O2 -L/usr/local/lib -L/usr/lib/postgresql/16/lib -lcmocka -lpq -lpgcommon -lpgport

docker-unit-test:
	docker build -t pg_iban_unit_tests -f Dockerfile .
	docker run --rm pg_iban_unit_tests

docker-integration-test:
	docker build -t pg_iban_integration_tests -f Dockerfile.integration .
	docker run --rm -e POSTGRES_PASSWORD=postgres -e POSTGRES_USER=postgres -e POSTGRES_DB=postgres pg_iban_integration_tests

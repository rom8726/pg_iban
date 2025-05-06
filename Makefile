EXTENSION = pg_iban
MODULES   = pg_iban
DATA      = pg_iban--1.0.sql
PG_CONFIG = pg_config

TESTS = test_pg_iban

PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

$(TESTS): $(TESTS).c pg_iban_validate.c
	$(CC) -o $@ $^ -I. -I/usr/include -I/usr/include/postgresql/16/server -Wall -Wextra -O2 -L/usr/local/lib -L/usr/lib/postgresql/16/lib -lcmocka -lpq -lpgcommon -lpgport

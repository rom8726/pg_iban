EXTENSION = pg_iban
MODULES = pg_iban
DATA = pg_iban--1.0.sql
PG_CONFIG = pg_config

PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

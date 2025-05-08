cmake_minimum_required(VERSION 3.24)
project(pg_iban C)

set(CMAKE_C_STANDARD 11)

add_executable(
        test_pg_iban
        test_pg_iban.c
        pg_iban_validate.c
        pg_iban_clean.c
        pg_iban_preprocess.c
        pg_iban_country.c
        pg_iban_bban.c
        pg_iban_format.c
)

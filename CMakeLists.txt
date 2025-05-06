cmake_minimum_required(VERSION 3.24)
project(pg_iban)

set(CMAKE_C_STANDARD 11)

add_executable(Test test_pg_iban.c pg_iban_validate.c)

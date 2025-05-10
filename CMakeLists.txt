cmake_minimum_required(VERSION 3.24)
project(pg_iban C)

set(CMAKE_C_STANDARD 11)

if(NOT DEFINED ENV{CMOCKA_INCLUDE_DIR})
   message(FATAL_ERROR "Environment variable 'CMOCKA_INCLUDE_DIR' is not set. Please define the path to CMocka include files.")
endif()

include_directories($ENV{CMOCKA_INCLUDE_DIR})

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

if(NOT DEFINED ENV{CMOCKA_LIBRARY})
    message(FATAL_ERROR "Environment variable 'CMOCKA_LIBRARY' is not set. Please define the path to CMocka dynamic library.")
endif()

target_link_libraries(test_pg_iban $ENV{CMOCKA_LIBRARY})

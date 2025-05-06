#include <stddef.h>
#include <setjmp.h>
#include <stdarg.h>
#include <cmocka.h>
#include "pg_iban_validate.h"

static void test_valid_iban(void **state) {
    assert_true(validate_pg_iban("DE89370400440532013000"));
    assert_true(validate_pg_iban("GB82WEST12345698765432"));
}

static void test_invalid_iban(void **state) {
    assert_false(validate_pg_iban("INVALIDIBAN"));
    assert_false(validate_pg_iban("12345"));
    assert_false(validate_pg_iban("DE8937040044XX32013000"));
    assert_false(validate_pg_iban("FR7612345678901234567890123"));
}

static void test_empty_iban(void **state) {
    assert_false(validate_pg_iban(""));
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_valid_iban),
        cmocka_unit_test(test_invalid_iban),
        cmocka_unit_test(test_empty_iban),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

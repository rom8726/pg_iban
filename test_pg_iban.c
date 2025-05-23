#include "def.h"
#include "pg_iban_bban.h"
#include "pg_iban_clean.h"
#include "pg_iban_country.h"
#include "pg_iban_format.h"
#include "pg_iban_validate.h"
#include <setjmp.h>
#include <stdarg.h>
#include <string.h>
#include <stddef.h>
#include <cmocka.h>

/* ---------- validate_iban_checksum tests ---------- */
static void test_checksum_valid(void** state) {
    assert_true(validate_iban_checksum("DE89370400440532013000"));
}

static void test_checksum_invalid(void** state) {
    assert_false(validate_iban_checksum("DE00370400440532013000"));
}

static void test_checksum_too_short(void** state) {
    assert_false(validate_iban_checksum("DE89"));
}

static void test_checksum_too_long(void** state) {
    char long_iban[MAX_IBAN_LENGTH + 5];
    memset(long_iban, 'A', sizeof(long_iban) - 1);
    long_iban[sizeof(long_iban) - 1] = '\0';
    assert_false(validate_iban_checksum(long_iban));
}

/* ------------------ validate_iban tests ------------------ */
static void test_valid_iban(void** state) {
    assert_true(validate_iban("DE89370400440532013000"));
    assert_true(validate_iban("GB82WEST12345698765432"));
}

static void test_invalid_iban(void** state) {
    assert_false(validate_iban("INVALIDIBAN"));
    assert_false(validate_iban("12345"));
    assert_false(validate_iban("DE8937040044XX32013000"));
    assert_false(validate_iban("FR7612345678901234567890123"));
}

static void test_empty_iban(void** state) {
    assert_false(validate_iban(""));
}

/* ------------------ clean_iban tests --------------------- */
static void test_clean_valid(void** state) {
    char dst[MAX_IBAN_LENGTH + 1];
    assert_true(clean_iban("de89 3704 0044 0532 0130 00", dst));
    assert_string_equal(dst, "DE89370400440532013000");
}

static void test_clean_invalid_char(void** state) {
    char dst[MAX_IBAN_LENGTH + 1];
    assert_false(clean_iban("DE89#3704 0044 0532 0130 00", dst));
}

static void test_clean_too_long(void** state) {
    char src[MAX_IBAN_LENGTH + 10];
    memset(src, '1', sizeof(src) - 1);
    src[sizeof(src) - 1] = '\0';

    char dst[MAX_IBAN_LENGTH + 1];
    assert_true(clean_iban(src, dst));
    assert_int_equal(strlen(dst), MAX_IBAN_LENGTH);
}

static void test_clean_empty(void** state) {
    char dst[MAX_IBAN_LENGTH + 1];
    assert_true(clean_iban("", dst));
    assert_string_equal(dst, "");
}

/* ------------------------- iban_country tests ------------------------- */
static void test_iban_country_valid(void** state) {
    char country[3];
    assert_true(iban_country("DE89370400440532013000", country));
    assert_string_equal(country, "DE");

    assert_true(iban_country("GB82WEST12345698765432", country));
    assert_string_equal(country, "GB");
}

static void test_iban_country_invalid(void** state) {
    char country[3];
    assert_false(iban_country("INVALIDIBAN", country));
    assert_false(iban_country("", country));
}

/* ------------------------- iban_bban tests ---------------------------- */
static void test_iban_bban_valid(void** state) {
    char bban[MAX_IBAN_LENGTH + 1];
    assert_true(iban_bban("DE89370400440532013000", bban));
    assert_string_equal(bban, "370400440532013000");

    assert_true(iban_bban("GB82WEST12345698765432", bban));
    assert_string_equal(bban, "WEST12345698765432");
}

static void test_iban_bban_invalid(void** state) {
    char bban[MAX_IBAN_LENGTH + 1];
    assert_false(iban_bban("INVALIDIBAN", bban));
    assert_false(iban_bban("", bban));
}

/* ------------------------- iban_format tests -------------------------- */
static void test_iban_format_valid(void** state) {
    char formatted_iban[MAX_IBAN_FORMATTED_LENGTH];
    assert_true(iban_format("DE89370400440532013000", formatted_iban));
    assert_string_equal(formatted_iban, "DE89 3704 0044 0532 0130 00");

    assert_true(iban_format("GB82WEST12345698765432", formatted_iban));
    assert_string_equal(formatted_iban, "GB82 WEST 1234 5698 7654 32");
}

static void test_iban_format_invalid(void** state) {
    char formatted_iban[MAX_IBAN_FORMATTED_LENGTH];
    assert_false(iban_format("INVALIDIBAN", formatted_iban));
    assert_false(iban_format("", formatted_iban));
}

int main(void) {
    const struct CMUnitTest tests[] = {
        /* validate_iban_checksum */
        cmocka_unit_test(test_checksum_valid),
        cmocka_unit_test(test_checksum_invalid),
        cmocka_unit_test(test_checksum_too_short),
        cmocka_unit_test(test_checksum_too_long),
        /* validate_iban */
        cmocka_unit_test(test_valid_iban),
        cmocka_unit_test(test_invalid_iban),
        cmocka_unit_test(test_empty_iban),
        /* clean_iban */
        cmocka_unit_test(test_clean_valid),
        cmocka_unit_test(test_clean_invalid_char),
        cmocka_unit_test(test_clean_too_long),
        cmocka_unit_test(test_clean_empty),
        /* iban_country */
        cmocka_unit_test(test_iban_country_valid),
        cmocka_unit_test(test_iban_country_invalid),
        /* iban_bban */
        cmocka_unit_test(test_iban_bban_valid),
        cmocka_unit_test(test_iban_bban_invalid),
        /* iban_format */
        cmocka_unit_test(test_iban_format_valid),
        cmocka_unit_test(test_iban_format_invalid),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

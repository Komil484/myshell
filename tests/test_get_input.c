#include "test_get_input.h"
#include "myutils.h"
#include <munit.h>

static MunitResult test_simple(const MunitParameter params[], void* user_data_or_fixture)
{
    FILE *test_file = fopen(TEST_FILE_PATH "/get_input/test1.txt", "r");

    char *input = fget_input(test_file);
    munit_assert_string_equal(input, "abcdef");

    fclose(test_file);

    return MUNIT_OK;
}

#define LEN 5358
static MunitResult test_long(const MunitParameter params[], void* user_data_or_fixture)
{
    char expected_out[LEN];
    for (int i = 0; i < LEN - 2; ++i) {
        expected_out[i] = 'a';
    }
    expected_out[LEN - 2] = 'z';
    expected_out[LEN - 1] = '\0';

    FILE *test_file = fopen(TEST_FILE_PATH "/get_input/test2.txt", "r");

    munit_assert_string_equal(fget_input(test_file), expected_out);

    fclose(test_file);

    return MUNIT_OK;
}

static MunitTest tests[] = {
    {
        "/simple", /* name */
        test_simple, /* test */
        NULL, /* setup */
        NULL, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    {
        "/long", /* name */
        test_long, /* test */
        NULL, /* setup */
        NULL, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    /* Mark the end of the array with an entry where the test
   * function is NULL */
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

const MunitSuite get_input_suite = {
    "/get_input", /* name */
    tests, /* tests */
    NULL, /* suites */
    1, /* iterations */
    MUNIT_SUITE_OPTION_NONE /* options */
};

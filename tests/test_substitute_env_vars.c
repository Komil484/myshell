#include "test_substitute_env_vars.h"
#include "myutils.h"
#include <munit.h>
#include <stdlib.h>

static MunitResult test_simple(const MunitParameter params[], void* user_data_or_fixture)
{
    setenv("MYSH_TEST_VAR", "simple value", 1);
    char *input = "$MYSH_TEST_VAR";

    char *expected = "`simple value`";

    char *result = substitute_env_vars(input);
    munit_assert_string_equal(result, expected);

    free(result);

    unsetenv("MYSH_TEST_VAR");
    return MUNIT_OK;
}

static MunitResult test_quotes_value(const MunitParameter params[], void* user_data_or_fixture)
{
    setenv("MYSH_TEST_VAR", "`random\"quotes'", 1);
    char *input = "$MYSH_TEST_VAR";

    char *expected = "`\\`random\"quotes'`";

    char *result = substitute_env_vars(input);
    munit_assert_string_equal(result, expected);

    free(result);

    unsetenv("MYSH_TEST_VAR");
    return MUNIT_OK;
}

static MunitResult test_escape_chars(const MunitParameter params[], void* user_data_or_fixture)
{
    setenv("MYSH_TEST_VAR", "escape\\chars\\", 1);
    char *input = "$MYSH_TEST_VAR";

    char *expected = "`escape\\\\chars\\\\`";

    char *result = substitute_env_vars(input);
    munit_assert_string_equal(result, expected);

    free(result);

    unsetenv("MYSH_TEST_VAR");
    return MUNIT_OK;
}

static MunitResult test_long_val(const MunitParameter params[], void* user_data_or_fixture)
{
    setenv("MYSH_TEST_VAR", "variable value longer than identifier", 1);
    char *input = "$MYSH_TEST_VAR";

    char *expected = "`variable value longer than identifier`";

    char *result = substitute_env_vars(input);
    munit_assert_string_equal(result, expected);

    free(result);

    unsetenv("MYSH_TEST_VAR");
    return MUNIT_OK;
}

static MunitResult test_quotes_input(const MunitParameter params[], void* user_data_or_fixture)
{
    setenv("MYSH_TEST_VAR", "value", 1);
    char *input = "'single'\"double\"`backticks`$MYSH_TEST_VAR";

    char *expected = "'single'\"double\"`backticks``value`";

    char *result = substitute_env_vars(input);
    munit_assert_string_equal(result, expected);

    free(result);

    unsetenv("MYSH_TEST_VAR");
    return MUNIT_OK;
}

static MunitResult test_dollar_in_quotes(const MunitParameter params[], void* user_data_or_fixture)
{
    char *input = "'$MYSH_TEST_VAR'";

    char *expected = "'$MYSH_TEST_VAR'";

    char *result = substitute_env_vars(input);
    munit_assert_string_equal(result, expected);

    free(result);

    unsetenv("MYSH_TEST_VAR");
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
        "/quotes_value", /* name */
        test_quotes_value, /* test */
        NULL, /* setup */
        NULL, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    {
        "/escape_chars", /* name */
        test_escape_chars, /* test */
        NULL, /* setup */
        NULL, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    {
        "/long_val", /* name */
        test_long_val, /* test */
        NULL, /* setup */
        NULL, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    {
        "/quotes_input", /* name */
        test_quotes_input, /* test */
        NULL, /* setup */
        NULL, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    {
        "/dollar_in_quotes", /* name */
        test_dollar_in_quotes, /* test */
        NULL, /* setup */
        NULL, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    /* Mark the end of the array with an entry where the test
   * function is NULL */
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

const MunitSuite substitute_env_vars_suite = {
    "/substitute_env_vars", /* name */
    tests, /* tests */
    NULL, /* suites */
    1, /* iterations */
    MUNIT_SUITE_OPTION_NONE /* options */
};

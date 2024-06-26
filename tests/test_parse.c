#include "test_parse.h"
#include "myutils.h"
#include <munit.h>
#include <stdlib.h>

void assert_arr_string(char **ptr, char **expected)
{
    unsigned i = 0;
    for (; expected[i] != NULL; ++i) {
        munit_assert_string_equal(ptr[i], expected[i]);
    }
    munit_assert_ptr_equal(ptr[i], expected[i]);
}

static MunitResult test_simple(const MunitParameter params[], void* user_data_or_fixture)
{
    char input[] = "cmd arg1 arg2";

    char *expected[] = {"cmd", "arg1", "arg2", NULL};

    char **parsed = parse(input);
    assert_arr_string(parsed, expected);
    free_parsed(parsed);

    return MUNIT_OK;
}

static MunitResult test_quotes(const MunitParameter params[], void* user_data_or_fixture)
{
    char input[] = "cmd \"double quotes\" 'single quotes'";

    char *expected[] = {"cmd", "double quotes", "single quotes", NULL};

    char **parsed = parse(input);
    assert_arr_string(parsed, expected);
    free_parsed(parsed);

    return MUNIT_OK;
}

static MunitResult test_escape_chars(const MunitParameter params[], void* user_data_or_fixture)
{
    char input[] = "cmd \"\\nescape chars\n\\\\\" '\\nraw chars\\\\'";

    char *expected[] = {"cmd", "\nescape chars\n\\", "\\nraw chars\\\\", NULL};

    char **parsed = parse(input);
    assert_arr_string(parsed, expected);
    free_parsed(parsed);

    return MUNIT_OK;
}

static MunitResult test_consec_quotes(const MunitParameter params[], void* user_data_or_fixture)
{
    char input[] = "cmd 'raw\\n\\\\'\"consec quotes\"";

    char *expected[] = {"cmd", "raw\\n\\\\consec quotes", NULL};

    char **parsed = parse(input);
    assert_arr_string(parsed, expected);
    free_parsed(parsed);

    return MUNIT_OK;
}

static MunitResult test_weird_escapes(const MunitParameter params[], void* user_data_or_fixture)
{
    char input[] = "cmd \"\\weird\\escapes\"";

    char *expected[] = {"cmd", "\\weird\\escapes", NULL};

    char **parsed = parse(input);
    assert_arr_string(parsed, expected);
    free_parsed(parsed);

    return MUNIT_OK;
}

static MunitResult test_backticks(const MunitParameter params[], void* user_data_or_fixture)
{
    char input[] = "cmd `backticks`";

    char *expected[] = {"cmd", "backticks", NULL};

    char **parsed = parse(input);
    assert_arr_string(parsed, expected);
    free_parsed(parsed);

    return MUNIT_OK;
}

static MunitResult test_backticks_escape(const MunitParameter params[], void* user_data_or_fixture)
{
    char input[] = "cmd `\\\\backticks\\`escape`\\n";

    char *expected[] = {"cmd", "\\\\backticks`escape\\n", NULL};

    char **parsed = parse(input);
    assert_arr_string(parsed, expected);
    free_parsed(parsed);

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
        "/quotes", /* name */
        test_quotes, /* test */
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
        "/consec_quotes", /* name */
        test_consec_quotes, /* test */
        NULL, /* setup */
        NULL, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    {
        "/weird_escapes", /* name */
        test_weird_escapes, /* test */
        NULL, /* setup */
        NULL, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    {
        "/backticks", /* name */
        test_backticks, /* test */
        NULL, /* setup */
        NULL, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    {
        "/backticks_escape", /* name */
        test_backticks_escape, /* test */
        NULL, /* setup */
        NULL, /* tear_down */
        MUNIT_TEST_OPTION_NONE, /* options */
        NULL /* parameters */
    },
    /* Mark the end of the array with an entry where the test
   * function is NULL */
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

const MunitSuite parse_suite = {
    "/parse", /* name */
    tests, /* tests */
    NULL, /* suites */
    1, /* iterations */
    MUNIT_SUITE_OPTION_NONE /* options */
};

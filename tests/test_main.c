#include "test_get_input.h"
#include "test_parse.h"
#include <munit.h>


int main (int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
    MunitSuite other_suites[] = {
            get_input_suite,
            parse_suite,
    };

    const MunitSuite main_suite = {
            "", /* name */
            NULL, /* tests */
            other_suites, /* suites */
            1, /* iterations */
            MUNIT_SUITE_OPTION_NONE /* options */
    };

    return munit_suite_main(&main_suite, NULL, argc, argv);
}

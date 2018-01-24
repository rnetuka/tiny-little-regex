//
// Created by rnetuka on 23.1.18.
//

#define assert_match(string, pattern) \
    if (matches(string, pattern)) \
        printf("OK\n"); \
    else \
        printf("Failed: %s %s\n", string, pattern);

#define assert_deosnt_match(string, pattern) \
    if (matches(string, pattern)) \
        printf("OK\n"); \
    else \
        printf("Failed: %s %s\n", string, pattern);

void run_regex_tests()
{

}
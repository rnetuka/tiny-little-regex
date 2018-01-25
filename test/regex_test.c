//
// Created by rnetuka on 23.1.18.
//

#include "myassert.h"
#include "../regex.h"

#define assert_matches(string, regex) assert_true(matches(string, regex));
#define assert_doesnt_match(string, regex) assert_false(matches(string, regex));

static void test_char_sequence_match()
{
    assert_matches("abcd", "abcd");
}

static void test_char_set_match()
{
    assert_matches("a", "[a-c]");
    assert_doesnt_match("d", "[a-c]");
}

static void test_number_set_match()
{
    for (int i = 0; i < 10; i++)
    {
        char string[2];
        sprintf(string, "%i", i);
        assert_matches(string, "[0-9]");
    }
}

static void test_star_quantifier()
{
    printf("Testing regex containing * quantifier\n");

    assert_matches("", "a*");
    assert_matches("aaa", "a*");
}

static void test_plus_quantifier()
{
    printf("Testing regex containing + quantifier\n");

    assert_doesnt_match("", "a+");
    assert_matches("a", "a+");
    assert_matches("aaa", "a+");
}

static void test_question_mark_quantifier()
{
    printf("Testing regex containing ? quantifier\n");

    assert_matches("a", "ab?");
    assert_matches("ab", "ab?");
}

static void test_fixed_quantifier()
{
    printf("Testing regex containing {n} quantifier\n");

    assert_doesnt_match("a", "a{2}");
    assert_matches("aa", "a{2}");
    assert_doesnt_match("aaa", "a{2}");
}

void run_regex_tests()
{
    test_char_sequence_match();
    test_char_set_match();
    test_number_set_match();
    test_star_quantifier();
    test_plus_quantifier();
    test_question_mark_quantifier();
    test_fixed_quantifier();
}


//
// Created by rnetuka on 23.1.18.
//

#include "myassert.h"
#include "../quantifier.h"

static void test_star_quantifier()
{
    printf("Testing * quantifier\n");

    Quantifier *quantifier = quantifier_new("*");
    assert_equals(quantifier->type, range);
    assert_equals(quantifier->min, 0);
    assert_equals(quantifier->max, -1);
    quantifier_free(quantifier);
}

static void test_plus_quantifier()
{
    printf("Testing + quantifier\n");

    Quantifier *quantifier = quantifier_new("+");
    assert_equals(quantifier->type, range);
    assert_equals(quantifier->min, 1);
    assert_equals(quantifier->max, -1);
    quantifier_free(quantifier);
}

static void test_question_mark_quantifier()
{
    printf("Testing ? quantifier\n");

    Quantifier *quantifier = quantifier_new("?");
    assert_equals(quantifier->type, range);
    assert_equals(quantifier->min, 0);
    assert_equals(quantifier->max, 1);
    quantifier_free(quantifier);
}

static void test_fixed_quantifier()
{
    printf("Testing fixed quantifier\n");

    Quantifier *quantifier = quantifier_new("{15}");
    assert_equals(quantifier->type, fixed);
    assert_equals(quantifier->value, 15);
    quantifier_free(quantifier);
}

static void test_range_quantifier()
{
    printf("Testing range quantifier\n");

    Quantifier *quantifier = quantifier_new("{2,4}");
    assert_equals(quantifier->type, range);
    assert_equals(quantifier->min, 2);
    assert_equals(quantifier->max, 4);

    quantifier_free(quantifier);

    quantifier = quantifier_new("{,4}");
    assert_equals(quantifier->type, range);
    assert_equals(quantifier->min, 0);
    assert_equals(quantifier->max, 4);

    quantifier_free(quantifier);

    quantifier = quantifier_new("{2,}");
    assert_equals(quantifier->type, range);
    assert_equals(quantifier->min, 2);
    assert_equals(quantifier->max, -1);
    quantifier_free(quantifier);
}

static void test_invalid_quantifier()
{
    printf("Testing invalid quantifiers\n");

    Quantifier *quantifier = quantifier_new("/");
    assert_equals(quantifier, NULL);

    quantifier = quantifier_new("{a}");
    assert_equals(quantifier, NULL);

    quantifier = quantifier_new("10");
    assert_equals(quantifier, NULL);

    quantifier = quantifier_new("{,}");
    assert_equals(quantifier, NULL);

    quantifier = quantifier_new("{3,1}");
    assert_equals(quantifier, NULL);
}

int run_quantifier_tests()
{
    freopen("/dev/null", "w", stderr);

    test_star_quantifier();
    test_plus_quantifier();
    test_question_mark_quantifier();
    test_fixed_quantifier();
    test_range_quantifier();
    test_invalid_quantifier();
}
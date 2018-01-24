//
// Created by rnetuka on 23.1.18.
//

#pragma once

#include <stdio.h>

#define assert_equals(actual, expected) \
    if ((actual) == (expected)) \
        printf("OK\n"); \
    else \
        printf("Failed\n");
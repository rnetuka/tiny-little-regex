//
// Created by rnetuka on 2.1.18.
//

#include "pattern.h"
#include "regex.h"

bool matches(const char *word, const char *regex)
{
    Pattern *pattern = pattern_new(regex);
    pattern_compile(pattern);

    bool result = state_machine_accepts(pattern->state_machine, word);
    pattern_free(pattern);

    return result;
}
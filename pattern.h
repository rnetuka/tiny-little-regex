//
// Created by rnetuka on 2.1.18.
//

#pragma once

#include "statemach.h"
#include "token.h"
#include "util/list.h"

typedef struct
{
    const char *string;
    List *tokens;
    StateMachine *state_machine;
}
Pattern;

Pattern *pattern_new(const char *string);
void pattern_free(Pattern *pattern);
void pattern_compile(Pattern *pattern);
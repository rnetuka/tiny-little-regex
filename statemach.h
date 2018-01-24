//
// Created by rnetuka on 2.1.18.
//

#pragma once

#include <stdbool.h>

#include "list.h"
#include "stack.h"
#include "token.h"

struct StateMachine;

typedef struct
{
    bool accepting;
    void (*on_enter)(struct StateMachine *state_machine, int param);
    void (*on_leave)(struct StateMachine *state_machine);
    int on_enter_param;
}
State;

typedef struct
{
    State *from;
    Token *input;
    State *to;
    char *stack_top;
}
Rule;

typedef struct StateMachine
{
    List *states;
    State * initial_state;
    State *sink_state;
    State *current_state;
    List *rules;
    Stack *stack;
}
StateMachine;

StateMachine *state_machine_new();
void state_machine_free(StateMachine *state_machine);
State *state_machine_add_state(StateMachine *state_machine);

#define state_machine_add_rule(state_machine, ...) state_machine_add_rule_(state_machine, (Rule) { __VA_ARGS__ })
void state_machine_add_rule_(StateMachine *state_machine, Rule params);

bool state_machine_accepts(StateMachine *state_machine, const char *word);
//
// Created by rnetuka on 2.1.18.
//

#include <stdlib.h>

#include "pattern.h"
#include "statemach.h"
#include "token.h"
#include "quantifier.h"

Pattern *pattern_new(const char *string)
{
    Pattern *pattern = malloc(sizeof(Pattern));
    pattern->string = string;
    tokenize(string, &(pattern->tokens));
    return pattern;
}

void pattern_free(Pattern *pattern)
{
    list_apply(pattern->tokens, token_free);
    list_free(pattern->tokens);

    if (pattern->state_machine)
        state_machine_free(pattern->state_machine);

    free(pattern);
}

static void push_to_stack(struct StateMachine *state_machine, int n)
{
    for (int i = 0; i < n; i++)
        stack_push(state_machine->stack, 'x');
}

static void clear_stack(struct StateMachine *state_machine)
{
    stack_clear(state_machine->stack);
}

void pattern_compile(Pattern *pattern)
{
    StateMachine *state_machine = state_machine_new();

    State *connection_point = state_machine->initial_state;

    for (int i = 0; i < list_size(pattern->tokens); i++)
    {
        Token *token = list_get(pattern->tokens, i);

        for (int j = 0; j < token->quantifier->min; j++)
        {
            State *state = state_machine_add_state(state_machine);
            state_machine_add_rule(state_machine, connection_point, token, state);
            connection_point = state;
        }

        if (token->quantifier->max == -1)
        {
            state_machine_add_rule(state_machine, connection_point, token, connection_point);
        }
        else if (token->quantifier->min == 0 || token->quantifier->min != token->quantifier->max)
        {
            int repetitions = token->quantifier->max - token->quantifier->min;

            connection_point->on_enter = push_to_stack;
            connection_point->on_enter_param = repetitions;
            connection_point->on_leave = clear_stack;

            state_machine_add_rule(state_machine,
                                   .from=connection_point,
                                   .input=token,
                                   .stack_top="x",
                                   .to=connection_point);
        }

        bool last_token = (i == list_size(pattern->tokens) - 1);

        if (last_token)
            connection_point->accepting = true;
    }
    pattern->state_machine = state_machine;
}
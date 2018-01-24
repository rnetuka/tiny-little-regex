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
    for (int i = 0; pattern->tokens[i] != NULL; i++)
        token_free(pattern->tokens[i]);

    free(pattern->tokens);

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

    for (int i = 0; pattern->tokens[i] != NULL; i++)
    {
        Token *token = pattern->tokens[i];
        bool last_token = pattern->tokens[i + 1] == NULL;

        switch (token->quantifier->type)
        {
            case fixed:
            {
                State *state;

                for (int j = 0; j < token->quantifier->value; j++)
                {
                    state = state_machine_add_state(state_machine);
                    state_machine_add_rule(state_machine, connection_point, token, state);
                    connection_point = state;
                }

                if (last_token)
                    connection_point->accepting = true;

                break;
            }

            case range:
            {
                State *state;

                for (int j = 0; j < token->quantifier->min; j++)
                {
                    state = state_machine_add_state(state_machine);
                    state_machine_add_rule(state_machine, connection_point, token, state);
                    connection_point = state;
                }

                if (token->quantifier->max == -1)
                {
                    state_machine_add_rule(state_machine, connection_point, token, connection_point);
                }
                else
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

                if (last_token)
                    connection_point->accepting = true;

                break;
            }

            /*case one_or_more:
            {
                State *state = state_machine_add_state(state_machine);

                state_machine_add_rule(state_machine, connection_point, token, state);
                state_machine_add_rule(state_machine, state, token, state);

                connection_point = state;

                if (last_token)
                    connection_point->accepting = true;

                break;
            }

            case zero_or_more:
                state_machine_add_rule(state_machine, connection_point, token, connection_point);

                if (last_token)
                    connection_point->accepting = true;

                break;*/
        }
    }
    pattern->state_machine = state_machine;
}
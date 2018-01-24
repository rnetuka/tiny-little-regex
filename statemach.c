//
// Created by rnetuka on 2.1.18.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "statemach.h"

static State *state_new()
{
    State *state = malloc(sizeof(State));
    state->accepting = false;
    state->on_enter = NULL;
    state->on_leave = NULL;
    state->on_enter_param = 0;
    return state;
}

static Rule *rule_new(Rule params)
{
    Rule *rule = malloc(sizeof(Rule));
    *rule = params;
    return rule;
}

StateMachine *state_machine_new()
{
    StateMachine *state_machine = malloc(sizeof(StateMachine));
    state_machine->states = list_new();
    state_machine->rules = list_new();
    state_machine->initial_state = state_machine_add_state(state_machine);
    state_machine->sink_state = state_machine_add_state(state_machine);
    state_machine->stack = stack_new();
    return state_machine;
}

void state_machine_free(StateMachine *state_machine)
{
    for (int i = 0; i < list_size(state_machine->states); i++)
        free(list_get(state_machine->states, i));

    for (int i = 0; i < list_size(state_machine->rules); i++)
        free(list_get(state_machine->rules, i));

    list_free(state_machine->states);
    list_free(state_machine->rules);

    stack_free(state_machine->stack);

    free(state_machine);
}

State *state_machine_add_state(StateMachine *state_machine)
{
    State *state = state_new();
    list_add(state_machine->states, state);
    return state;
}

void state_machine_add_rule_(StateMachine *state_machine, Rule params)
{
    Rule *rule = rule_new(params);
    list_add(state_machine->rules, rule);
}

static void leave_current_state(StateMachine *state_machine)
{
    if (state_machine->current_state->on_leave)
        state_machine->current_state->on_leave(state_machine);
}

static void enter_current_state(StateMachine *state_machine)
{
    if (state_machine->current_state->on_enter)
        state_machine->current_state->on_enter(state_machine, state_machine->current_state->on_enter_param);
}

static Rule *state_machine_find_rule(const StateMachine *state_machine, char input)
{
    for (int i = 0; i < list_size(state_machine->rules); i++)
    {
        Rule *rule = list_get(state_machine->rules, i);
        if (state_machine->current_state == rule->from  && token_matches(rule->input, input))
            if (rule->stack_top)
                if (!stack_is_empty(state_machine->stack) && *rule->stack_top == *(stack_top(state_machine->stack)))
                    return rule;
                else
                    return NULL;
            else
                return rule;
    }
    return NULL;
}

static void state_machine_read_char(StateMachine *state_machine, char input)
{
    Rule *rule = state_machine_find_rule(state_machine, input);

    if (rule)
    {
        if (rule->stack_top)
            stack_pop(state_machine->stack);

        if (rule->from != rule->to)
            leave_current_state(state_machine);

        state_machine->current_state = rule->to;

        if (rule->from != rule->to)
            enter_current_state(state_machine);
    }
    else
        state_machine->current_state = state_machine->sink_state;
}

static void state_machine_read_word(StateMachine *state_machine, const char *word)
{
    state_machine->current_state = state_machine->initial_state;
    enter_current_state(state_machine);

    for (int i = 0; i < strlen(word); i++)
        state_machine_read_char(state_machine, word[i]);
}

bool state_machine_accepts(StateMachine *state_machine, const char *word)
{
    state_machine_read_word(state_machine, word);
    return state_machine->current_state->accepting;
}
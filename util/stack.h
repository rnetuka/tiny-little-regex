//
// Created by rnetuka on 9.1.18.
//

#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct
{
    size_t size;
    size_t capacity;
    char *elements;
}
Stack;

Stack *stack_new();
void stack_free(Stack *stack);
void stack_push(Stack *stack, char element);
char *stack_top(const Stack *stack);
char *stack_pop(Stack *stack);
bool stack_is_empty(const Stack *stack);
void stack_clear(Stack *stack);
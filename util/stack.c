//
// Created by rnetuka on 9.1.18.
//

#include <stdlib.h>

#include "stack.h"

Stack *stack_new()
{
    Stack *stack = malloc(sizeof(Stack));
    stack->size = 0;
    stack->capacity = 20;
    stack->elements = malloc(stack->capacity);
    return stack;
}

void stack_free(Stack *stack)
{
    free(stack->elements);
    free(stack);
}

void stack_push(Stack *stack, char element)
{
    if (stack->size == stack->capacity)
    {
        stack->capacity += 10;
        stack->elements = realloc(stack->elements, stack->capacity);
    }
    stack->elements[stack->size] = element;
    stack->size++;
}

char *stack_top(const Stack *stack)
{
    return stack->size == 0 ? NULL : &(stack->elements[0]);
}

char *stack_pop(Stack *stack)
{
    void *result = stack_top(stack);

    if (! stack_is_empty(stack))
        stack->size--;

    return result;
}

bool stack_is_empty(const Stack *stack)
{
    return stack->size == 0;
}

void stack_clear(Stack *stack)
{
    stack->size = 0;
}
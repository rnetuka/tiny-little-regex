//
// Created by rnetuka on 10.1.18.
//

#include <stdlib.h>

#include "list.h"

List *list_new()
{
    List *list = malloc(sizeof(List));
    list->capacity = 10;
    list->size = 0;
    list->elements = malloc(list->capacity * sizeof(void *));
    return list;
}

void list_free(List *list)
{
    free(list->elements);
    free(list);
}

void list_add(List *list, void *element)
{
    if (list->size == list->capacity)
    {
        list->capacity += 10;
        realloc(list->elements, list->capacity * sizeof(void *));
    }
    list->elements[list->size++] = element;
}

void list_clear(List *list)
{
    list->size = 0;
}

size_t list_size(const List *list)
{
    return list->size;
}

void *list_get(const List *list, int i)
{
    if (i < 0 || i >= list->size)
        return NULL;

    return list->elements[i];
}

void list_apply(List *list, void (*function)(void *))
{
    for (int i = 0; i < list_size(list); i++)
        function(list_get(list, i));
}
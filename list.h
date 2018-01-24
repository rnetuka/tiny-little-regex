//
// Created by rnetuka on 10.1.18.
//

#pragma once

#include <stddef.h>

typedef struct
{
    void **elements;
    size_t capacity;
    size_t size;
}
List;

List *list_new();
void list_free(List *list);
void list_add(List *list, void *element);
void list_clear(List *list);
size_t list_size(const List *list);
void *list_get(const List *list, int i);

void list_apply(List *list, void (*function)(void *));
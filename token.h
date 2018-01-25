//
// Created by rnetuka on 4.1.18.
//

#pragma once

#include <stddef.h>

#include "quantifier.h"
#include "util/list.h"

typedef enum
{
    character,
    line_beginning,
    line_end,
    wildcard,
    any_of
}
TokenType;

typedef struct
{
    char *string;
    char *inner_string;
    TokenType type;
    Quantifier *quantifier;
}
Token;

Token *token_new(const char *string);
void token_free(Token *token);
bool token_matches(const Token *token, char character);

void tokenize(const char *string, List **placeholder);
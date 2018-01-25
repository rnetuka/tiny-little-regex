//
// Created by rnetuka on 4.1.18.
//

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "util/list.h"

Token *token_new(const char *string)
{
    if (strlen(string) == 0)
        goto error;

    char *inner_string;

    TokenType type = character;

    if (strcmp(string, ".") == 0)
        type = wildcard;

    else if (strcmp(string, "^") == 0)
        type = line_beginning;

    else if (strcmp(string, "$") == 0)
        type = line_end;

    else if (strcmp(string, "\\d") == 0)
        string = "[0-9]";

    else if (strcmp(string, "\\D") == 0)
        string = "[^0-9]";

    else if (strcmp(string, "\\s") == 0)
        string = "[ \t\r\n\f]";

    else if (strcmp(string, "\\S") == 0)
        string = "[^ \t\r\n\f]";

    else if (strcmp(string, "\\.") == 0)
        string = ".";

    else if (strcmp(string, "\\^") == 0)
        string = "^";

    else if (strcmp(string, "\\$") == 0)
        string = "$";

    else if (strcmp(string, "\\[") == 0)
        string = "[";

    else if (strcmp(string, "\\]") == 0)
        string = "]";

    if (strncmp(string, "[", 1) == 0 && strlen(string) >= 3)
    {
        size_t max_size = strlen(string)
                          + strlen("abcdefghijklmnopqrstuvwxyz")
                          + strlen("ABCDEFGHIJKLMNOPQRSTUVWXYZ")
                          + strlen("0123456789");

        char characters[max_size];
        characters[0] = '\0';

        for (int i = 0; i < strlen(string); i++)
        {
            // TODO escape \\-
            if (string[i] == '-')
            {
                char min = string[i - 1];
                char max = string[i + 1];

                if (min > max)
                    return NULL;

                for (char j = min + 1; j < max; j++)
                {
                    char str[2] = {j, '\0'};
                    strncat(characters, str, 1);
                }
            }
            else
            {
                strncat(characters, &string[i], 1);
            }
        }
        inner_string = calloc(strlen(characters) + 1, sizeof(char));
        strcpy(inner_string, characters);
        type = any_of;

    }
    else if (strlen(string) == 1)
    {
        inner_string = malloc(2);
        strcpy(inner_string, string);
    }
    else
    {
        goto error;
    }

    Token *token = malloc(sizeof(Token));
    token->string = malloc(strlen(string) + 1);

    strcpy(token->string, string);

    token->inner_string = inner_string;
    token->type = type;
    token->quantifier = NULL;
    return token;

    error:
    fprintf(stderr, "Invalid regex token string %s", string);
    return NULL;
}

void token_free(Token *token)
{
    if (token->quantifier)
        quantifier_free(token->quantifier);

    free(token->inner_string);
    free(token);
}

const char *token_string(const Token *token)
{
    return token->string;
}

bool token_matches(const Token *token, char c)
{
    if (token->type == wildcard)
        return true;

    if (token->type == line_beginning)
        return false;

    if (token->type == line_end)
        return false;

    if (token->type == character)
        return token->inner_string[0] == c;

    if (token->type == any_of)
    {
        int start = 1;
        int end = (int) strlen(token->inner_string) - 1;

        bool negation = false;

        if (token->inner_string[1] == '^')
        {
            negation = true;
            start++;
        }

        bool result = false;

        for (int i = start; i < end; i++)
        {
            if (token->inner_string[i] == c)
            {
                result = true;
                break;
            }
        }

        if (negation)
            result = !result;

        return result;
    }
    return false;
}

void tokenize(const char *string, Token ***placeholder)
{
    List *tokens = list_new();

    for (int i = 0; i < strlen(string);)
    {
        char character = string[i];

        Token *token;

        if (character == '\\')
        {
            if (i + 1 >= strlen(string))
                goto error;

            char next = string[i + 1];

            token = token_new((char[2]) { next, '\0' });
            i += 2;
        }
        else if (character == '[')
        {
            char *p = strchr(string + i, ']');

            if (!p)
                goto error;

            size_t length = p - string - i + 1;
            char substring[length + 1];

            for (int j = 0; j < length; j++)
                substring[j] = *(string + i + j);

            substring[length] = '\0';

            token = token_new(substring);
            i += strlen(substring);
        }
        else
        {
            token = token_new((char[2]) {character, '\0'});
            i++;
        }

        if (i < strlen(string))
        {
            char next = string[i];

            if (next == '*' || next == '+' || next == '?')
            {
                token->quantifier = quantifier_new((char[2]) { next, '\0' });
                i++;
            }
            else if (next == '{')
            {
                char *p = strchr(string + i, '}');

                if (!p)
                    goto error;

                size_t length = p - string - i + 1;
                char substring[length + 1];

                for (int j = 0; j < length; j++)
                    substring[j] = *(string + i + j);

                substring[length] = '\0';

                token->quantifier = quantifier_new(substring);
                i += strlen(substring);
            }
        }
        if (! token->quantifier)
            token->quantifier = quantifier_new("{1}");

        list_add(tokens, token);
    }

    *placeholder = malloc((list_size(tokens) + 1) * sizeof(Token));

    for (int i = 0; i < list_size(tokens); i++)
        (*placeholder)[i] = list_get(tokens, i);

    (*placeholder)[list_size(tokens)] = NULL;
    goto finally;

    error:
    fprintf(stderr, "Invalid regex: %s", string);
    list_apply(tokens, token_free);

    finally:
    list_free(tokens);
}
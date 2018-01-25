//
// Created by rnetuka on 4.1.18.
//

#define _GNU_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "quantifier.h"

#define strtoi(string, error) (int) strtol(string, error, 10)

Quantifier *quantifier_new(const char *string)
{
    if (!string || strlen(string) == 0)
        goto error;

    if (strcmp(string, "*") == 0)
        string = "{0,}";

    else if (strcmp(string, "+") == 0)
        string = "{1,}";

    else if (strcmp(string, "?") == 0)
        string = "{0,1}";

    else if (string[0] != '{')
        goto error;

    else if (string[strlen(string) - 1] != '}')
        goto error;

    int min, max;

    if (strchr(string, ','))        // range {n,m}
    {
        char working_copy[strlen(string) + 1];
        strcpy(working_copy, string);

        char *part1 = strtok(working_copy, ",");
        char *part2 = strtok(NULL, ",");

        if (strtok(NULL, ","))  // contains more than one comma
            goto error;

        if (strcmp(part1, "{") == 0 && strcmp(part2, "}") == 0)     // {,}
            goto error;

        if (strcmp(part1, "{") == 0)
            min = 0;
        else
        {
            part1 += 1;

            char *err;
            min = strtoi(part1, &err);

            if (*err)
                goto error;
        }

        if (strcmp(part2, "}") == 0)
            max = -1;
        else
        {
            part2[strlen(part2) - 1] = '\0';

            char *err;
            max = strtoi(part2, &err);

            if (*err)
                goto error;
        }

        bool limited_range = max != -1;

        if (limited_range && min > max)
            goto error;
    }
    else                            // fixed count {n}
    {
        char working_copy[strlen(string) - 1];
        strncpy(working_copy, string + 1, strlen(string) - 2);
        working_copy[strlen(string) - 2] = '\0';

        char *err;
        int n = (int) strtol(working_copy, &err, 10);

        if (*err)
            goto error;

        min = max = n;
    }

    Quantifier *quantifier = malloc(sizeof(Quantifier));
    asprintf(&quantifier->string, "%s", string);
    quantifier->min = min;
    quantifier->max = max;
    return quantifier;

    error:
    fprintf(stderr, "Invalid regex quantifier: %s", string);
    return NULL;
}

void quantifier_free(Quantifier *quantifier)
{
    free(quantifier->string);
    free(quantifier);
}
//
// Created by rnetuka on 4.1.18.
//

#pragma once

typedef enum
{
    fixed,
    range
}
QuantifierType;

typedef struct
{
    char *string;
    QuantifierType type;
    union
    {
        int value;
        struct
        {
            int min;
            int max;
        };
    };
}
Quantifier;



Quantifier *quantifier_new(const char *string);
void quantifier_free(Quantifier *quantifier);
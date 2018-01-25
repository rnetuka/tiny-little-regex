//
// Created by rnetuka on 4.1.18.
//

#pragma once

typedef struct
{
    char *string;
    int min;
    int max;
}
Quantifier;

/**
 * Creates a new regex quantifier.
 * https://www.regular-expressions.info/refrepeat.html
 *
 * Supported strings are:
 * <ul>
 *   <li>"*"</li>
 *   <li>"+"</li>
 *   <li>"?"</li>
 *   <li>"{n}"</li>
 *   <li>"{n,m}"</li>
 *   <li>"{n,}", where n >= 0</li>
 *   <li>"{,m}", where m >= 1</li>
 * </ul>
 *
 * @param   string
 *          string with regex quantifier
 *
 * @return  new quantifier or <code>NULL</code>, if provided string is not valid
 */
Quantifier *quantifier_new(const char *string);

void quantifier_free(Quantifier *quantifier);
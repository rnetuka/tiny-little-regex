#include <stdio.h>
#include <string.h>

#include "regex.h"
#include "test/tests.h"

int main(int argc, char *argv[])
{
    if (argc == 2 && strcmp(argv[1], "test") == 0)
    {
        run_tests();
        return 0;
    }

    if (argc == 3)
    {
        const char *string = argv[1];
        const char *regex = argv[2];

        if (matches(string, regex))
            printf("Matches\n");
        else
            printf("Doesn't match\n");
    }
}
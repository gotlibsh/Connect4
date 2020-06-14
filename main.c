#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "c4_utils.h"

extern void run_tests();
extern void play();

int main(int argc, char* argv[])
{
    if (argc > 2)
    {
        printf("usage: %s [test]\n", argv[0]);
        return 1;
    }
    if (argc == 2)
    {
        if (strcmp(argv[1], "test") == 0)
        {
            run_tests();
            printf("Tests Passed\n");
            return 0;
        }
        else
        {
            printf("usage: %s [test]\n", argv[0]);
            return 1;
        }
    }

    play();
    
    return 0;
}
#include <stdio.h>
#include <stdint.h>
#include "c4_utils.h"


void print_usage(char* program_name)
{
    printf("Usage: %s {play|util|test}\n", program_name);
}

extern void run_tests();
extern bool play(int argc, char* argv[]);
extern bool util(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        print_usage(argv[0]);
        return 1;
    }
    if (strcmp(argv[1], "play") == 0)
    {
        if (!play(argc-2, &argv[2]))
        {
            print_usage(argv[0]);
            return 1;
        }
        return 0;
    }
    if (strcmp(argv[1], "util") == 0)
    {
        if (!util(argc-2, &argv[2]))
        {
            print_usage(argv[0]);
            return 1;
        }
        return 0;
    }
    if (strcmp(argv[1], "test") == 0)
    {
        run_tests();
        printf("Tests Passed\n");
        return 0;
    }
    
    print_usage(argv[1]);
    return 1;
}
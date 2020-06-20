#include <stdio.h>
#include <stdint.h>
#include "c4_utils.h"


void print_usage()
{
    printf("Usage: Connect4 [options]\n");
    printf("Options:\n");
    printf("      play [<machine|human> [depth=N]]     play against the machine and specify the starting player\n");
    printf("                                           and the machine's depth search\n");
    printf("      util <eval|isover>                   eval - evaluates a given position and returns a bit-board with the new move along with the evaluation\n");
    printf("                                                 this command reads from standard input the following: <red_bitboard> <yellow_bitboard> <depth> <piece>\n");
    printf("                                           isover - evaluates a given position whether it's game over and determines the winner\n");
    printf("                                                 this command reads from standard input the following: <red_bitboard> <yellow_bitboard>\n");
    printf("      test                                 run all built-in tests\n");
}

extern void run_tests();
extern bool play(int argc, char* argv[]);
extern bool util(int argc, char* argv[]);

int main2(int argc, char* argv[])
{
    if (argc < 2)
    {
        print_usage();
        return 1;
    }
    if (strcmp(argv[1], "play") == 0)
    {
        if (!play(argc-2, &argv[2]))
        {
            print_usage();
            return 1;
        }
        return 0;
    }
    if (strcmp(argv[1], "util") == 0)
    {
        if (!util(argc-2, &argv[2]))
        {
            print_usage();
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
    
    print_usage();
    return 1;
}
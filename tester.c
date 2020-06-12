#include <stdio.h>
#include <stdlib.h>
#include "c4_utils.h"


void _assert(bool expression, char* file, uint32_t line)
{
    if(!expression)
    {
        printf("Assertion failed - File: %s - Line: %d", file, line);
        exit(1);
    }
}
#define assert(expression)      (_assert(!!(expression), __FILE__, __LINE__))


void test_rule_of_2()
{
    c4_bitboard b = {0};
    c4_bitboard* pb = &b;


}

void run_tests()
{
    test_rule_of_2();
}
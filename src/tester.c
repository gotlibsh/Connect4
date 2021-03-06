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


void test_bit_count()
{
    uint64_t i = 0x0;
    
    
    assert(BIT_COUNT(i) == 0);

    i = 0xFFFFFFFFFFFFFFFF;
    assert(BIT_COUNT(i) == 64);

    i = 0xFFFFFFFF00000000;
    assert(BIT_COUNT(i) == 32);
    
    i = 0x00000000AAAAAAAA;
    assert(BIT_COUNT(i) == 16);
    
    i = 0xFFFF0000FFFF0000;
    assert(BIT_COUNT(i) == 32);
    
    i = 0x0123456789ABCDEF;
    assert(BIT_COUNT(i) == 32);
}

void test_get_piece()
{
    c4_bitboard b = {0};
    c4_bitboard* pb = &b;


    /*
    X - - - - - -
    - X - - - - -
    - - X O - - -
    - - - X O - -
    - - - - - O -
    - - - - - - O
    */
    b.r_board = 0x0000020202020000;
    b.y_board = 0x0000000001010101;
    assert(get_piece(pb, 1, 1) == RED);
    assert(get_piece(pb, 2, 2) == RED);
    assert(get_piece(pb, 3, 3) == RED);
    assert(get_piece(pb, 4, 4) == RED);
    assert(get_piece(pb, 3, 4) == YELLOW);
    assert(get_piece(pb, 4, 5) == YELLOW);
    assert(get_piece(pb, 5, 6) == YELLOW);
    assert(get_piece(pb, 6, 7) == YELLOW);
    assert(get_piece(pb, 1, 2) == EMPTY);
}

void test_set_piece()
{
    c4_bitboard b = {0};
    c4_bitboard* pb = &b;


    /*
    X - - O - - -
    - - O - - - -
    - O - - - - X
    O - - - - X -
    - - - - X - -
    - - - X - - O
    */
    set_piece(pb, 1, 1, RED);
    set_piece(pb, 3, 7, RED);
    set_piece(pb, 4, 6, RED);
    set_piece(pb, 5, 5, RED);
    set_piece(pb, 6, 4, RED);
    set_piece(pb, 1, 4, YELLOW);
    set_piece(pb, 2, 3, YELLOW);
    set_piece(pb, 3, 2, YELLOW);
    set_piece(pb, 4, 1, YELLOW);
    set_piece(pb, 6, 7, YELLOW);
    assert(b.r_board == 0x0000020000208208);
    assert(b.y_board == 0x0000004104100001);
}

void test_drop_piece()
{
    c4_bitboard b = {0};
    c4_bitboard* pb = &b;


    /*
    - - - - - - -
    - - - O O - -
    - - - X X - -
    - - - O X - O
    - - - O X - O
    X X O X O - X
    */
    drop_piece(pb, 1, RED);
    drop_piece(pb, 2, RED);
    drop_piece(pb, 3, YELLOW);
    drop_piece(pb, 4, RED);
    drop_piece(pb, 4, YELLOW);
    drop_piece(pb, 4, YELLOW);
    drop_piece(pb, 4, RED);
    drop_piece(pb, 4, YELLOW);
    drop_piece(pb, 5, YELLOW);
    drop_piece(pb, 5, RED);
    drop_piece(pb, 5, RED);
    drop_piece(pb, 5, RED);
    drop_piece(pb, 5, YELLOW);
    drop_piece(pb, 7, RED);
    drop_piece(pb, 7, YELLOW);
    drop_piece(pb, 7, YELLOW);
    assert(b.r_board == 0x0000000001810269);
    assert(b.y_board == 0x00000000C0024494);
}

extern score_t calc_rule_of_2(c4_bitboard* board);
void test_rule_of_2()
{
    c4_bitboard b = {0};
    c4_bitboard* pb = &b;
    extern score_t rule_of_2_magic_score;


    /*
    - - - - - - -
    - - - - - - -
    - X - - - O -
    - X - - - O -
    - - - - - - -
    - - - - - - -
    */
    b.r_board = 0x0000000004080000;
    b.y_board = 0x0000000000408000;
    assert(calc_rule_of_2(pb) == 0);
    
    /*
    - - - - - - -
    - - - - - - -
    - - - X X - -
    - - - X X - -
    - - - - - - -
    - - - - - - -
    */
    b.r_board = 0x0000000001830000;
    b.y_board = 0x0000000000000000;
    assert(calc_rule_of_2(pb) == (18)*(rule_of_2_magic_score));

    /*
    X - - O - O O
    - - - - - - -
    - - X X - - -
    - - - X - - O
    - O - - X - -
    O - - - - - O
    */
    b.r_board = 0x0000020003020200;
    b.y_board = 0x0000005800005041;
    assert(calc_rule_of_2(pb) == (3+2)*(rule_of_2_magic_score) - (1+2+1)*(rule_of_2_magic_score));
}

extern score_t calc_rule_of_3(c4_bitboard* board);
void test_rule_of_3()
{
    c4_bitboard b = {0};
    c4_bitboard* pb = &b;
    extern score_t rule_of_3_magic_score;


    /*
    - - - - - - -
    - - - - - - -
    - - X - - - -
    - - - X - - -
    - - - - X - -
    - - - - - - -
    */
    b.r_board = 0x0000000002020200;
    b.y_board = 0x0000000000000000;
    assert(calc_rule_of_3(pb) == (2)*(rule_of_3_magic_score));

    /*
    - - - - - - -
    - - O - - - -
    - - X O - - -
    - - - X O - -
    - - - - X - -
    - - - - - - -
    */
    b.r_board = 0x0000000002020200;
    b.y_board = 0x0000000101010000;
    assert(calc_rule_of_3(pb) == 0);

    /*
    - - X - - - -
    - X - - - - -
    X - - - - - -
    - - - - - - O
    - - - - - O -
    - - - - O - -
    */
    b.r_board = 0x0000008208000000;
    b.y_board = 0x0000000000004104;
    assert(calc_rule_of_3(pb) == 0);

    /*
    O - - - - - O
    - O - - - - -
    - - - - - - O
    - - - O - O O
    - - - - O - -
    - - - - - - -
    */
    b.r_board = 0x0000000000000000;
    b.y_board = 0x0000020A0022C200;
    assert(calc_rule_of_3(pb) == (5)*(-rule_of_3_magic_score));

    /*
    - - O - O - -
    - X - X - X -
    - - X - O - -
    - - - X O O -
    - - - - X - -
    X X - - - - O
    */
    b.r_board = 0x00000002A2020260;
    b.y_board = 0x000000A000818001;
    assert(calc_rule_of_3(pb) == (2-1)*(rule_of_3_magic_score));
}

extern score_t calc_rule_of_center(c4_bitboard* board);
void test_rule_of_center()
{
    c4_bitboard b = {0};
    c4_bitboard* pb = &b;
    extern score_t rule_of_center_magic_score;


    /*
    - - - - - - -
    - - - - - - -
    - - - - - - -
    - - - - - - -
    - - - - - - -
    - - - X - - -
    */
    b.r_board = 0x0000000000000008;
    b.y_board = 0x0000000000000000;
    assert(calc_rule_of_center(pb) == (1)*(rule_of_center_magic_score));
    
    /*
    - - - - - - -
    - - - - - - -
    - - - X - - -
    - - - O - - -
    - - - O - - -
    - - - O - - -
    */
    b.r_board = 0x0000000001000000;
    b.y_board = 0x0000000000020408;
    assert(calc_rule_of_center(pb) == (1 - 3)*(rule_of_center_magic_score));
    
    /*
    - - - O - - -
    - - - X - - -
    - - - O - - -
    - - - X - - -
    - - - O - - -
    - - - X - - -
    */
    b.r_board = 0x0000000080020008;
    b.y_board = 0x0000004001000400;
    assert(calc_rule_of_center(pb) == 0);
}

extern score_t calc_winning_rule(c4_bitboard* board);
void test_winning_rule()
{
    c4_bitboard b = {0};
    c4_bitboard* pb = &b;
    extern score_t winning_rule_magic_score;


    assert(calc_winning_rule(pb) == 0);

    /*
    - - - - - - -
    - - - - - - -
    - - - - - - -
    - - - - - - -
    - - - - - - -
    X X X X - - -
    */
    b.r_board = 0x0000000000000078;
    b.y_board = 0x0000000000000000;
    assert(calc_winning_rule(pb) == (1)*(winning_rule_magic_score));
    
    /*
    - - - - - - -
    - - - - - O -
    - - - - O - -
    - - - O - - -
    - - O - - - -
    - - - - - - -
    */
    b.r_board = 0x0000000000000000;
    b.y_board = 0x0000000020820800;
    assert(calc_winning_rule(pb) == (-1)*(winning_rule_magic_score));
    
    /*
    - - - - - - -
    - - - - - - -
    - - - X - - -
    - - - - X - -
    - - - - - X -
    - - - - - - X
    */
    b.r_board = 0x0000000001010101;
    b.y_board = 0x0000000000000000;
    assert(calc_winning_rule(pb) == (1)*(winning_rule_magic_score));
    
    /*
    - - - - - - -
    - - - - - - -
    - - - - - - -
    - - - O O O O
    - - - - - - -
    - - - - - - -
    */
    b.r_board = 0x0000000000000000;
    b.y_board = 0x000000000003C000;
    assert(calc_winning_rule(pb) == (-1)*(winning_rule_magic_score));
}

extern bool is_game_over(c4_bitboard* board, player* p);
void test_is_game_over()
{
    c4_bitboard b = {0};
    c4_bitboard* pb = &b;
    player p, *pp = &p;

    /*
    - - - - - - -
    - - - - - - -
    - - - - - - -
    - - - - - - -
    - - - - - - -
    - - - - - - -
    */
    assert(is_game_over(pb, pp) == false);

    /*
    X X X X X X X
    X X X X X X X
    X X X X X X X
    X X X X X X X
    X X X X X X X
    X X X X X X X
    */
    b.r_board = 0xFFFFFFFFFFFFFFFF;
    b.y_board = 0x0000000000000000;
    assert(is_game_over(pb, pp) == true);

    /*
    X X X X X X X
    X X X X X X X
    X X X X X X X
    O O O O O O O
    O O O O O O O
    O O O O O O O
    */
    b.r_board = 0x000003FFFFE00000;
    b.y_board = 0x00000000001FFFFF;
    assert(is_game_over(pb, pp) == true);

    /*
    - - - - - - -
    - - - - - - -
    - - - - - - -
    - - - - - - -
    - - - - - - -
    X X X X - - -
    */
    b.r_board = 0x0000000000000078;
    b.y_board = 0x0000000000000000;
    assert(is_game_over(pb, pp) == true);

    /*
    - - - - - - -
    - - - - O - -
    - - - - O - -
    - - - - O - -
    - - - - O - -
    - - - - - - -
    */
    b.r_board = 0x0000000000000000;
    b.y_board = 0x0000000040810200;
    assert(is_game_over(pb, pp) == true);

    /*
    - - - - - - -
    - - - - - - -
    - - X - - - -
    - - - X - - -
    - - - - X - -
    - - - - - X -
    */
    b.r_board = 0x0000000002020202;
    b.y_board = 0x0000000000000000;
    assert(is_game_over(pb, pp) == true);
    
    /*
    - - - - - - -
    - - - - - - -
    - - - - - - O
    - - - - - O -
    - - - - O - -
    - - - O - - -
    */
    b.r_board = 0x0000000000000000;
    b.y_board = 0x0000000000208208;
    assert(is_game_over(pb, pp) == true);
    
    /*
    O O O - - - X
    X X X - O - -
    - - - O X - -
    - - O X - - -
    - - X - X X X
    O - - - O O O
    */
    b.r_board = 0x0000000F00820B80;
    b.y_board = 0x0000038041040047;
    assert(is_game_over(pb, pp) == false);
}

extern void get_child_boards(c4_bitboard* board, c4_bitboard* children, piece p, uint8_t* child_count);
void test_get_child_boards()
{
#define assert_boards_eq(brd, r, y)     (assert(((brd).r_board == r) && ((brd).y_board == y)))
    c4_bitboard b = {0};
    c4_bitboard* pb = &b;
    c4_bitboard children[BOARD_WIDTH];
    uint8_t child_count = 0;


    /*
    - - - - - - -
    - - - - - - -
    - - - - - - -
    - - - - - - -
    - - - - - - -
    - - - - - - -
    */
    get_child_boards(pb, children, RED, &child_count);
    assert(child_count == BOARD_WIDTH);
    assert_boards_eq(children[0], 0x0000000000000040, 0x0000000000000000);
    assert_boards_eq(children[1], 0x0000000000000020, 0x0000000000000000);
    assert_boards_eq(children[2], 0x0000000000000010, 0x0000000000000000);
    assert_boards_eq(children[3], 0x0000000000000008, 0x0000000000000000);
    assert_boards_eq(children[4], 0x0000000000000004, 0x0000000000000000);
    assert_boards_eq(children[5], 0x0000000000000002, 0x0000000000000000);
    assert_boards_eq(children[6], 0x0000000000000001, 0x0000000000000000);

    /*
    - - - - - X O
    - - - O X O X
    - - - O X O X
    - - X O X O X
    - X O X O X O
    X O X O X O X
    */
    b.r_board = 0x0000001050A55555;
    b.y_board = 0x00000008A1428AAA;
    get_child_boards(pb, children, YELLOW, &child_count);
    assert(child_count == 5);
    assert_boards_eq(children[0], 0x0000001050A55555, 0x00000008A142AAAA);
    assert_boards_eq(children[1], 0x0000001050A55555, 0x00000008A14A8AAA);
    assert_boards_eq(children[2], 0x0000001050A55555, 0x00000008A3428AAA);
    assert_boards_eq(children[3], 0x0000001050A55555, 0x00000048A1428AAA);
    assert_boards_eq(children[4], 0x0000001050A55555, 0x00000028A1428AAA);

    /*
    - - - - - - -
    - - - - - - -
    - - - - - - -
    - - - - - - -
    - - - - - - -
    X X X X O O O
    */
    b.r_board = 0x0000000000000078;
    b.y_board = 0x0000000000000007;
    get_child_boards(pb, children, RED, &child_count);
    assert(child_count == 0);
}

void run_tests()
{
    test_bit_count();
    test_get_piece();
    test_set_piece();
    test_drop_piece();
    test_rule_of_2();
    test_rule_of_3();
    test_rule_of_center();
    test_winning_rule();
    test_is_game_over();
    test_get_child_boards();
}
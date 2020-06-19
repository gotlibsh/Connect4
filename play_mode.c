#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "c4_utils.h"
#include "minimax.h"


void make_human_move(c4_bitboard* board, piece p)
{
    int64_t human_move = -1;


    printf("enter column index (1-7): ");
    scanf("%lld", &human_move);

    while(!is_legal_move(board, human_move))
    {
        printf("illegal move!\n");
        printf("enter column index (1-7): ");
        scanf("%lld", &human_move);
    }
    
    add_piece(board, (uint8_t)human_move, p);
}

void make_machine_move(c4_bitboard* board, piece p, uint8_t depth)
{
    node_result machine_move = {0};
    c4_bitboard children[BOARD_WIDTH];
    uint8_t child_count = 0;
    bool is_maximizer = (p == RED);


    assert(p != EMPTY);
    machine_move = minimax(board, depth, p, MIN_SCORE, MAX_SCORE, is_maximizer);
    printf("machine-eval: %d\n", machine_move.eval);

    get_child_boards(board, children, p, &child_count);
    assert(child_count > 0);

    *board = children[machine_move.child_index];
}

bool parse_play_args(int argc, char* argv[], bool* machine_start, uint8_t* machine_depth)
{
    *machine_start = false;
    *machine_depth = 7;

    if (argc > 0)
    {
        if (strcmp(argv[0], "human") == 0)
        {
            *machine_start = false;
        }
        else if (strcmp(argv[0], "machine") == 0)
        {
            *machine_start = true;
        }
        else
        {
            return false;
        }
        argc--;
    }
    if (argc > 0)
    {
        *machine_depth = atoi(argv[1]);
    }

    return true;
}

bool play(int argc, char* argv[])
{
    c4_bitboard board = {0};
    piece p = RED;
    player winner = EMPTY;
    bool machine_to_play = false;
    uint8_t depth = 10;


    if (!parse_play_args(argc, argv, &machine_to_play, &depth))
    {
        return false;
    }

    while(!is_game_over(&board, &winner))
    {
        if (machine_to_play)
        {
            make_machine_move(&board, p, depth);
        }
        else
        {
            make_human_move(&board, p);
        }

        print_board(&board);
        printf("\n");

        p = OPP_PIECE(p);
        machine_to_play = !machine_to_play;
    }

    printf("Game Over!\n");

    if (winner != EMPTY)
        printf("Player %s won!\n", STR_PIECE(winner));
    else
        printf("Draw!");

    return true;
}

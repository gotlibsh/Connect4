#include <stdio.h>
#include <assert.h>
#include "c4_utils.h"
#include "evaluation.h"


typedef struct _node_result
{
    uint8_t child_index;
    score_t eval;
} node_result;

void make_machine_move(c4_bitboard* board, uint8_t child_index, piece p)
{
    c4_bitboard children[BOARD_WIDTH];
    uint8_t child_count = 0;


    get_child_boards(board, children, p, &child_count);
    assert(child_count > 0);

    *board = children[child_index];
}

node_result minimax(c4_bitboard* board, uint8_t depth, piece p, bool is_maximizer)
{
    node_result result = {0};
    c4_bitboard children[BOARD_WIDTH];
    uint8_t child_count = 0;
    score_t peak_eval = is_maximizer ? MIN_SCORE : MAX_SCORE;
    uint8_t index_of_peak = -1;


    if (depth == 0)
    {
        result.child_index = -1;
        result.eval = static_evaluation(board);
        return result;
    }

    get_child_boards(board, children, p, &child_count);

    if (child_count == 0)
    {
        result.child_index = -1;
        result.eval = static_evaluation(board);
        return result;
    }

    for (int i = 0; i < child_count; i++)
    {
        result = minimax(&children[i], depth - 1, OPP_PIECE(p), !is_maximizer);

        if (is_maximizer)
        {
            if (result.eval > peak_eval)
            {
                peak_eval = result.eval;
                index_of_peak = i;
            }
        }
        else
        {
            if (result.eval < peak_eval)
            {
                peak_eval = result.eval;
                index_of_peak = i;
            }
        }
    }

    result.eval = peak_eval;
    result.child_index = index_of_peak;

    return result;
}

void play()
{
    c4_bitboard board = {0};
    piece p = RED;
    player winner = EMPTY;
    bool machine_to_play = false;
    node_result machine_move = {0};
    uint8_t human_move = -1;
    uint8_t depth = 8;


    while(!is_game_over(&board, &winner))
    {
        if (machine_to_play)
        {
            machine_move = minimax(&board, depth, p, false);
            make_machine_move(&board, machine_move.child_index, p);
            printf("machine-eval: %d\n", machine_move.eval);
        }
        else
        {
            printf("enter column index (1-7): ");
            scanf("%hhd", &human_move);
            add_piece(&board, human_move, p);
        }

        print_board(&board);
        printf("\n");

        p = OPP_PIECE(p);
        machine_to_play = !machine_to_play;
    }

    printf("Game Over!");
}
#include <stdio.h>
#include <assert.h>
#include "c4_utils.h"
#include "evaluation.h"


#define MAX(a,b)                    ((a) > (b) ? a : b)
#define MIN(a,b)                    ((a) < (b) ? a : b)

typedef struct _node_result
{
    uint8_t child_index;
    score_t eval;
} node_result;


node_result minimax(c4_bitboard* board, uint8_t depth, piece p, score_t alpha, score_t beta, bool is_maximizer)
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
        result = minimax(&children[i], depth - 1, OPP_PIECE(p), alpha, beta, !is_maximizer);

        if (is_maximizer)
        {
            if (result.eval > peak_eval)
            {
                peak_eval = result.eval;
                index_of_peak = i;
            }

            alpha = MAX(alpha, peak_eval);

            if (alpha >= beta)
            {
                break;
            }
        }
        else
        {
            if (result.eval < peak_eval)
            {
                peak_eval = result.eval;
                index_of_peak = i;
            }

            beta = MIN(beta, peak_eval);

            if (beta <= alpha)
            {
                break;
            }
        }
    }

    result.eval = peak_eval;
    result.child_index = index_of_peak;

    return result;
}

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

void play()
{
    c4_bitboard board = {0};
    piece p = RED;
    player winner = EMPTY;
    bool machine_to_play = false;
    uint8_t depth = 10;


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
}

void util(char* cmd)
{
    c4_bitboard board = {0};
    c4_bitboard children[BOARD_WIDTH];
    uint8_t depth = 0;
    piece p = EMPTY;
    uint8_t child_count = 0;
    node_result result = {0};
    player winner;
    bool over;


    scanf("%llx", &board.r_board);
    scanf("%llx", &board.y_board);

    if (strcmp(cmd, "over") == 0)
    {
        over = is_game_over(&board, &winner);
        printf("%d %d\n", over, winner);
    }
    else if(strcmp(cmd, "eval") == 0)
    {
        scanf("%hhd", &depth);  // get required depth
        scanf("%d", &p);        // get piece to put next

        result = minimax(&board, depth, p, MIN_SCORE, MAX_SCORE, (p == RED));
        get_child_boards(&board, children, p, &child_count);
        printf("0x%016llx 0x%016llx %d\n", children[result.child_index].r_board, children[result.child_index].y_board, result.eval);
    }
}
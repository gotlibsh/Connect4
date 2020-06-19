#include "minimax.h"
#include "evaluation.h"


#define MAX(a,b)                    ((a) > (b) ? a : b)
#define MIN(a,b)                    ((a) < (b) ? a : b)


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
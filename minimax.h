#ifndef _MINIMAX_H_
#define _MINIMAX_H_

#include "c4_utils.h"

typedef struct _node_result
{
    int8_t child_index;
    score_t eval;
} node_result;

node_result minimax(c4_bitboard* board, uint8_t depth, piece p, score_t alpha, score_t beta, bool is_maximizer);

#endif
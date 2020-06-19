#include "c4_utils.h"
#include "evaluation.h"


score_t calc_rule_of_x(c4_bitboard* board, uint8_t x, uint16_t magic_score)
{
    score_t score = 0;
    uint64_t r_seq, y_seq;
    bool is_r_seq, is_y_seq;


    // loop through the board horizontal sequences
    for (int row = 1; row <= BOARD_HEIGHT; row++)
    {
        for (int col = 1; col <= BOARD_WIDTH - (SEQ_LEN-1); col++)
        {
            r_seq = GET_HRZ_SEQ(board->r_board, row, col);  // get horizontal sequence of four squares of red board
            y_seq = GET_HRZ_SEQ(board->y_board, row, col);  // get horizontal sequence of four squares of yellow board

            is_r_seq = IS_SEQ_ONE_TYPE(r_seq, y_seq);       // check if sequence consists solely of red squares and/or empty squares
            is_y_seq = IS_SEQ_ONE_TYPE(y_seq, r_seq);       // check if sequence consists solely of yellow squares and/or empty squares

            if (!(is_r_seq || is_y_seq))
                continue;

            if (is_r_seq && BIT_COUNT(r_seq) == x)
            {
                score += magic_score;
            }

            if (is_y_seq && BIT_COUNT(y_seq) == x)
            {
                score -= magic_score;
            }
        }
    }

    // loop through the board looking for vertical sequences
    for (int row = 1; row <= BOARD_HEIGHT - (SEQ_LEN-1); row++)
    {
        for (int col = 1; col <= BOARD_WIDTH; col++)
        {
            r_seq = GET_VRT_SEQ(board->r_board, row, col);
            y_seq = GET_VRT_SEQ(board->y_board, row, col);

            is_r_seq = IS_SEQ_ONE_TYPE(r_seq, y_seq);
            is_y_seq = IS_SEQ_ONE_TYPE(y_seq, r_seq);

            if (!(is_r_seq || is_y_seq))
                continue;

            // repeating code begin !!!!
            if (is_r_seq && BIT_COUNT(r_seq) == x)
            {
                score += magic_score;
            }

            if (is_y_seq && BIT_COUNT(y_seq) == x)
            {
                score -= magic_score;
            }
            // repeating code end !!!!
        }
    }

    // loops through the board looking for diagonal sequences
    for (int row = 1; row <= BOARD_HEIGHT - (SEQ_LEN-1); row++)
    {
        for (int col = 1; col <= BOARD_WIDTH - (SEQ_LEN-1); col++)
        {
            r_seq = GET_DGNL_SEQ(board->r_board, row, col);
            y_seq = GET_DGNL_SEQ(board->y_board, row, col);

            is_r_seq = IS_SEQ_ONE_TYPE(r_seq, y_seq);
            is_y_seq = IS_SEQ_ONE_TYPE(y_seq, r_seq);

            // printf("r-brd: 0x%X\tr-seq: 0x%X\t r,c: %d,%d\n", board->r_board, r_seq, row, col);
            // printf("y-brd: 0x%X\ty-seq: 0x%X\t r,c: %d,%d\n", board->y_board, y_seq, row, col);

            if (!(is_r_seq || is_y_seq))
                continue;

            // repeating code begin !!!!
            if (is_r_seq && BIT_COUNT(r_seq) == x)
            {
                score += magic_score;
            }

            if (is_y_seq && BIT_COUNT(y_seq) == x)
            {
                score -= magic_score;
            }
            // repeating code end !!!!
        }
    }

    // loops through the board looking for reverse diagonal sequences
    for (int row = 1; row <= BOARD_HEIGHT - (SEQ_LEN-1); row++)
    {
        for (int col = SEQ_LEN; col <= BOARD_WIDTH; col++)  // since we go for reverse diagonal, we can skip columns that are insufficient
        {
            r_seq = GET_REV_DGNL_SEQ(board->r_board, row, col);
            y_seq = GET_REV_DGNL_SEQ(board->y_board, row, col);

            is_r_seq = IS_SEQ_ONE_TYPE(r_seq, y_seq);
            is_y_seq = IS_SEQ_ONE_TYPE(y_seq, r_seq);

            // printf("r-brd: 0x%X\tr-seq: 0x%X\t r,c: %d,%d\n", board->r_board, r_seq, row, col);
            // printf("y-brd: 0x%X\ty-seq: 0x%X\t r,c: %d,%d\n", board->y_board, y_seq, row, col);

            if (!(is_r_seq || is_y_seq))
                continue;

            // repeating code begin !!!!
            if (is_r_seq && BIT_COUNT(r_seq) == x)
            {
                score += magic_score;
            }

            if (is_y_seq && BIT_COUNT(y_seq) == x)
            {
                score -= magic_score;
            }
            // repeating code end !!!!
        }
    }

    return score;
}

int8_t rule_of_2_magic_score = 3;
score_t calc_rule_of_2(c4_bitboard* board)
{
    return calc_rule_of_x(board, 2, rule_of_2_magic_score);
}

int8_t rule_of_3_magic_score = 5;
score_t calc_rule_of_3(c4_bitboard* board)
{
    return calc_rule_of_x(board, 3, rule_of_3_magic_score);
}

int8_t rule_of_center_magic_score = 2;
score_t calc_rule_of_center(c4_bitboard* board)
{
    uint64_t r_center = GET_COL(board->r_board, 4);
    uint64_t y_center = GET_COL(board->y_board, 4);

    return (BIT_COUNT(r_center) - BIT_COUNT(y_center))*(rule_of_center_magic_score);
}

int16_t winning_rule_magic_score = 1000;
score_t calc_winning_rule(c4_bitboard* board)
{
    piece winner;
    

    is_game_over(board, &winner);

    if (winner == RED)
        return winning_rule_magic_score;
    if (winner == YELLOW)
        return -winning_rule_magic_score;

    return 0;
}

score_t static_evaluation(c4_bitboard* board)
{
    return  calc_rule_of_center(board)  +
            calc_rule_of_2(board)       +
            calc_rule_of_3(board)       +
            calc_winning_rule(board);
}
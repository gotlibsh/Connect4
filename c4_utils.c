#include "c4_utils.h"


int8_t rule_of_2_magic_score = 2;
int16_t calc_rule_of_2(c4_bitboard* board)
{
    int16_t score = 0;
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

            if (is_r_seq && BIT_COUNT(r_seq) == 2)
            {
                score += rule_of_2_magic_score;
            }

            if (is_y_seq && BIT_COUNT(y_seq) == 2)
            {
                score -= rule_of_2_magic_score;
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
            if (is_r_seq && BIT_COUNT(r_seq) == 2)
            {
                score += rule_of_2_magic_score;
            }

            if (is_y_seq && BIT_COUNT(y_seq) == 2)
            {
                score -= rule_of_2_magic_score;
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
            if (is_r_seq && BIT_COUNT(r_seq) == 2)
            {
                score += rule_of_2_magic_score;
            }

            if (is_y_seq && BIT_COUNT(y_seq) == 2)
            {
                score -= rule_of_2_magic_score;
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
            if (is_r_seq && BIT_COUNT(r_seq) == 2)
            {
                score += rule_of_2_magic_score;
            }

            if (is_y_seq && BIT_COUNT(y_seq) == 2)
            {
                score -= rule_of_2_magic_score;
            }
            // repeating code end !!!!
        }
    }

    return score;
}
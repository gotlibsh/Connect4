#include <stdint.h>


#ifdef _MSC_VER
 #include <intrin.h>
 #ifdef _WIN64
  #define BIT_COUNT(x)              (__popcnt64(x))
 #else
  #define BIT_COUNT(x)              (__popcnt((x)>>32) + __popcnt((x)))
 #endif
#elif __GNUC__
 #define BIT_COUNT(x)               (__builtin_popcountll(x))
#else
 #define BIT_COUNT(x)
#endif

typedef uint8_t                     bool;
#define true                        (1)
#define false                       (0)

#define BOARD_WIDTH                 (7)
#define BOARD_HEIGHT                (6)
#define BOARD_SIZE                  (BOARD_WIDTH * BOARD_HEIGHT)
#define BOARD_MASK                  (0x000003FFFFFFFFFF)    // lower 42 bits
#define IS_BOARD_FULL(y, r)         (((y) | (r)) == BOARD_MASK)
#define SEQ_LEN                     (4)
#define HRZ_SEQ_MASK                (0x000000000000000F)
#define VRT_SEQ_MASK                (0x0000000000204081)
#define DGNL_SEQ_MASK               (0x0000000001010101)
#define REV_DGNL_SEQ_MASK           (0x0000000000208208)
#define IS_SEQ_ONE_TYPE(s1, s2)     (((s1) ^ (s2)) == (s1))
#define GET_HRZ_SEQ(brd, r, c)      (((brd) >> (38 - ((r-1) * BOARD_WIDTH + (c-1)))) & HRZ_SEQ_MASK)        // horizontal sequence
#define GET_VRT_SEQ(brd, r, c)      (((brd) >> (20 - ((r-1) * BOARD_WIDTH) - (c-1))) & VRT_SEQ_MASK)        // vertical sequence
                                                                                                            // formula of 20:
                                                                                                            // (board_width - 1)                        "number of shift right per row"
                                                                                                            // * (board_height - (seq_len - 1))         "number of rows that can start a vertical sequence of 4"
                                                                                                            // + ((board_height - (seq_len - 1)) - 1)   "number of times jumping to next row"
#define GET_DGNL_SEQ(brd, r, c)     (((brd) >> (17 - ((r-1) * BOARD_WIDTH) - (c-1))) & DGNL_SEQ_MASK)       // diagonal sequence
                                                                                                            // formula of 17:
                                                                                                            // (board_width - 4)                        "number of shift right for diagonals starting the same row"
                                                                                                            // * (board_height - (seq_len - 1))         "number of rows that can start a diagonal sequence of 4"
                                                                                                            // + ((board_height - (seq_len - 1)) - 1)   "number of times jumping to next row"
                                                                                                            // * (seq_len - 1)                          "number of squares in each row that can't start a diagonal"
                                                                                                            // + ((board_height - (seq_len - 1)) - 1)   "number of times jumping to next row"
#define GET_REV_DGNL_SEQ(brd, r, c) (((brd) >> (17 - ((r-1) * BOARD_WIDTH) - (c-4))) & REV_DGNL_SEQ_MASK)   // reverse diagonal sequence
                                                                                                            // formula of 17:
                                                                                                            // (board_width - 4)                        "number of shift right for diagonals starting the same row"
                                                                                                            // * (board_height - (seq_len - 1))         "number of rows that can start a diagonal sequence of 4"
                                                                                                            // + ((board_height - (seq_len - 1)) - 1)   "number of times jumping to next row"
                                                                                                            // * (seq_len - 1)                          "number of squares in each row that can't start a diagonal"
                                                                                                            // + ((board_height - (seq_len - 1)) - 1)   "number of times jumping to next row"

/*
    Represents a Connect 4 board position.

    y_board is the locations of the Yellow pieces on the board.
    r_board is the locations of the Red pieces on the board.

    Only lower 42 (6*7) bits of each sub-board are used, upper 22 bits are reserved.
*/
typedef struct _c4_bitboard
{
    uint64_t y_board;
    uint64_t r_board;
} c4_bitboard;


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
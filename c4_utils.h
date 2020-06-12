#ifndef _C4_UTILS_H_
#define _C4_UTILS_H_

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
#define CLEAN_BOARD(brd)            { brd.r_board = 0x0; brd.y_board = 0x0; }
#define GET_BIT(brd, r, c)          ((brd) & (1ULL << (BOARD_SIZE - (((row-1) * BOARD_WIDTH) + col))))
#define SET_BIT(brd, r, c)          ((brd) |= (1ULL << (BOARD_SIZE - (((row-1) * BOARD_WIDTH) + col))))
#define IS_BOARD_FULL(r, y)         (((r) | (y)) == BOARD_MASK)

#define STR_PIECE(p)                (((p) == RED) ? "X" : ((p) == YELLOW) ? "O" : "-")

#define SEQ_LEN                     (4)
#define COL_MASK                    (0x0000020408102040)
#define GET_COL(brd, c)             ((brd) & (COL_MASK >> (c-1)))
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

typedef enum _piece
{
    RED     = 0,
    YELLOW  = 1,
    EMPTY   = 2
} piece;

typedef piece   player;

void  set_piece(c4_bitboard* board, uint8_t row, uint8_t col, piece p);
piece get_piece(c4_bitboard* board, uint8_t row, uint8_t col);
void add_piece(c4_bitboard* board, uint8_t index, piece p);
bool is_game_over(c4_bitboard* board, player* p);

void print_board(c4_bitboard* board);

#endif
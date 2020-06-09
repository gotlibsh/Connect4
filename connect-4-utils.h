#include <stdint.h>


#define BOARD_WIDTH             (7)
#define BOARD_HEIGHT            (6)
#define BOARD_SIZE              (BOARD_WIDTH * BOARD_HEIGHT)
#define BOARD_MASK              (0x000003FFFFFFFFFF)    // lower 42 bits


/*
    Represents a Connect 4 board position.

    y_board is the locations of the Yeloow pieces on the board.
    r_board is the locations of the Red pieces on the board.

    Only lower 42 (6*7) bits of each sub-board are used, upper 22 bits are reserved.
*/
typedef struct _c4_bitboard
{
    uint64_t y_board;
    uint64_t r_board;
} c4_bitboard;
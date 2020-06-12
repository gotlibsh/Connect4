#include "c4_utils.h"


void set_piece(c4_bitboard* board, uint8_t row, uint8_t col, piece p)
{
    if (p == RED)
        SET_BIT(board->r_board, row, col);
    else if (p == YELLOW)
        SET_BIT(board->y_board, row, col);
}

piece get_piece(c4_bitboard* board, uint8_t row, uint8_t col)
{
    if (GET_BIT(board->r_board, row, col))  return RED;
    if (GET_BIT(board->y_board, row, col))  return YELLOW;

    return EMPTY;
}

void print_board(c4_bitboard* board)
{
    uint8_t row, col;

    for(row = 1; row <= BOARD_HEIGHT; row++)
    {
        for(col = 1; col <= BOARD_WIDTH; col++)
        {
            printf("%s ", STR_PIECE(get_piece(board, row, col)));
        }
        printf("\n");
    }
}
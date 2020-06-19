#include <assert.h>
#include <stdio.h>
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

void add_piece(c4_bitboard* board, uint8_t col, piece p)
{
    uint64_t full_col;
    int8_t row;


    full_col = GET_COL(board->r_board, col) | GET_COL(board->y_board, col);
    row = BOARD_HEIGHT - (uint8_t)BIT_COUNT(full_col);

    assert(row >= 1);

    set_piece(board, row, col, p);
}

bool is_game_over(c4_bitboard* board, player* p)
{
    *p = EMPTY;

    if (IS_BOARD_FULL(board->r_board, board->y_board))
    {
        return true;
    }

    // looking for winning sequences horizontally
    for (int row = 1; row <= BOARD_HEIGHT; row++)
    {
        for (int col = 1; col <= BOARD_WIDTH - (SEQ_LEN-1); col++)
        {
            if ((GET_HRZ_SEQ(board->r_board, row, col) & HRZ_SEQ_MASK) == HRZ_SEQ_MASK)
            {
                *p = RED;
                return true;
            }
            if ((GET_HRZ_SEQ(board->y_board, row, col) & HRZ_SEQ_MASK) == HRZ_SEQ_MASK)
            {
                *p = YELLOW;
                return true;
            }
        }
    }

    // looking for winning sequences vertically
    for (int row = 1; row <= BOARD_HEIGHT - (SEQ_LEN-1); row++)
    {
        for (int col = 1; col <= BOARD_WIDTH; col++)
        {
            if ((GET_VRT_SEQ(board->r_board, row, col) & VRT_SEQ_MASK) == VRT_SEQ_MASK)
            {
                *p = RED;
                return true;
            }
            if ((GET_VRT_SEQ(board->y_board, row, col) & VRT_SEQ_MASK) == VRT_SEQ_MASK)
            {
                *p = YELLOW;
                return true;
            }
        }
    }

    // looking for winning sequences diagonally
    for (int row = 1; row <= BOARD_HEIGHT - (SEQ_LEN-1); row++)
    {
        for (int col = 1; col <= BOARD_WIDTH - (SEQ_LEN-1); col++)
        {
            if ((GET_DGNL_SEQ(board->r_board, row, col) & DGNL_SEQ_MASK) == DGNL_SEQ_MASK)
            {
                *p = RED;
                return true;
            }
            if ((GET_DGNL_SEQ(board->y_board, row, col) & DGNL_SEQ_MASK) == DGNL_SEQ_MASK)
            {
                *p = YELLOW;
                return true;
            }
        }
    }

    // looking for winning sequences reverse-diagonally
    for (int row = 1; row <= BOARD_HEIGHT - (SEQ_LEN-1); row++)
    {
        for (int col = SEQ_LEN; col <= BOARD_WIDTH; col++)
        {
            if ((GET_REV_DGNL_SEQ(board->r_board, row, col) & REV_DGNL_SEQ_MASK) == REV_DGNL_SEQ_MASK)
            {
                *p = RED;
                return true;
            }
            if ((GET_REV_DGNL_SEQ(board->y_board, row, col) & REV_DGNL_SEQ_MASK) == REV_DGNL_SEQ_MASK)
            {
                *p = YELLOW;
                return true;
            }
        }
    }

    return false;
}

bool is_legal_move(c4_bitboard* board, int64_t col)
{
    uint64_t full_col;


    if (col < 1 || col > BOARD_WIDTH)
    {
        return false;
    }

    full_col = GET_COL(board->r_board, col) | GET_COL(board->y_board, col);
    return (BIT_COUNT(full_col) < BOARD_HEIGHT);
}

void get_child_boards(c4_bitboard* board, c4_bitboard* children, piece p, uint8_t* child_count)
{
    int child_index = 0;
    uint64_t full_col;
    player dummy;


    *child_count = 0;
    if (is_game_over(board, &dummy))
    {
        return;
    }

    for (int col = 1; col <= BOARD_WIDTH; col++)
    {
        full_col = GET_COL(board->r_board, col) | GET_COL(board->y_board, col);
        
        if (BIT_COUNT(full_col) < BOARD_HEIGHT)    // still room for a piece in this column
        {
            children[child_index] = *board;
            add_piece(&children[child_index], col, p);
            child_index++;
        }
    }

    *child_count = child_index;
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
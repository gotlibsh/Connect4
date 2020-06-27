#include <stdio.h>
#include "c4_utils.h"
#include "minimax.h"


typedef enum _action
{
    EVAL = 0,
    IS_GAME_OVER
} action;


bool parse_util_args(int argc, char* argv[], action* action)
{
    *action = EVAL;

    if (strcmp(argv[0], "eval") == 0)
    {
        *action = EVAL;
    }
    else if(strcmp(argv[0], "isover") == 0)
    {
        *action = IS_GAME_OVER;
    }
    else
    {
        return false;
    }
    
    return true;
}

bool util(int argc, char* argv[])
{
    c4_bitboard board = {0};
    c4_bitboard children[BOARD_WIDTH];
    action action;
    uint8_t depth = 0;
    piece p = EMPTY;
    uint8_t child_count = 0;
    node_result result = {0};
    player winner;
    bool over;


    if (!parse_util_args(argc, argv, &action))
    {
        return false;
    }
    
    switch (action)
    {
    case EVAL:
        {
            scanf("%llx", &board.r_board);
            scanf("%llx", &board.y_board);
            scanf("%hhd", &depth);  // get required depth
            scanf("%d", &p);        // get piece type to put next

            result = minimax(&board, depth, p, MIN_SCORE, MAX_SCORE, (p == RED));
            
            if (result.child_index < 0) // terminal node
            {
                printf("0x0 0x0 0\n");
                return true;
            }
            
            get_child_boards(&board, children, p, &child_count);

            printf("0x%016llx 0x%016llx %d\n",
                    children[result.child_index].r_board,
                    children[result.child_index].y_board,
                    result.eval);
        }
        break;
    case IS_GAME_OVER:
        {
            scanf("%llx", &board.r_board);
            scanf("%llx", &board.y_board);

            over = is_game_over(&board, &winner);

            printf("%d %d\n", over, winner);
        }
        break;
    default:
        return false;
    }

    return true;
}
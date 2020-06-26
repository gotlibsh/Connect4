from subprocess import Popen, PIPE


def is_game_over(red, yellow, engine_path):
    p = Popen([engine_path, 'util', 'isover'], stdin=PIPE, stdout=PIPE)
    
    params = bytearray(' '.join([hex(red), hex(yellow)]), 'utf-8')
    is_game_over, winner = p.communicate(params)[0].split()

    return (int(is_game_over), int(winner))


def get_machine_move(red_board, yellow_board, depth, turn, engine_path):
    p = Popen([engine_path, 'util', 'eval'], stdin=PIPE, stdout=PIPE)
    
    params = bytearray(' '.join([hex(red_board), hex(yellow_board), str(depth), str(turn)]), 'utf-8')
    new_red, new_yellow, evaluation = p.communicate(params)[0].split()

    return (int(new_red, 16), int(new_yellow, 16), int(evaluation))


def prettyprint_board(red, yellow):
    first_piece_loc = 42-1

    for i in range(42):
        if red & (1 << (first_piece_loc - i)):
            print('X', end=' ')
        elif yellow & (1 << (first_piece_loc - i)):
            print('O', end=' ')
        else:
            print('-', end=' ')

        if (i+1) % 7 == 0:
            print()

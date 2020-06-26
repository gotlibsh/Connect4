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

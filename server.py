import argparse
from subprocess import Popen, PIPE
from flask import Flask, request, jsonify
app = Flask(__name__)

CONSOLE_ARGS = None


def is_game_over(red, yellow):
    p = Popen([CONSOLE_ARGS.engine_path, 'util', 'isover'], stdin=PIPE, stdout=PIPE)
    
    params = bytearray(' '.join([hex(red), hex(yellow)]), 'utf-8')
    is_game_over, winner = p.communicate(params)[0].split()

    return (int(is_game_over), int(winner))


def get_machine_move(red_board, yellow_board, depth, turn):
    p = Popen([CONSOLE_ARGS.engine_path, 'util', 'eval'], stdin=PIPE, stdout=PIPE)
    
    params = bytearray(' '.join([hex(red_board), hex(yellow_board), str(depth), str(turn)]), 'utf-8')
    new_red, new_yellow, evaluation = p.communicate(params)[0].split()

    return (int(new_red, 16), int(new_yellow, 16), int(evaluation))


def convert_color_to_number(turn):
    return 0 if turn == 'red' else 1 if turn == 'yellow' else None


def convert_number_to_color(winner):
    return 'red' if winner == 0 else 'yellow' if winner == 1 else None


def convert_2d_board_to_bitboard(two_d_board):
    r, y = 0, 0
    index = 0

    for row in two_d_board:
        for col in row:
            if col == 'red':
                r |= (1 << (41-index))
            if col == 'yellow':
                y |= (1 << (41-index))
            index += 1

    return r,y


def get_new_column(old_red, old_yellow, new_red, new_yellow):
    old = old_red | old_yellow
    new = new_red | new_yellow

    new_bit = old ^ new
    column_mask = 0x0000000810204081
    column = 7

    while not (column_mask & new_bit) != 0:
        new_bit >>= 1
        column -= 1

    return column


@app.route('/evaluate', methods = ['POST', 'GET'])
def evaluate():
    client_req = request.get_json()
    
    red, yellow = convert_2d_board_to_bitboard(client_req['board'])
    depth = client_req['depth']
    turn = convert_color_to_number(client_req['turn'])

    # check if given board is already game-over
    game_over, winner = is_game_over(red, yellow)

    # if game-over, return immediately
    if game_over:
        return jsonify(column=None, is_game_over=True, winner=convert_number_to_color(winner), eval=None)

    # evaluate the given board position and get a new move
    new_red, new_yellow, evaluation = get_machine_move(red_board=red, yellow_board=yellow, depth=depth, turn=turn)
    column = get_new_column(red, yellow, new_red, new_yellow)

    # check if new board is game-over
    game_over, winner = is_game_over(new_red, new_yellow)

    return jsonify(column=column, is_game_over=game_over, winner=convert_number_to_color(winner), eval=evaluation)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('engine_path')
    
    CONSOLE_ARGS = parser.parse_args()

    app.run()
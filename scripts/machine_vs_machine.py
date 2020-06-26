import argparse
from subprocess import Popen, PIPE
from wrapper import *

CONSOLE_ARGS = None

def run_game():
    turn_to_player = lambda turn: 'red' if turn == 0 else 'yellow' if turn == 1 else 'n/a'
    turn = 0
    depth = [CONSOLE_ARGS.red, CONSOLE_ARGS.yellow]
    red, yellow = 0, 0

    game_over, winner = is_game_over(red, yellow, CONSOLE_ARGS.engine_path)
    while not game_over:
        red, yellow, evaluation = get_machine_move(red, yellow, depth[turn], turn, CONSOLE_ARGS.engine_path)

        print(f'turn: {turn_to_player(turn):<10}eval: {evaluation:<10}')
        
        if CONSOLE_ARGS.graphic:
            prettyprint_board(red, yellow)
            print()
        else:
            print(f'position: red={hex(red)} yellow={hex(yellow)}')
            print()
        
        game_over, winner = is_game_over(red, yellow, CONSOLE_ARGS.engine_path)
        
        turn += 1
        turn %= 2

    print(f'winner: {turn_to_player(winner)}')
    print(f'final position: red={hex(red)} yellow={hex(yellow)}')


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('engine_path')
    parser.add_argument('-r', '--red', default=5, help='depth search for RED', metavar='')
    parser.add_argument('-y', '--yellow', default=5, help='depth search for YELLOW', metavar='')
    parser.add_argument('--graphic', action='store_true', help='show a graphic board after each move')

    global CONSOLE_ARGS
    CONSOLE_ARGS = parser.parse_args()

    run_game()


if __name__ == "__main__":
    main()
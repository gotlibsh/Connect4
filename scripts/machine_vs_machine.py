import argparse
from subprocess import Popen, PIPE
from wrapper import get_machine_move, is_game_over

CONSOLE_ARGS = None


def run_game():
    turn = 0
    depth = [CONSOLE_ARGS.red, CONSOLE_ARGS.yellow]
    red, yellow = 0, 0

    game_over, winner = is_game_over(red, yellow, CONSOLE_ARGS.engine_path)
    while not game_over:
        red, yellow, evaluation = get_machine_move(red, yellow, depth[turn], turn, CONSOLE_ARGS.engine_path)
        print(f'turn: {turn}, eval: {evaluation}')
        game_over, winner = is_game_over(red, yellow, CONSOLE_ARGS.engine_path)
        turn += 1
        turn %= 2

    print(winner)
    print(hex(red), hex(yellow))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('engine_path')
    parser.add_argument('-r', '--red', default=5, help='depth search for RED', metavar='')
    parser.add_argument('-y', '--yellow', default=5, help='depth search for YELLOW', metavar='')

    global CONSOLE_ARGS
    CONSOLE_ARGS = parser.parse_args()

    run_game()


if __name__ == "__main__":
    main()
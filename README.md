# Connect4

Connect4 is a C implementation of the Connect4 game for machine vs. human.

The machine's play is based on the minimax algorithm extended with alpha-beta pruning.

## Build

```bash
cmake .
make
```

## Usage
```
Usage: Connect4 [options]
Options:
      play [<machine|human> [depth=N]]     play against the machine and specify the starting player
                                           and the machine's depth search
      util <eval|isover>                   eval: evaluates a given position and returns a bit-board with the new move along with the evaluation
                                                 this command reads from standard input the following: <red_bitboard> <yellow_bitboard> <depth> <piece>
                                           isover: evaluates a given position whether it's game over and determines the winner
                                                 this command reads from standard input the following: <red_bitboard> <yellow_bitboard>
      test                                 run all built-in tests
```

## Play

In play mode, you play versus the machine.
You can specify who's the starting player by passing a parameter:
```
./Connect4 human
```
if you wish to be the starting player, or:
```
./Connect4 machine
```
if you wish for the machine to be the starting player.
Additionally, you can specify the depth in which the machine will go to search for his moves by passing a second parameter:
```
./Connect4 human 10
```
will force the machine to search up to depth 10.

## Util

In util mode the program can be used to check up on a specific position what the machine thinks is the best move up to a specified depth
or whether or not a given board is in game-over state.<br/>
To get the best move suggested by the machine for  a specific position, use:

```
./Connect4 util eval
```

Next, write in stdin the board position in bit-board format, first bit-board for the red pieces and second bit-board for the yellow pieces (separated by newline).<br/>
Then, write the depth of which you want the machine to search, and finally, the next piece to put in the board, use 0 for red and 1 for yellow.
Example:

```
./Connect4 util eval
0x000000000002182C
0x0000000001040412
7
1
```

## Run Tests

```bash
./Connect4 test
```

## Bit Board


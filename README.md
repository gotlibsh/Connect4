# Connect4

Connect4 is a C implementation of the Connect4 game for machine vs. human.<br/>
The machine's play is based on the minimax algorithm extended with alpha-beta pruning.


## Bitboard

To represent a board position, this program uses bitboards.<br/>
Each position is represented by two 64-bit integers, the first one is called *red_bitboard* and the second one *yellow_bitboard*.<br/>
The *red_bitboard* uses its lower 42 bits to hold all of the locations on the board where red pieces are present, same goes for *yellow_bitboard*. The upper 22 bits of each bitboard are not used.<br/>

For example, the following bitboard:

```
red_bitboard:    0x000000000002182C
yellow_bitboard: 0x0000000001040412
```

represents the following board (X = red, O = yellow):

```
- - - - - - -
- - - - - - -
- - - O - - -
- - O X - - -
- X X O - - -
- X O X X O -
```

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

this will force the machine to search up to depth 10.

## Util

In util mode the program can be used to check up on a specific position what the machine thinks is the best move up to a specified depth
or whether or not a given board is in game-over state.<br/>
To get the best move suggested by the machine for a specific position, use:

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

The output will look like this:

```
0x000000000002182c 0x0000000003040412 -13
```

First hexadecimal number represents the red bit-board and the second represents the yellow bit-board, both after the machine's new move.<br/>
As you can see, the red bit-board didn't change because the machine was told to make a move for yellow.<br/>
The 3rd number is the machine's objective evaluation of the given position (positive means red is better, negative means yellow is better).<br/>

To check on a position if it's game-over and get the winner, use:

```
./Connect4 util isover
```

Next, write in stdin the board position in bit-board format, first bit-board for the red pieces and second bit-board for the yellow pieces (separated by newline).<br/>
Example:

```
./Connect4 util isover
0x000000C00808AD68
0x0000000183561012
```

checks up on the following (won by red) position:

```
- - X X - - -
- - O O - - -
X - O O - O -
O X O O - X -
X O X X - X -
X X O X - O -
```

The output will look like this:

```
1 0
```

First number indicates game-over (1 means game is over, 0 means game is not over), and the second number indicates the winner (0 for red, 1 for yellow).<br/>
In case the game is not over, the winner indication will be 2 and can be ignored.


## Run Tests

To run all built-in tests simply use:

```
./Connect4 test
```

If all tests passed you should see:

```
Tests Passed
```

Otherwise, you'll get a failure message pointing to the file and line causing it:

```
Assertion failed - File: C:\Users\user\projects\connect4\tester.c - Line: 521
```

## GUI

Web Link: [Connect 4 Heroku App](https://connect4-react.herokuapp.com/)

For the backend, the [Python Server Script](scripts/server.py) was used (with slight modifications to fit for Heroku deployment).<br/>
For the frontend the [React App](gui/) was used. Please see [here](gui/README.md) how to setup the GUI locally.<br/>
Credits: [Jacob Gottlieb](https://github.com/GottliebJacob) did an amazing job building this wonderful react application.<br/>

## Conclusion
### Go play and have fun!!!
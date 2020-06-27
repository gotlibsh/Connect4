# Connect4 GUI

Here are the instructions to setup the web-gui locally on your machine.<br/>

### Note
Although it works for both Windows/Linux, the instructions here are for Windows only.

## Prerequisites

Make sure you have the following installed on your machine:
1. npm
2. python 3

## Setup

First lets install all of the required node packages:
```
connect4/gui> npm install
```

Next lets install all of the required packages for the python server:
```
connect4/gui> cd ../scripts
connect4/scripts> pip install -r requirements.txt 
```

### Note
You can recompile the executable before starting the game to make sure it's compatible with your system ([see here the build instructions](../README.md)).<br/>
If you choose to do it, you should copy the Connect4 executable to the [gui](.) folder.

## Run

Lets change back the directory to the [gui](.) folder:
```
connect4/scripts> cd ../gui
```

Finally lets start the game:
```
connect4/gui> npm run game
```

The web browser should open and show the game menu.

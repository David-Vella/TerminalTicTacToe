#!/bin/bash

INSTALLPATH=/usr/local/bin

# check for dependencies 
GCC=$(pacman -Q gcc)
NCURSES=$(pacman -Q ncurses)

if [[ "$GCC" == *"gcc"* && "$NCURSES" == *"ncurses"* ]]; then
    echo "Dependencies found: $GCC, $NCURSES"
else 
    echo "error: Missing dependency"
    exit 1
fi

# attmept to compile
gcc -Werror -Wall -std=c99 main.c tictactoe.c -lncurses -o tictactoe

# if last command failed then exit script and print error message
if [[ $? -ne 0 ]]; then
    echo "Compilation Failed"
    exit 1
fi

echo "Compilation Successfull"

# attempt to install
cp tictactoe $INSTALLPATH

if [[ $? -ne 0 ]]; then
    echo "error: installation failed"
    exit 1
else 
    echo "Installation successfull"
fi

exit 0
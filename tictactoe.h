/*
 * Lightwight Tic-Tac-Toe library featuring an intelligent
 * and extremely efficient bot.
 *  
 * Created by David Vella, November 2019
 */

#ifndef BOARD_H
#define BOARD_H

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>

// Player markers
#define EMPTY_MARKER ' ' // ASCII: 32
#define PLAYER_MARKER 'X' // ASCII: 88
#define CPU_MARKER 'O' // ASCII: 79

// game outcomes
#define IN_PROGRESS 0
#define PLAYER_WIN 1
#define CPU_WIN 2
#define NO_WINNER 3

// board spaces
#define TOP_LEFT 0
#define TOP_MIDDLE 1
#define TOP_RIGHT 2
#define MIDDLE_LEFT 3
#define MIDDLE_CENTER 4
#define MIDDLE_RIGHT 5
#define BOTTOM_LEFT 6
#define BOTTOM_MIDDLE 7
#define BOTTOM_RIGHT 8
#define BOARD_SIZE 9

// three-in-a-row opportunities 
#define TOP_ROW 1
#define MIDDLE_ROW 2
#define BOTTOM_ROW 4
#define LEFT_COLUMN 8
#define MIDDLE_COLUMN 16
#define RIGHT_COLUMN 32
#define BACK_DIAGONAL 64
#define FORWARD_DIAGONAL 128

// Indexes for relevant sums array
#define TOP_ROW_IND 0
#define MIDDLE_ROW_IND 1
#define BOTTOM_ROW_IND 2
#define LEFT_COLUMN_IND 3
#define MIDDLE_COLUMN_IND 4
#define RIGHT_COLUMN_IND 5
#define BACK_DIAGONAL_IND 6
#define FORWARD_DIAGONAL_IND 7
#define RELEVANT_SUMS 8

// Initializes an empty board
// board - char array of size 9
void board_init(char *board);

// Gets the marker on the board at index
// board - char array of size 9
// index - int 0-8, representing the board space
// returns a char representing the marker
char board_at(char *board, uint8_t index);

// Determines spaces in the board where markers have not been played.
// Fills open array with pointers to spaces in board.
// board - char array of size 9
// open - char pointer array of size 9
// returns the number of open spaces
uint8_t board_open_spaces(char *board, char **open);

// Places a marker on the board at index
// board - char array of size 9
// marker - char to be played on board
// index - int 0-8, board space where marker is played
void board_player_move(char *board, char marker, uint8_t index);

// Places a marker randomly on the board
// board - char array of size 9
// marker - char to be played on board
void board_cpu_move_rand(char *board, char marker);

// Intelligently places a marker to avoid losing 
// board - char array of size 9
// marker - char to be played on board
void board_cpu_move_smart(char *board, char marker, uint8_t turn);

// Checks if the game is over
// board - char array of size 9
// returns IN_PROGRESS if the game is not over
uint8_t board_game_over(const char *board);

#endif // BOARD_H
/*
 * Lightwight Tic-Tac-Toe library featuring an intelligent
 * and extremely efficient bot.
 *  
 * Created by David Vella, November 2019
 */

#include "tictactoe.h"

// Initializes an empty board
// board - char array of size 9
void board_init(char *board)
{
    srand(time(NULL));

    for (char *ptr = board; ptr < board + BOARD_SIZE; ++ptr)
        *ptr = EMPTY_MARKER;
}

// Gets the marker on the board at index
// board - char array of size 9
// index - int 0-8, representing the board space
// returns a char representing the marker
char board_at(char *board, uint8_t index)
{
    assert(0 <= index && index <= 8);

    return board[index];
}

// Determines spaces in the board where markers have not been played.
// Fills open array with pointers to spaces in board.
// board - char array of size 9
// open - char pointer array of size 9
// returns the number of open spaces
uint8_t board_open_spaces(char *board, char **open)
{
    char **open_ptr = open;

    for (char *board_ptr = board; board_ptr < board + BOARD_SIZE; ++board_ptr)
    {
        if (*board_ptr == EMPTY_MARKER)
            *open_ptr++ = board_ptr;
    }
    return open_ptr - open;
}

// Places a marker on the board at index
// board - char array of size 9
// marker - char to be played on board
// index - int 0-8, board space where marker is played
void board_player_move(char *board, char marker, uint8_t index)
{
    assert(0 <= index && index <= 8);

    board[index] = marker;
}

// Places a marker randomly on the board
// board - char array of size 9
// marker - char to be played on board
void board_cpu_move_rand(char *board, char marker)
{
    char *open_spaces[BOARD_SIZE];
    uint8_t num_spaces = board_open_spaces(board, open_spaces);

    *open_spaces[rand() % num_spaces] = marker;
}

// Computes the sum of chars along each 3-in-a-row opportunity.
// Writes solutions into sums array.
// board - char array of size 9
// sums - uint array of size 8
static void board_relevant_sums(const char *board, uint16_t *sums)
{ 
    sums[TOP_ROW_IND] = 
        board[TOP_LEFT] + board[TOP_MIDDLE] + board[TOP_RIGHT];

    sums[MIDDLE_ROW_IND] = 
        board[MIDDLE_LEFT] + board[MIDDLE_CENTER] + board[MIDDLE_RIGHT];

    sums[BOTTOM_ROW_IND] = 
        board[BOTTOM_LEFT] + board[BOTTOM_MIDDLE] + board[BOTTOM_RIGHT];

    sums[LEFT_COLUMN_IND] = 
        board[TOP_LEFT] + board[MIDDLE_LEFT] + board[BOTTOM_LEFT];

    sums[MIDDLE_COLUMN_IND] = 
        board[TOP_MIDDLE] + board[MIDDLE_CENTER] + board[BOTTOM_MIDDLE];

    sums[RIGHT_COLUMN_IND] = 
        board[TOP_RIGHT] + board[MIDDLE_RIGHT] + board[BOTTOM_RIGHT];

    sums[FORWARD_DIAGONAL_IND] = 
        board[BOTTOM_LEFT] + board[MIDDLE_CENTER] + board[TOP_RIGHT];

    sums[BACK_DIAGONAL_IND] = 
        board[TOP_LEFT] + board[MIDDLE_CENTER] + board[BOTTOM_RIGHT];
}

// Intelligently places a marker to avoid losing 
// board - char array of size 9
// marker - char to be played on board
void board_cpu_move_smart(char *board, char marker)
{
    static uint8_t turn = 1;

    char *cpu_move = board;

    char *open_spaces[BOARD_SIZE];
    board_open_spaces(board, open_spaces);

    // Use bytes to construct boolean arrays indicating which 
    // 3-in-a-row winning opportunities intersect at a space.
    uint8_t intersects[BOARD_SIZE];

    intersects[TOP_LEFT] = TOP_ROW | LEFT_COLUMN | BACK_DIAGONAL;
    intersects[TOP_MIDDLE] = TOP_ROW | MIDDLE_COLUMN;
    intersects[TOP_RIGHT] = TOP_ROW | RIGHT_COLUMN | FORWARD_DIAGONAL;

    intersects[MIDDLE_LEFT] = MIDDLE_ROW | LEFT_COLUMN;
    intersects[MIDDLE_CENTER] = MIDDLE_ROW | MIDDLE_COLUMN | FORWARD_DIAGONAL | BACK_DIAGONAL;
    intersects[MIDDLE_RIGHT] = MIDDLE_ROW | RIGHT_COLUMN;
    
    intersects[BOTTOM_LEFT] = BOTTOM_ROW | LEFT_COLUMN | FORWARD_DIAGONAL;
    intersects[BOTTOM_MIDDLE] = BOTTOM_ROW | MIDDLE_COLUMN;
    intersects[BOTTOM_RIGHT] = BOTTOM_ROW | RIGHT_COLUMN | BACK_DIAGONAL;

    uint16_t sums[RELEVANT_SUMS];
    board_relevant_sums(board, sums);

    uint8_t max_winning_potential = 0;

    for (uint8_t space = 0; space < BOARD_SIZE; ++space)
    {
        uint8_t winning_potential = 0;

        if (board[space] != EMPTY_MARKER)
            continue;
         
        uint8_t index = 0; // keep track of index of board array

        for (uint8_t i = 1; i; i <<= 1)
        {
            if (intersects[space] & i)
            {
                // add winning potential depending on the markers present
                // in the 3-in-a-row winning opportunities intersecting
                // the space.
                if (sums[index] == 2 * CPU_MARKER + EMPTY_MARKER)
                    winning_potential += 50;

                if (sums[index] == 2 * PLAYER_MARKER + EMPTY_MARKER)
                    winning_potential += 20;

                if (sums[index] == CPU_MARKER + 2 * EMPTY_MARKER)
                    winning_potential += 3;

                if (sums[index] == PLAYER_MARKER + 2 * EMPTY_MARKER)
                    winning_potential += 2;

                if (sums[index] == 3 * EMPTY_MARKER)
                    winning_potential += 1;
            }
            ++index;
        }
        // if there are multiple spaces with equal winning potential, 
        // choose one randomly
        if (winning_potential > max_winning_potential
            || (winning_potential == max_winning_potential && rand() % 2))
        {
            max_winning_potential = winning_potential;
            cpu_move = board + space;
        }
    }
    // Special case where the bot can lose
    // [X][ ][ ]
    // [ ][O][ ]
    // [ ][ ][X]
    // Algorithm computes optimal play in corner so force play in one of
    // the side spaces
    if (board[TOP_LEFT] == board[BOTTOM_RIGHT] && 
        board[TOP_RIGHT] == board[BOTTOM_LEFT] && turn == 2 &&
       (board[TOP_RIGHT] == PLAYER_MARKER || board[TOP_LEFT] == PLAYER_MARKER))
            board[2 * (rand() % 4) + 1] = CPU_MARKER;
    else
        *cpu_move = CPU_MARKER;

    ++turn;
}

// Checks if the game is over
// board - char array of size 9
// returns IN_PROGRESS if the game is not over
uint8_t board_game_over(const char *board)
{
    uint8_t filled = 0;

    uint16_t sums[RELEVANT_SUMS];
    board_relevant_sums(board, sums);
     
    for (const uint16_t *ptr = sums; ptr < sums + RELEVANT_SUMS; ++ptr)
    {
        if (*ptr == 3 * PLAYER_MARKER)
            return PLAYER_WIN;
        if (*ptr == 3 * CPU_MARKER)
            return CPU_WIN;
        if (*ptr > 3 * CPU_MARKER)
            ++filled;
    }
    if (filled == RELEVANT_SUMS)
        return NO_WINNER;

    return IN_PROGRESS;
}
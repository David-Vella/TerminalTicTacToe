/*
 * Terminal Tic-Tac-Toe app for linux terminals featuring an 
 * intelligent cpu bot. Created using the ncurses library
 * 
 * Created by David Vella, November 2019
 */

#include <ncurses.h>

#include "tictactoe.h"

// cpu player difficulties
#define IMPOSSIBLE 0
#define EASY 1

// Key Codes
#define KEY_q 113
#define KEY_1 49
#define KEY_2 50
#define KEY_3 51
#define KEY_4 52
#define KEY_5 53
#define KEY_6 54
#define KEY_7 55
#define KEY_8 56
#define KEY_9 57
#define BOARD_KEY_OFFSET 49

// game modes
#define CPU_EASY KEY_1
#define CPU_IMPOSSIBLE KEY_2
#define TWO_PLAYER KEY_3

// Human player move outcomes
#define LEAVE 0
#define MOVE 1

// diff - cpu difficultly setting
uint8_t play_cpu_game(uint8_t diff);
uint8_t play_two_player_game();

// board - char array of size 9
int8_t player_move(char *board, char marker);

// board - char array of size 9
void display_board_screen(const char *board);
void display_result_screen(int8_t result, int8_t mode);
void display_title_screen();

int main()
{
    // initialize ncurses terminal window
    initscr();
    noecho();

    int8_t result = -1;

    while (true)
    {
        display_title_screen();
    
        int8_t mode = 0;

        // wait for user to choose game mode with valid input
        while ((mode != KEY_q && mode > KEY_3) || mode < KEY_1)
            mode = wgetch(stdscr);

        if (mode == KEY_1)
            result = play_cpu_game(EASY);

        else if (mode == KEY_2)
            result = play_cpu_game(IMPOSSIBLE);

        else if (mode == KEY_3)
            result = play_two_player_game();

        // if user presses q on title screen, exit loop
        if (mode == KEY_q)
            break;

        // if user leaves a game, return to title screen
        if (result == LEAVE)
            continue;

        display_result_screen(result, mode);

        // wait for user to press any key, then return to title screen
        int input = -1;
        while (result != LEAVE && input == -1)
            input = wgetch(stdscr);
    }

    endwin();

    return EXIT_SUCCESS;
}

uint8_t play_cpu_game(uint8_t diff)
{
    uint8_t result = 0;

    uint8_t turn = 1;

    char board[BOARD_SIZE];
    board_init(board);

    display_board_screen(board);

    while (result == IN_PROGRESS)
    {
        if (player_move(board, PLAYER_MARKER) == LEAVE)
            return result;

        display_board_screen(board);

        result = board_game_over(board);
        if (result != IN_PROGRESS)
            return result;

        if (diff == IMPOSSIBLE)
            board_cpu_move_smart(board, CPU_MARKER, turn);
        else
            board_cpu_move_rand(board, CPU_MARKER);

        display_board_screen(board);

        result = board_game_over(board);
        if (result != IN_PROGRESS)
            return result;
        
        ++turn;
    }
    return result;
}

uint8_t play_two_player_game()
{
    uint8_t result = 0;

    char board[BOARD_SIZE];
    board_init(board);

    display_board_screen(board);
    char marker = PLAYER_MARKER;

    do 
    {
        if (player_move(board, marker) == LEAVE)
            return result;

        display_board_screen(board);

        result = board_game_over(board); 

        if (marker == PLAYER_MARKER)
            marker = CPU_MARKER;
        else 
            marker = PLAYER_MARKER;
    } 
    while (result == IN_PROGRESS);

    return result;   
}

int8_t player_move(char *board, char marker)
{
    int input = 0;

    while (input != KEY_q && ((KEY_1 > input || input > KEY_9) 
            || board[input - BOARD_KEY_OFFSET] != ' '))
        input = wgetch(stdscr);

    if (input == KEY_q)
        return LEAVE;
    else
    {
        board_player_move(board, marker, input - BOARD_KEY_OFFSET);
        return MOVE;
    }
}

void display_align_vertical(const int8_t msg_height)
{
    for (int i = 0; i < LINES / 2 - msg_height / 2; ++i)
        printw("\n");
}

void display_align_horizontal(const int8_t msg_width)
{
    for (int i = 0; i < COLS / 2 - msg_width / 2; ++i)
        printw(" ");
}

void display_title_screen()
{
    const int8_t TITLE_SCREEN_WIDTH = 32;
    const int8_t TITLE_SCREEN_HEIGHT = 11;
    const char *title_line0 = "     Welcome to Tic-Tac-Toe     \n";
    const char *title_line1 = "     created by David Vella     \n";
    const char *title_line2 = "    press 1-3 to select mode    \n";
    const char *title_line3 = "  1 - player vs easy cpu        \n";
    const char *title_line4 = "  2 - player vs impossible cpu  \n";
    const char *title_line5 = "  3 - two player                \n";
    const char *title_line6 = "    press q anytime to leave    \n";

    werase(stdscr);

    display_align_vertical(TITLE_SCREEN_HEIGHT);

    display_align_horizontal(TITLE_SCREEN_WIDTH);
    printw(title_line0);

    display_align_horizontal(TITLE_SCREEN_WIDTH);
    printw(title_line1);

    printw("\n");

    display_align_horizontal(TITLE_SCREEN_WIDTH);
    printw(title_line2);    

    printw("\n");

    display_align_horizontal(TITLE_SCREEN_WIDTH);
    printw(title_line3);

    display_align_horizontal(TITLE_SCREEN_WIDTH);
    printw(title_line4);

    display_align_horizontal(TITLE_SCREEN_WIDTH);
    printw(title_line5);

    printw("\n");

    display_align_horizontal(TITLE_SCREEN_WIDTH);
    printw(title_line6);

    wrefresh(stdscr);

    curs_set(0);
}

void display_board_screen(const char *board)
{
    const int8_t BOARD_SCREEN_WIDTH = 36;
    const int8_t BOARD_SCREEN_HEIGHT = 5;
    const char *board_line0 = "     [%c][%c][%c]        [1][2][3]     \n";
    const char *board_line1 = "     [%c][%c][%c]        [4][5][6]     \n";
    const char *board_line2 = "     [%c][%c][%c]        [7][8][9]     \n";
    const char *board_line3 = "choose a space by pressing a key 1-9\n";

    werase(stdscr);
    display_align_vertical(BOARD_SCREEN_HEIGHT);

    display_align_horizontal(BOARD_SCREEN_WIDTH);
    printw(board_line0, board[TOP_LEFT], board[TOP_MIDDLE], board[TOP_RIGHT]); 

    display_align_horizontal(BOARD_SCREEN_WIDTH);
    printw(board_line1, board[MIDDLE_LEFT], board[MIDDLE_CENTER], board[MIDDLE_RIGHT]);

    display_align_horizontal(BOARD_SCREEN_WIDTH);
    printw(board_line2, board[BOTTOM_LEFT], board[BOTTOM_MIDDLE], board[BOTTOM_RIGHT]);

    printw("\n");

    display_align_horizontal(BOARD_SCREEN_WIDTH);
    printw(board_line3);

    wrefresh(stdscr);

    curs_set(0);
}

void display_result_screen(int8_t result, int8_t mode)
{
    printw("\n");

    if (result == PLAYER_WIN)
    {
        if (mode == CPU_EASY || mode == CPU_IMPOSSIBLE)
        {
            display_align_horizontal(12);
            printw("Player Wins!");
        }
        else 
        {
            display_align_horizontal(14);
            printw("Player 1 Wins!");            
        }
    }
    else if (result == CPU_WIN)
    {
        if (mode == CPU_EASY || mode == CPU_IMPOSSIBLE)
        {
            display_align_horizontal(8);
            printw("CPU Wins");
        }
        else 
        {
            display_align_horizontal(14);
            printw("Player 2 Wins!");            
        }
    }
    else
    {
        display_align_horizontal(12);
        printw("It's a Tie");
    }
    curs_set(0);
}
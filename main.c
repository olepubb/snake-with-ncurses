#include "snake.h"
#include <string.h>

int main()
{
    if(init_curses() == -1) return -1; // Checks if the terminal supports color, if not prints error and quits.

    // Strings for the welcome and game over screen.
    const char *tutorial_msg = "Arrow keys to move, 'e' to quit the game.";
    const char *welcome_msg = "Welcome to snake, eat frog, don't eat self.";
    const char *restart_msg = "Press 'r' to play again.";
    const char *gameover_msg = "GAME OVER";

    // Prints the welcome screen to the main window and waits for input.
    attron(COLOR_PAIR(1));
    mvprintw((LINES / 2) - 1, (COLS / 2) - (strlen(welcome_msg) / 2), welcome_msg);
    attroff(COLOR_PAIR(1));
    mvprintw((LINES / 2) + 1, (COLS / 2) - (strlen(tutorial_msg) / 2), tutorial_msg);
    getch();

    // Create window for snake game and start the game it that window.
    WINDOW *win = create_snake_window();
    snake_go: // Label for the end screen.
        snake(win);
    wclear(win); // When game ends clear the game window buffer (in case of a restart).
    clear(); // Clear main window to print game over screen.

    // Prints game over screen to main screen.
    attron(COLOR_PAIR(1));
    mvprintw((LINES / 2) - 3, (COLS / 2) - (strlen(gameover_msg) / 2), gameover_msg);
    attroff(COLOR_PAIR(1));
    char score_msg[20];// Create dynamic string that displays the user score (assumes no one will get a score bigger than 99,999).
    snprintf(score_msg, sizeof(score_msg), "You ate %d frogs.", score); // Format the score string.
    mvprintw((LINES / 2) - 1, (COLS / 2) - (strlen(score_msg) / 2), score_msg);
    mvprintw((LINES / 2), (COLS / 2) - (strlen(restart_msg) / 2), restart_msg);
    refresh();
    
    // If user presses 'r' go to 'snake_go' label and start the game over. Else close ncurses and quit.
    if(getch() == 'r') goto snake_go;
    endwin();
    return 0;
}
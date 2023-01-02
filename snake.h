#ifndef SNAKE_H
#define SNAKE_H

#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// Global vars (window size and socre keeper);
int stdwin_size_x, stdwin_size_y, score; 

// Init curses.
int init_curses()
{
    initscr(); // Initialize curses.
    if(has_colors() == FALSE) // Check if terminal has color support.
	{	
		printf("Your terminal does not support color.\n");
		endwin();
        return -1;
	}
    start_color(); // Start using colors.
    init_color(COLOR_BLACK, 0, 0, 0); // Set black to be the blackest black.
    init_pair(1, COLOR_GREEN, COLOR_BLACK); // Pair green and black.
    cbreak(); // Make characters available as soon as the user types them (not waiting for ENTER). 
    noecho(); // Dont show characters typed.
    keypad(stdscr, TRUE); // Enable reading function keys (like arrow keys). **
    curs_set(0); // Invisble curser
    return 0;
};

// Create the game window for snake and place it in the middle of the main window.
WINDOW* create_snake_window()
{
    getmaxyx(stdscr, stdwin_size_y, stdwin_size_x);
    int win_heigth = stdwin_size_y - 5;
    int win_width = stdwin_size_x - 10;
    WINDOW *win = newwin(win_heigth, win_width, (stdwin_size_y / 2) - (win_heigth / 2), (stdwin_size_x / 2) - (win_width / 2));
    keypad(win, TRUE);
    wtimeout(win, 0);
    box(win, 0, 0);
    return win;
};

// Snake data structure (doubley linked list) and its functions.
typedef struct snake_node
{
    int x;
    int y;
    struct snake_node *next;
    struct snake_node *prev;
} snake_node;

// Create new snake node.
snake_node* create_snake_node(int x, int y, snake_node* prev)
{
    snake_node *tmp = (snake_node*)malloc(sizeof(snake_node));
    tmp->x = x;
    tmp->y = y;
    tmp->next = NULL;
    tmp->prev = prev;
    return tmp;
};

// Add new snake node to the tail of the snake.
void snake_add_tail(snake_node *head)
{   
    snake_node *tmp = head;
    if(tmp->next == NULL)
    {
        tmp->next = create_snake_node(tmp->x, tmp->y, tmp);
    }
    else
    {
        snake_add_tail(tmp->next);
    }
};

// Update the snake.
void update_snake(snake_node* head, int new_x, int new_y, WINDOW* win)
{
    snake_node *tmp = head;
    while(tmp->next != NULL) tmp = tmp->next; // Go to end of the snake.
    mvwaddch(win, tmp->y, tmp->x, ' '); // Print 'blank' at the end of the snake.
    while(tmp->prev != NULL) // Shift all the coordinates forward until the head.
    {
        tmp->x = tmp->prev->x;
        tmp->y = tmp->prev->y;
        tmp = tmp->prev;
    }
    // New head coordinates.
    tmp->x = new_x;
    tmp->y = new_y;
};

// Print the snake to the window.
void print_snake(snake_node* head, WINDOW* win)
{
    snake_node *tmp = head;
    while(tmp != NULL)
    {
        mvwaddch(win, tmp->y, tmp->x, ACS_BLOCK);
        tmp = tmp->next;
    }
};

// Free snake allocated memory.
void free_snake(snake_node* head)
{
    if(head == NULL) return;
    free_snake(head->next);
    free(head);
}

void snake(WINDOW* win)
{
    // Vars
    int ch; // character buffer
    int player_x, player_y; // Snake head position.
    int screen_x, screen_y; // Screen size.
    int SNAKE_SPEED_X = 50000; // Movment speed on X axis.
    int SNAKE_SPEED_Y = 70000; // Movment speed on Y axis.
    score = 0; // Score counter.
    int frog_x, frog_y; // Frog coordinates.

    // Player direction and state.
    enum direction { close, stand, up, down, left, right };
    typedef enum direction direction;
    direction d = stand;

    // Get screen dimensions.
    getmaxyx(win, screen_y, screen_x);

    // Place player in the middle of the screen.
    player_x = screen_x / 2;
    player_y = screen_y / 2;

    // Init player.
    snake_node *head = create_snake_node(player_x, player_y, NULL);
    print_snake(head, win);

    // Init frog. (Place it in random positon and paint it green).
    srand(time(0));
    frog_x = ((rand() % (screen_x - 2)) + 1);
    frog_y = ((rand() % (screen_y - 2)) + 1);
    wattron(win, COLOR_PAIR(1));
    mvwaddch(win, frog_y, frog_x, ACS_DIAMOND);
    wattroff(win, COLOR_PAIR(1));
    
    // GAME LOOP
    while (d != close)
    {
        // Print score to the screen.
        move(0, 0);
        clrtoeol();
        mvprintw(0, 0, "Score: %d", score);
        refresh();
        box(win, 0 ,0);
        wrefresh(win);

        // Handle collison with frog and update score.
        if(player_x == frog_x && player_y == frog_y)
        {
            score++; // Update score.
            frog_x = ((rand() % (screen_x - 2)) + 1); 
            frog_y = ((rand() % (screen_y - 2)) + 1);
            wattron(win, COLOR_PAIR(1));
            mvwaddch(win, frog_y, frog_x, ACS_DIAMOND); // Print new frog.
            wattroff(win, COLOR_PAIR(1));
            snake_add_tail(head); // Add snake tail.
        };

        // Print frog to screen on every iteration of the loop in case new frog spawns inside snake.
        // This is fine becasue ncurses checks if something is already printed.
        wattron(win, COLOR_PAIR(1));
        mvwaddch(win, frog_y, frog_x, ACS_DIAMOND); // Print new frog.
        wattroff(win, COLOR_PAIR(1));

        // Player movement update.
        if(d == left)
        {
            if(mvwinch(win, player_y, player_x - 1) == ACS_BLOCK) d = close; // Collision with self.
            if(player_x - 1 == 0) d = close; // Collison with wall.
            player_x--; // Update head coordinate.
            update_snake(head, player_x, player_y, win); // Update whole snake.
            print_snake(head, win); 
            wrefresh(win); 
            usleep(SNAKE_SPEED_X); // Delay the program a bit so it is playable.
        }
        if(d == right)
        {
            if(mvwinch(win, player_y, player_x + 1) == ACS_BLOCK) d = close;
            if(player_x + 1 == screen_x - 1) d = close;
            player_x++;
            update_snake(head, player_x, player_y, win);
            print_snake(head, win);
            wrefresh(win);
            usleep(SNAKE_SPEED_X);
        }
        if(d == up)
        {
            if(mvwinch(win, player_y - 1, player_x) == ACS_BLOCK) d = close;
            if(player_y - 1 == 0) d = close;
            player_y--;
            update_snake(head, player_x, player_y, win);
            print_snake(head, win);
            wrefresh(win);
            usleep(SNAKE_SPEED_Y);
        }
        if(d == down)
        {
            if(mvwinch(win, player_y + 1, player_x) == ACS_BLOCK) d = close;
            if(player_y + 1 == screen_y - 1) d = close;
            player_y++;
            update_snake(head, player_x, player_y, win);
            print_snake(head, win);
            wrefresh(win);
            usleep(SNAKE_SPEED_Y);
        }

        // Player input.
        ch = wgetch(win);
        switch (ch)
        {
        case KEY_LEFT:
            d = left;
            break;
        case KEY_RIGHT:
            d = right;
            break;
        case KEY_UP:
            d = up;
            break;
        case KEY_DOWN:
            d = down;
            break;
        case 'e':
            d = close;
            break;
        }
    }
    free_snake(head); // Free snake from memory.
};

#endif
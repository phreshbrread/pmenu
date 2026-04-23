#include <stdio.h>
#include <ncurses.h>

/* Declare variables */
int startx  = 0;
int starty  = 0;
int rows    = 0;
int cols    = 0;
int input   = 0;

char *options[] = {"Shutdown", "Reboot", "Suspend", "Cancel"};
int option_count = sizeof(options) / sizeof(char *);    // Get number of options
/* -------------------------------------------------------------------------- */

int main() {
    initscr();              // Initialise curses mode
    cbreak();               // Disable line buffering
    noecho();               // Don't echo characters back to terminal
    keypad(stdscr, TRUE);   // Enable special keys - need for arrows
    
    /* Testing */
    printw("Option count: %i", option_count);
    /* ----------------------------------- */

    // Loop to account for window resizing
    while (true) {
        getmaxyx(stdscr, rows, cols);   // Get size of terminal
        erase();                        // Clear previous output

        printw("Working");
        refresh();

       /* Handle input here */
       input = getch();
       if (input == KEY_UP) {
           // Move selection up
       } else if (input == KEY_DOWN) {
           // Move selection down
       } else {
           // Do nothing
       }
       /* ----------- */
    }

    endwin();   // End curses mode
    return 0;
}

/* TODO:
 * 1. Open terminal when run
 *      - Just use $TERM to begin with, make more robust later
 *      - Ideally have the terminal appear as a floating window if possible
 * 2. Print 'power off', 'reboot' and 'suspend' options in the middle of the terminal
 * 3. Allow user to select an option
 * 4. Handle selected option appropriately
 * 5. Allow for some sort of customisation
*/

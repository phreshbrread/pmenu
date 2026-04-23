#include <stdio.h>
#include <ncurses.h>

/* Declare variables */
int startx = 0;
int starty = 0;
int rows, cols;

char *options[] = {"Shutdown", "Reboot", "Suspend", "Cancel"};
/* --------------------------------------------------------- */

int main() {
    initscr(); // Initialise curses mode
    keypad(stdscr, TRUE); // Enable arrow key usage
    noecho();
    cbreak(); // Disable line buffering

    
    // Loop to account for window resizing
    while (true) {
        getmaxyx(stdscr, rows, cols); // Get size of terminal
        erase(); // Clear previous output

        mvaddstr(rows/2, cols/3, "Test");
        //mvaddstr();
        refresh();
    }

    endwin(); // End curses mode
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

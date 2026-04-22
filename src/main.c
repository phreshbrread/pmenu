#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>


int init_window () {
    // Attempt to open default system terminal
    // Return 1 if it fails
    if (system("$TERM") != 0) {
        printf("Couldn't open a terminal\n");
        return 1;
    }

    return 0;
}

int main() {
    /*
    initscr(); // Initialise curses mode

    printw("Greetings, friend.");
    refresh();
    getch();


    endwin(); // End curses mode - free memory, data, etc    
    */

    init_window();

    return 0;
}



/* TODO:
 * 1. Open terminal when run
 *      - Just use $TERM to begin with, make more robust later
 *      - Ideally have the terminal appear as a floating window if possible
 * 2. Print 'power off', 'reboot' and 'suspend' options in the middle of the terminal
 * 3. Allow user to select an option
 * 4. Handle selected option appropriately
 *      - Need to determine init system
 *      - Need to figure out how this works on BSD too
*/

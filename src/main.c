#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <menu.h>

/* Declare variables */
int startx  = 0;
int starty  = 0;
int rows    = 0;
int cols    = 0;
int input   = 0;
int i       = 0;

char *options[] = {"Shutdown", "Reboot", "Suspend", "Cancel"};
int option_count = sizeof(options) / sizeof(char *);    // Determine number of array entries
/* -------------------------------------------------------------------------- */

int main() {
    /* Declare menu variables */
    MENU *power_menu;
    ITEM **items;
    /* ------- */

    initscr();              // Initialise curses mode
    cbreak();               // Disable line buffering
    noecho();               // Don't echo characters back to terminal
    keypad(stdscr, TRUE);   // Enable special keys - need for arrows

    items = (ITEM **)calloc(option_count + 1, sizeof(ITEM *));

    /* Create menu items based on options array */
    for (i; i < option_count; ++i) {
        items[i] = new_item(options[i], options[i]);    // Add items here
    }
    items[option_count] = (ITEM *)NULL;                 // Terminate option list with null pointer
    /* ---------------------------------------- */

    power_menu = new_menu(items);     // Create power menu
    post_menu(power_menu);            // Display power menu
    
    refresh();                                  

    /* Handle input here */
    while (true) {
    input = getch();
    if (input == KEY_UP) {
        menu_driver(power_menu, REQ_UP_ITEM);
        break;
    } else if (input == KEY_DOWN) {
        menu_driver(power_menu, REQ_DOWN_ITEM);
        break;
    }
    }

    /* ----------- */
    
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

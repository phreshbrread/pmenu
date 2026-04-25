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

char *options[] = { "Shutdown", "Reboot", "Suspend", "Cancel" };
int option_count = sizeof(options) / sizeof(char *);    // Determine number of array entries
/* -------------------------------------------------------------------------- */

int main() {
    /* Declare menu variables */
    MENU *power_menu;
    ITEM **items;
    /* ---------------------- */

    /* Start curses mode, disable line buffering, disable character echo and enable special keys */
    initscr();
    cbreak();
    noecho(); 
    keypad(stdscr, TRUE); 
    /* ----------------------------------------------------------------------------------------- */

    /* Allocate memory for array of pointers to ITEM, zero-initialising everything so the last item is NULL */
    items = calloc(option_count + 1, sizeof(ITEM *));
    /* ---------------------------------------------------------------------------------------------------- */

    /* Create menu items based on options array */
    for (i; i < option_count; ++i) {
        items[i] = new_item(options[i], "");
    }
    items[option_count] = (ITEM *)NULL; // Terminate option list with null pointer
    /* ---------------------------------------- */

    power_menu = new_menu(items);     // Create power menu
    post_menu(power_menu);            // Display power menu

    refresh();                                  

    /* Handle input here */
    while ((input = getch()) != KEY_F(1)) {
        if (input == KEY_UP) {
            menu_driver(power_menu, REQ_UP_ITEM);
        } else if (input == KEY_DOWN) {
            menu_driver(power_menu, REQ_DOWN_ITEM);
        }
    }
    /* ----------------- */

    /* Free memory used by menu */
    free_item(items[0]);
    free_item(items[1]);
    free_menu(power_menu);
    /* ------------------------ */

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

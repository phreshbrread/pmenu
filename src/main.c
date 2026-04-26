#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <menu.h>

/* Declare variables */
int selected_option_index   = 0;
int startx                  = 0;
int starty                  = 0;
int rows                    = 0;
int cols                    = 0;
int input                   = 0;
int i                       = 0;

char *options[] = { "Shutdown", "Reboot", "Suspend", "Cancel" };
int option_count = sizeof(options) / sizeof(char *);    // Determine number of array entries
/* -------------------------------------------------------------------------- */

int main() {
    /* Declare menu variables */
    MENU *power_menu;
    ITEM *selected_option;
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

    power_menu = new_menu(items);     // Create menu based off items
    post_menu(power_menu);            // Display power menu

    refresh();                                  

    /* Handle input */
    while ((input = getch())){
        switch (input) {
            case KEY_UP:
            case'k':
                menu_driver(power_menu, REQ_UP_ITEM);
                break;
            case KEY_DOWN:
            case'j':
                menu_driver(power_menu, REQ_DOWN_ITEM);
                break;
            case KEY_ENTER:
            case '\n':
            case '\r':
                selected_option_index = item_index(current_item(power_menu)); // Get the index of the current option
                mvprintw(6, 6, "%i", selected_option_index);
                break;
        }
    }
    /* ------------ */

    /* Determine action based on selected option */
    switch (selected_option_index) {
        case 0: // Shutdown
            break;
        case 1: // Reboot
            break;
        case 2: // Suspend
            break;
        case 3: // Cancel
            break;
    }
    /* ----------------------------------------- */


    /* Free memory used by menu */
    free_item(items[0]);
    free_item(items[1]);
    free_menu(power_menu);
    /* ------------------------ */

    endwin();   // End curses mode
    return 0;
}

/* TODO:
 * - Create new terminal when run
 *   - (Might not be necessary)
 * - Handle selected menu entry
 * - Center menu options correctly
 * - Allow for configuration options */

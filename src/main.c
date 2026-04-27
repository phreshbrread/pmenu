#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <menu.h>

/* Declare variables */
int selected_option_index   = 3; // 3 is the index for cancel
int startx                  = 0;
int starty                  = 0;
int rows                    = 0;
int cols                    = 0;
int input                   = 0;
int i                       = 0;

char *options[] = { "Shutdown", "Reboot", "Suspend", "Cancel" };
int option_count = sizeof(options) / sizeof(char *);    // Determine number of array entries
/* -------------------------------------------------------------------------- */

int main(int argc, char *argv[]) {
    /* Declare menu variables */
    MENU *power_menu;
    ITEM **items;
    /* ---------------------- */

    /* Initialise ncurses */
    set_escdelay(50); // Set delay for escape key
    initscr();
    cbreak();
    noecho(); 
    keypad(stdscr, TRUE); 
    /* ------------------ */

    // Allocate memory for array of pointers to ITEM,
    // zero-initialising everything so the last item is NULL
    items = calloc(option_count + 1, sizeof(ITEM *));

    /* Create menu items from options array */
    for (i; i < option_count; ++i) {
        items[i] = new_item(options[i], "");
    }
    items[option_count] = (ITEM *)NULL; // Terminate option list with null pointer
    /* ------------------------------------ */

    power_menu = new_menu((ITEM **)items);  // Create menu based off items
    set_menu_mark(power_menu, ">");         // Set menu marker
    post_menu(power_menu);                  // Display power menu

    refresh();                                  

    /* Handle input */
    // TODO handle '\r' and KEY_ENTER as well
    while ((input = getch()) != '\n') {
        switch (input) {
            case KEY_UP:
            case'k':
                menu_driver(power_menu, REQ_UP_ITEM);
                break;
            case KEY_DOWN:
            case'j':
                menu_driver(power_menu, REQ_DOWN_ITEM);
                break;
            case 27:        // 27 is the raw value of ESC since there is no KEY macro
                return 0;   // TODO handle properly
                break;
        }
    }
    /* ------------ */

    selected_option_index = item_index(current_item(power_menu)); // Get the index of the current option

    /* Free memory and end curses mode */
    for (i = 0; i < (option_count + 1); ++i) {
        free_item(items[i]);
    }
    free_menu(power_menu);
    endwin();
    /* ------------------------------- */

    /* Determine action based on selected option */
    // TODO implement actual power commands
    // TODO use execvp() instead of system()
    switch (selected_option_index) {
        case 0: // Shutdown
            printf("Shutting down...\n");

#if defined(__linux__)
            char *args[] = { "shutdown", "-P", "now", NULL };
            execvp("shutdown", args);


            break;
        case 1: // Reboot
            printf("Rebooting...\n");

            break;
        case 2: // Suspend
            printf("Suspending...\n");

#elif defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD)
            system("zzz");
#endif
            break;
        case 3: // Cancel
            printf("Cancelled.\n");
            break;
    }
    /* ----------------------------------------- */

    return 0;
}

/* Extra TODO:
 * - Create new terminal when run
 *   - (Might not be necessary)
 * - Center menu options correctly
 * - Allow for configuration options */

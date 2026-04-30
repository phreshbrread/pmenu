#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <menu.h>

/* Determine platform at compilation */
#if defined(__linux__)
#define PLATFORM_LINUX
#elif defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD)
#define PLATFORM_BSD
#endif
/* --------------------------------- */

/* Declare global variables */
int selected_option_index   = 3; // 3 is the index for cancel
int input                   = 0;
int i                       = 0;
int max_x                   = 0;
int max_y                   = 0;
int sub_max_x               = 0;
int sub_max_y               = 0;

bool enter_pressed          = false;

char *options[] = { "Shutdown", "Reboot", "Suspend", "Cancel" };
int option_count = sizeof(options) / sizeof(char *);    // Determine number of array entries
/* ------------------------ */

/* Declare menu variables */
ITEM **items;
MENU *power_menu;
MEVENT mouse_event;
WINDOW *menu_window;
WINDOW *menu_subwin;
/* ---------------------- */

/* Free up memory used by the menu */
void unload_menu() {
    unpost_menu(power_menu);
    free_menu(power_menu);
    for (i = 0; i < (option_count + 1); ++i) {
        free_item(items[i]);
    }
}
/* ------------------------------- */

int main(int argc, char *argv[]) {
    /* Initialise ncurses */
    set_escdelay(50); // Set delay for escape key
    initscr();
    cbreak();
    noecho(); 
    keypad(stdscr, TRUE);
    /* ------------------ */

    // TODO Allow for mouse usage
    // Set up mouse handling
    mousemask(BUTTON1_RELEASED, NULL);  // Left click release

    // Allocate memory for array of pointers to ITEM,
    // zero-initialising everything so the last item is NULL
    items = calloc(option_count + 1, sizeof(ITEM *));

    /* Create menu items from options array */
    for (i; i < option_count; ++i) {
        items[i] = new_item(options[i], "");
    }
    items[option_count] = (ITEM *)NULL; // Terminate option list with null pointer
    /* ------------------------------------ */

    /* Create window for menu */
    power_menu = new_menu((ITEM **)items);                              // Create menu based off items
    menu_opts_off(power_menu, O_NONCYCLIC);                             // Force enable menu wrapping
    set_menu_mark(power_menu, ">");                                     // Set menu marker
    getmaxyx(stdscr, max_y, max_x);                                     // Get size of terminal window
                                                                        //menu_window = newwin(max_y / 2, max_x / 2, max_y / 4, max_x / 4);   // Create a window in the middle of the terminal

                                                                        // WINDOW *newwin(int nlines, int ncols, int begin_y, int begin_x);
    menu_window = newwin(option_count + 4, strlen(options[0]) + 10, max_y / 4, max_x / 4);

    keypad(menu_window, TRUE);
    box(menu_window, 0, 0);
    /* ---------------------- */

    getmaxyx(menu_window, sub_max_y, sub_max_x);    // Get size of main menu window
    set_menu_win(power_menu, menu_window);          // Assign power menu to the main menu window

    // Create derived window in the middle of the main window
    menu_subwin = derwin(menu_window, 0, 0, sub_max_y / 4, sub_max_x / 4);
   // box(menu_subwin, 0, 0);

    set_menu_sub(power_menu, menu_subwin);          // Set power menu subwindow
    post_menu(power_menu);                          // Display power menu

    /* Handle input */
    while (input = wgetch(menu_window)) {
        switch (input) {
            case '\n':
            case '\r':
            case KEY_ENTER:
                enter_pressed = true;
                break;
            case KEY_UP:
            case KEY_LEFT:
            case 'k':
            case 'h':
                menu_driver(power_menu, REQ_PREV_ITEM);
                break;
            case KEY_DOWN:
            case KEY_RIGHT:
            case 'j':
            case 'l':
                menu_driver(power_menu, REQ_NEXT_ITEM);
                break;
            case 27: // 27 is the raw value of ESC since there is no KEY macro
                unload_menu();
                endwin();
                printf("Cancelled.\n");
                return 0;
        }

        if (enter_pressed) { break; } // Break free of while loop
    }
    /* ------------ */

    selected_option_index = item_index(current_item(power_menu)); // Get the index of the current option

    unload_menu();
    endwin();

    // TODO Add confirmation dialogue

    /* Handle selected menu option */
    switch (selected_option_index) {
        case 0: // Shutdown
            printf("Shutting down...\n");

#if defined(PLATFORM_LINUX)
            system("shutdown -P now");
#elif defined(PLATFORM_BSD)
            system("shutdown -p now");
#endif
            break;
        case 1: // Reboot
            printf("Rebooting...\n");
            system("shutdown -r now");      // Works for both Linux and BSD
            break;
        case 2: // Suspend
            printf("Suspending...\n");
#if defined(PLATFORM_LINUX)
            /* Try several ways of suspending */
            if (system("systemctl suspend > /dev/null 2>&1") == 0) {
                return 0;
            } else if (system("loginctl suspend > /dev/null 2>&1") == 0) {
                return 0;
            } else if (system("pm-suspend > /dev/null 2>&1") == 0) {
                return 0;
            } else {
                printf("Suspend not supported\n");
            }
            /* ------------------------------ */
#elif defined(PLATFORM_BSD)
            system("zzz");
#endif
            break;
        case 3: // Cancel
            printf("Cancelled.\n");
            return 0;
    }
    /* --------------------------- */

    return 0;
}

/* Extra TODO:
 * - Add Windows support
 * - Potentially use execvp() instead of system()
 * - Allow for configuration options */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <menu.h>

// TODO Pull VERSION from flake.nix
#define VERSION "1.0.1"

/* Determine platform at compilation */
#if defined(__linux__)
#define PLATFORM_LINUX
#elif defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD)
#define PLATFORM_BSD
#endif
/* --------------------------------- */

/* Declare global variables */
int selected_option_index   = 3;    // Default to 3 for cancel
int input                   = 0;
int i                       = 0;
int max_x                   = 0;
int max_y                   = 0;
int sub_max_x               = 0;
int sub_max_y               = 0;

bool enter_pressed          = false;

char *options[] = { "Shutdown", "Reboot", "Suspend", "Cancel" };
int option_count = sizeof(options) / sizeof(char *);    // Determine number of array entries
int longest_option_char_count;
/* ------------------------ */

/* Declare menu variables */
ITEM **items;
MENU *power_menu;
MEVENT mouse_event;
WINDOW *menu_window;
WINDOW *menu_subwin;
/* ---------------------- */

int main(int argc, char *argv[]) {
    // TODO Un-hardcode this
    longest_option_char_count = strlen(options[0]); // Get length of longest menu option

    /* Initialise ncurses */
    set_escdelay(50); // Set delay for escape key
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    /* ------------------ */

    getmaxyx(stdscr, max_y, max_x); // Get size of terminal window

    // TODO Allow for mouse usage

    // Allocate memory for array of pointers to ITEM, zero-initialising everything so the last item is NULL
    items = calloc(option_count + 1, sizeof(ITEM *));

    /* Create menu items from options array */
    for (i; i < option_count; ++i) {
        items[i] = new_item(options[i], options[i]);
    }
    items[option_count] = (ITEM *)NULL; // Terminate option list with null pointer
    /* ------------------------------------ */

    /* Create main window for menu */
    power_menu = new_menu((ITEM **)items);  // Create menu based off items
    menu_opts_off(power_menu, O_NONCYCLIC); // Force enable menu wrapping
    menu_opts_off(power_menu, O_SHOWDESC); // Disable item descriptions
    set_menu_mark(power_menu, ">");         // Set menu marker

    // Create main menu window using size of power menu + padding
    menu_window = newwin(option_count + 4, longest_option_char_count + 10, max_y / 2 - option_count, max_x / 2 - longest_option_char_count);

    keypad(menu_window, TRUE);
    box(menu_window, 0, 0);
    /* ---------------------- */

    getmaxyx(menu_window, sub_max_y, sub_max_x);    // Get size of main menu window
    set_menu_win(power_menu, menu_window);          // Assign power menu to the main menu window

    // Create derived window in the middle of the main window
    menu_subwin = derwin(menu_window, 0, 0, sub_max_y / 4, sub_max_x / 4);

    mvwprintw(menu_window, 0, 2, "pmenu %s", VERSION);  // Window titlebar
    set_menu_sub(power_menu, menu_subwin);              // Set power menu subwindow
    post_menu(power_menu);                              // Display power menu

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
                printf("Cancelled.\n");
                goto cleanup;
        }

        if (enter_pressed) { break; } // Break free of while loop
    }
    /* ------------ */

    selected_option_index = item_index(current_item(power_menu));   // Get the index of the current option

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
                break;
            } else if (system("loginctl suspend > /dev/null 2>&1") == 0) {
                break;
            } else if (system("pm-suspend > /dev/null 2>&1") == 0) {
                break;
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
            goto cleanup;
    }
    /* --------------------------- */


    /* Free up memory used by the menu */
cleanup:
    unpost_menu(power_menu);
    free_menu(power_menu);
    for (i = 0; i < (option_count + 1); ++i) {
        free_item(items[i]);
    }
    free(items);
    delwin(menu_subwin);
    delwin(menu_window);
    endwin();
    /* ------------------------------- */

    return 0;
}

/* Extra TODO:
 * - Properly refresh window so resizing doesn't break it
 * - Add Windows support
 * - Potentially use execvp() instead of system()
 * - Allow for configuration options */

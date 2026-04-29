#include <stdio.h>
#include <stdlib.h>
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
int startx                  = 0;
int starty                  = 0;
int rows                    = 0;
int cols                    = 0;
int input                   = 0;
int i                       = 0;

bool enter_pressed          = false;

char *options[] = { "Shutdown", "Reboot", "Suspend", "Cancel" };
int option_count = sizeof(options) / sizeof(char *);    // Determine number of array entries
/* ------------------------ */

/* Declare menu variables */
ITEM **items;
MENU *power_menu;
MEVENT mouse_event;
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

    // TODO Center the menu
    /* Move menu to the middle of the terminal */
    /* --------------------------------------- */

    // TODO Add a border around the menu
    // TODO Allow for mouse usage
    power_menu = new_menu((ITEM **)items);  // Create menu based off items
    menu_opts_off(power_menu, O_NONCYCLIC); // Force enable menu wrapping
    set_menu_mark(power_menu, ">");         // Set menu marker
    post_menu(power_menu);                  // Display power menu

    refresh();                                  

    /* Handle input */
    while (input = getch()) {
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <menu.h>

#include "pmenu.h"
#include "args.h"

int input, max_x, max_y, menu_win_max_x, menu_win_max_y = 0;

/* Declare global variables */
int selected_option_index       = 3;    // Default to 3 for cancel

char    *options[]                  = { "Shutdown", "Reboot", "Suspend", "Cancel" };
int     option_count                = sizeof(options) / sizeof(char *);
int     longest_option_char_count   = 0;

bool enter_pressed, choice_confirmed = false;
/* ------------------------ */

/* Declare menu variables */
ITEM    **items;
MENU    *power_menu;
WINDOW  *menu_window;
WINDOW  *menu_subwin;
MEVENT  mouse_event;

ITEM    **confirm_items;
MENU    *confirm_menu;
WINDOW  *confirm_menu_subwin;
/* ---------------------- */

/* Get version from file */
const char version[] = {
#include "version.txt"
};
/* --------------------- */

int get_longest_option_char_count() {
    int current, longest = 0;

    for (int i = 0; i < option_count; ++i) { // For each option
        current = strlen(options[i]);

        if (current > longest) {
            longest = current;
        }
    }

    return longest;
}

void cleanup() {
    /* Free memory used by main menu */
    unpost_menu(power_menu);
    free_menu(power_menu);
    for (int i = 0; i < (option_count + 1); ++i) {
        free_item(items[i]);
    }
    free(items);
    /* ----------------------------- */

    /* Free confirm menu memory */
    unpost_menu(confirm_menu);
    free_menu(confirm_menu);

    for (int i = 0; i < 3; ++i) {
        free_item(confirm_items[i]);
    }
    free(confirm_items);
    /* ------------------------ */

    delwin(menu_subwin);
    delwin(menu_window);
}

void cancel_and_exit(int exit_code) {
    cleanup();
    endwin();
    printf("Cancelled.\n");
    exit(exit_code);
}

int get_user_selection_index(WINDOW *window_to_interface_with, MENU *menu_to_interface_with) {
    menu_driver(menu_to_interface_with, REQ_FIRST_ITEM);

    /* Handle input */
    while ((input = wgetch(window_to_interface_with))) {
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
                menu_driver(menu_to_interface_with, REQ_PREV_ITEM);
                break;
            case KEY_DOWN:
            case KEY_RIGHT:
            case 'j':
            case 'l':
                menu_driver(menu_to_interface_with, REQ_NEXT_ITEM);
                break;
            case 27: // 27 is the raw value of ESC since there is no KEY macro
                cancel_and_exit(EXIT_SUCCESS);
            case 49: // Raw value for '1'
                if (NUM_SELECT) { return 0; }
            case 50: // Raw value for '2'
                if (NUM_SELECT) { return 1; }
            case 51: // Raw value for '3'
                if (NUM_SELECT) { return 2; }
            case 52: // Raw value for '4'
                if (NUM_SELECT) { cancel_and_exit(EXIT_SUCCESS); }
        }

        if (enter_pressed) { break; } // Break free of while loop
    }
    /* ------------ */

    enter_pressed = false; // Revert enter state
    return item_index(current_item(menu_to_interface_with)); // Return index of the selected option
}

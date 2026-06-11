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
int     selected_option_index       = 3;    // Default to 3 for cancel

char    *options[]                  = { "Shutdown", "Reboot", "Suspend", "Cancel" };
int     option_count                = sizeof(options) / sizeof(char *);
int     longest_option_char_count   = 0;

bool    enter_pressed, choice_confirmed = false;
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

        if (current > longest) { longest = current; }
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
            /* Keyboard inputs */
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
            case '1':
                if (NUM_SELECT) { return 0; }
                break;
            case '2':
                if (NUM_SELECT) { return 1; }
                break;
            case '3':
                if (menu_to_interface_with == confirm_menu) { break; }
                if (NUM_SELECT) { return 2; }
                break;
            case '4':
                if (menu_to_interface_with == confirm_menu) { break; }
                if (NUM_SELECT) { return 3; }
                break;
            case 27: // 27 is the raw value of ESC
                return 3;
                break;
                /* --------------- */

                /* Mouse inputs */
            case KEY_MOUSE:
                if(getmouse(&mouse_event) == OK) {
                    int local_y = mouse_event.y;
                    int local_x = mouse_event.x;

                    // Get mouse position relative to subwindow
                    if (wmouse_trafo(menu_subwin, &local_y, &local_x, FALSE)) {
                        if(mouse_event.bstate & BUTTON1_CLICKED) {
                            switch (local_y) {
                                case 0:
                                    return 0;
                                    break;
                                case 1:
                                    return 1;
                                    break;
                                case 2:
                                    if (menu_to_interface_with == confirm_menu) { break; }
                                    return 2;
                                    break;
                                case 3:
                                    if (menu_to_interface_with == confirm_menu) { break; }
                                    return 3;
                                    break;
                            }
                        }
                    }
                }
        }
        /* ------------ */

        if (enter_pressed) { break; } // Break free of while loop
    }
    /* ------------ */

    enter_pressed = false; // Revert enter state
    return item_index(current_item(menu_to_interface_with)); // Return index of the selected option
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <menu.h>

// Flags
bool TEST_MODE, NUM_SELECT, DISPLAY_HORIZONTAL, NO_CONFIRM = false;

int input, max_x, max_y, menu_win_max_x, menu_win_max_y = 0;

/* Declare global variables */
int selected_option_index       = 3;    // Default to 3 for cancel

char *options[]                 = { "Shutdown", "Reboot", "Suspend", "Cancel" };
int option_count                = sizeof(options) / sizeof(char *);
int longest_option_char_count   = 0;

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

void show_help_message() {
    printf("Usage: pmenu [OPTIONS]...\n");
    printf("Valid arguments:\n"
            "     --help\t\tShow this help message.\n"
            "  -d --noconfirm\tDisable confirmation window.\n"
            "  -s --num-select\tEnable number key usage for menu options.\n"
            "  -t --testing\t\tEnable testing mode (disables actual menu functions).\n"
            "  -v --version\t\tShow current version.\n"
            "\nNot yet implemented (planned):\n"
            "  -n --show-nums\tDisplay numbers before menu entries\n");
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

void set_flags(int argc, char *argv[]) {
    // TODO Match exact input to prevent cases like "-help" enabling DISPLAY_HORIZONTAL

    for (int i = 1; i < argc; ++i) { // Start 'i' at 1 because argv[0] = current binary path
        if (strstr(argv[i], "--help")) {
            show_help_message();
            exit(EXIT_SUCCESS);
        }
        else if (strstr(argv[i], "-v") || strstr(argv[i], "--version")) {
            printf("pmenu version %s\n", version);
            exit(0);
        }
        else if (strstr(argv[i], "-t") || strstr(argv[i], "--testing")) {
            TEST_MODE = true;
            printf("Testing mode enabled.\n");
        }
        else if (strstr(argv[i], "-h") || strstr(argv[i], "--horizontal")) {
            DISPLAY_HORIZONTAL = true;
            printf("Horizontal menu enabled.\n");
        }
        else if (strstr(argv[i], "-d") || strstr(argv[i], "--noconfirm")) {
            NO_CONFIRM = true;
            printf("Confirmation window disabled.\n");
        }
        else if (strstr(argv[i], "-s") || strstr(argv[i], "--num-select")) {
            NUM_SELECT = true;
            printf("Number selection enabled.\n");
        }
        else {
            show_help_message();
            exit(0);
        }
    }
}

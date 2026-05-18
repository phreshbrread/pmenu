#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <menu.h>

#include "pmenu.h"

// Flags
bool TEST_MODE, NUM_SELECT, DISPLAY_HORIZONTAL, NO_CONFIRM = false;

/* Determine platform at compilation */
#if defined(__linux__)
#define PLATFORM_LINUX
#elif defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD)
#define PLATFORM_BSD
#endif
/* --------------------------------- */

/* Declare global variables */
int selected_option_index   = 3;    // Default to 3 for cancel

int i, input, max_x, max_y, menu_win_max_x, menu_win_max_y = 0;

char *options[]                 = { "Shutdown", "Reboot", "Suspend", "Cancel" };
int option_count                = sizeof(options) / sizeof(char *);
int longest_option_char_count   = 0;

bool enter_pressed, choice_confirmed = false;
/* ------------------------ */

/* Get version from file */
const char version[] = {
#include "version.txt"
};
/* --------------------- */

/* Declare menu variables */
ITEM **items;
MENU *power_menu;
MEVENT mouse_event;
WINDOW *menu_window;
WINDOW *menu_subwin;

ITEM **confirm_items;
MENU *confirm_menu;
/* ---------------------- */

void cleanup() {
    /* Free memory used by main menu */
    unpost_menu(power_menu);
    free_menu(power_menu);
    for (i = 0; i < (option_count + 1); ++i) {
        free_item(items[i]);
    }
    free(items);
    /* ----------------------------- */

    /* Free confirm menu memory */
    unpost_menu(confirm_menu);
    free_menu(confirm_menu);

    for (i = 0; i < 3; ++i) {
        free_item(confirm_items[i]);
    }
    free(confirm_items);
    /* ------------------------ */

    delwin(menu_subwin);
    delwin(menu_window);
}

void cancel_and_exit() {
    cleanup();
    endwin();
    printf("Cancelled.\n");
    exit(0);
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
                cancel_and_exit();
            case 49: // Raw value for '1'
                if (NUM_SELECT) { return 0; }
            case 50: // Raw value for '2'
                if (NUM_SELECT) { return 1; }
            case 51: // Raw value for '3'
                if (NUM_SELECT) { return 2; }
            case 52: // Raw value for '4'
                if (NUM_SELECT) { cancel_and_exit(); }
        }

        if (enter_pressed) { break; } // Break free of while loop
    }
    /* ------------ */

    enter_pressed = false; // Revert enter state
    return item_index(current_item(menu_to_interface_with)); // Return index of the selected option
}

void set_flags(int argc, char *argv[]) {
    // TODO Match exact input to prevent cases like "-help" enabling DISPLAY_HORIZONTAL

    for (i = 1; i < argc; ++i) { // Start 'i' at 1 because argv[0] = current binary path
        if (strstr(argv[i], "--help")) {
            show_help_message();
            exit(0);
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

int main(int argc, char *argv[]) {
    if (argc > 1) {
        set_flags(argc, argv);
    }

    /* Get length of longest option */
    // TODO Un-hardcode this
    longest_option_char_count = strlen(options[0]); // Get length of longest menu option
    /* ---------------------------- */

    /* Initialise ncurses */
    set_escdelay(50); // Set delay for escape key
    initscr();
    curs_set(0);
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    /* ------------------ */

    getmaxyx(stdscr, max_y, max_x); // Get size of terminal window

    // TODO Allow for mouse usage

    // Allocate memory for array of pointers to ITEM, zero-initialising everything so the last item is NULL
    items = calloc(option_count + 1, sizeof(ITEM *));

    /* Create menu items from options array */
    for (i = 0; i < option_count; ++i) {
        items[i] = new_item(options[i], options[i]);
    }
    items[option_count] = (ITEM *)NULL; // Terminate option list with null pointer

    power_menu = new_menu((ITEM **)items);  // Create menu based off items
    menu_opts_off(power_menu, O_NONCYCLIC); // Force enable menu wrapping
    menu_opts_off(power_menu, O_SHOWDESC);  // Disable item descriptions
    set_menu_mark(power_menu, ">");         // Set menu marker
    /* ------------------------------------ */

    /* Create confirmation menu */
    confirm_items = calloc(3, sizeof(ITEM *));
    confirm_items[0] = new_item("Yes",  NULL);
    confirm_items[1] = new_item("No",   NULL);
    confirm_items[2] = (ITEM *)NULL;

    confirm_menu = new_menu((ITEM **)confirm_items);
    menu_opts_off(confirm_menu, O_NONCYCLIC); // Force enable menu wrapping
    menu_opts_off(confirm_menu, O_SHOWDESC);  // Disable item descriptions
    set_menu_mark(confirm_menu, ">");         // Set menu marker
    /* ------------------------ */

    /* Create main window for menu */
    // Create window using size of menu + padding
    menu_window = newwin(option_count + 4, longest_option_char_count + 10, max_y / 2 - option_count, max_x / 2 - longest_option_char_count);

    keypad(menu_window, TRUE);
    /* ---------------------- */

    getmaxyx(menu_window, menu_win_max_y, menu_win_max_x);  // Get size of main menu window
    set_menu_win(power_menu, menu_window);                  // Assign power menu to the main menu window

    // Create derived window in the middle of the main window
    menu_subwin = derwin(menu_window, 0, 0, menu_win_max_y / 4, menu_win_max_x / 4);
    set_menu_sub(power_menu, menu_subwin); // Set power menu subwindow

    // TODO Fix confirm subwindow positioning
    WINDOW *confirm_menu_subwin = derwin(menu_window, 0, 0, menu_win_max_y / 4, menu_win_max_x / 3 + 1);

    set_menu_win(confirm_menu, menu_window);
    set_menu_sub(confirm_menu, confirm_menu_subwin);
    keypad(confirm_menu_subwin, TRUE);

    if(TEST_MODE) {
        printw("TEST MODE");
        refresh();
    }

    while (!choice_confirmed) {
        box(menu_window, 0, 0);
        mvwprintw(menu_window, 0, 2, "pmenu %s", version);  // Window titlebar
        post_menu(power_menu);                              // Display power menu

        selected_option_index = get_user_selection_index(menu_window, power_menu);  // Handle input for main menu
        if(selected_option_index == 3) { choice_confirmed = true; }                 // If cancel then confirm

        unpost_menu(power_menu);
        wrefresh(menu_window);

        /* Confirmation */
        if (NO_CONFIRM) { choice_confirmed = true; }

        if (!choice_confirmed) {
            post_menu(confirm_menu);
            box(menu_window, 0, 0); // Re-draw box around main window

            if (get_user_selection_index(confirm_menu_subwin, confirm_menu) == 0) {
                choice_confirmed = true;
            }
        }
        /* ------------ */
    }

    endwin();

    /* Handle selected menu option */
    switch (selected_option_index) {
        case 0:
            printf("Shutting down...\n");
            if (TEST_MODE) { return 0; }

#if defined(PLATFORM_LINUX)
            // TODO Try several different shutdown options since Gentoo and Guix use different commands
            system("shutdown -P now");
#elif defined(PLATFORM_BSD)
            system("shutdown -p now");
#endif
            break;
        case 1:
            printf("Rebooting...\n");
            if (TEST_MODE) { return 0; }

            system("shutdown -r now"); // Works for both Linux and BSD
            break;
        case 2: // Suspend
            printf("Suspending...\n");
            if (TEST_MODE) { return 0; }

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
        case 3:
            cancel_and_exit();
    }
    /* --------------------------- */

    cleanup();
    return 0;
}

/* Extra TODO:
 * - Properly refresh window so resizing doesn't break it
 * - Add Windows support
 * - Allow for configuration options */

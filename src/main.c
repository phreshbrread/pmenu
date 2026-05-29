#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <menu.h>

#include "pmenu.h"
#include "args.h"

/* Determine platform at compilation */
#if defined(__linux__)
#define PLATFORM_LINUX
#elif defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD)
#define PLATFORM_BSD
#endif
/* --------------------------------- */

int main(int argc, char *argv[]) {
    if (argc > 1) {
        set_flags(argc, argv);
    }

    if (SHOW_HELP) {
        show_help_message();
        exit(EXIT_SUCCESS);
    }

    if (SHOW_VERSION) {
        printf("pmenu version %s\n", version);
        exit(EXIT_SUCCESS);
    }

    /* Get length of longest option */
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

    // Allocate memory for array of pointers to ITEM, zero-initialising everything so the last item is NULL
    items = calloc(option_count + 1, sizeof(ITEM *));

    /* Create menu items from options array */
    for (int i = 0; i < option_count; ++i) {
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

    confirm_menu_subwin = derwin(menu_window, 0, 0, menu_win_max_y / 4, menu_win_max_x / 3 - 2);

    set_menu_win(confirm_menu, menu_window);
    set_menu_sub(confirm_menu, confirm_menu_subwin);
    keypad(confirm_menu_subwin, TRUE);

    if(TEST_MODE) {
        printw("TEST MODE");
        refresh();
    }

    while (!choice_confirmed) {
        box(menu_window, 0, 0);

        mvwprintw(menu_window, 0, 2, "pmenu %s", version);    // Window title
        post_menu(power_menu);                                // Display power menu

        selected_option_index = get_user_selection_index(menu_window, power_menu);  // Handle input for main menu
        if(selected_option_index == 3) { choice_confirmed = true; }                 // If cancel then confirm

        unpost_menu(power_menu);
        wrefresh(menu_window);

        /* Confirmation */
        if (NO_CONFIRM) { choice_confirmed = true; }

        if (!choice_confirmed) {
            post_menu(confirm_menu);
            box(menu_window, 0, 0); // Re-draw box around main window
            mvwprintw(menu_window, 0, 2, "Are you sure?");    // Window title
            wrefresh(menu_window);

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
            if (TEST_MODE) { return EXIT_SUCCESS; }

#if defined(PLATFORM_LINUX)
            if (system("shutdown -P now > /dev/null 2>&1") == 0) {          // Works for most distros
                break;
            } else if (system("shutdown -h now > /dev/null 2>&1") == 0) {   // Gentoo
                break;
            } else if (system("herd power-off root") == 0) {                // Guix
                break;
            } else {
                printf("Failed to shut down\n");
                return EXIT_FAILURE;
            }
#elif defined(PLATFORM_BSD)
            system("shutdown -p now");
#endif
            break;
        case 1:
            printf("Rebooting...\n");
            if (TEST_MODE) { return EXIT_SUCCESS; }

            if (system("shutdown -r now > /dev/null 2>&1") != 0) { // Works for both Linux and BSD
                printf("Failed to reboot\n");
                exit(EXIT_FAILURE);
            };
            break;
        case 2: // Suspend
            printf("Suspending...\n");
            if (TEST_MODE) { return EXIT_SUCCESS; }

#if defined(PLATFORM_LINUX)
            /* Try several ways of suspending */
            if (system("systemctl suspend > /dev/null 2>&1") == 0) {
                break;
            } else if (system("loginctl suspend > /dev/null 2>&1") == 0) {
                break;
            } else if (system("pm-suspend > /dev/null 2>&1") == 0) {
                break;
            } else {
                cleanup();
                printf("Suspend not supported\n");
                return EXIT_FAILURE;
            }
            /* ------------------------------ */
#elif defined(PLATFORM_BSD)
            system("zzz");
#endif
            break;
        case 3:
            cancel_and_exit(EXIT_SUCCESS);
    }
    /* --------------------------- */

    cleanup();
    return EXIT_SUCCESS;
}

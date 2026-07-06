#ifndef PMENU_H
#define PMENU_H
#include <ncurses.h>
#include <menu.h>

/* Variables */
extern bool enter_pressed, choice_confirmed;

extern int input, max_x, max_y, menu_win_max_x, menu_win_max_y, selected_option_index, longest_option_char_count;

extern const int option_count;

extern char *options[];

extern const char version[];
/* --------- */

/* Menu variables */
extern ITEM **items, **confirm_items;
extern MENU *power_menu, *confirm_menu;
extern WINDOW *menu_window, *menu_subwin, *confirm_menu_subwin;
extern MEVENT mouse_event;
/* -------------- */

/* Functions */
int get_longest_option_char_count();

void show_help_message();

void cleanup();

void cancel_and_exit(int exit_code);

int get_user_selection_index(WINDOW *window_to_interface_with, MENU *menu_to_interface_with);

void set_flags(int argc, char *argv[]);
/* --------- */

#endif

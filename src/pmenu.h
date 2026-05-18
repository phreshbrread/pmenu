#ifndef PMENU_H
#define PMENU_H

void show_help_message();

void cancel_and_exit();

int get_user_selection_index(WINDOW *window_to_interface_with, MENU *menu_to_interface_with);

void set_flags(int argc, char *argv[]);

#endif

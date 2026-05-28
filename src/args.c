#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <menu.h>

#include "args.h"
#include "pmenu.h"

// Args
Argument valid_args[] = {
    { "",       "--help",       "Show this help message.",                      false },
    { "-d",     "--noconfirm",  "Disable confirmation window.",                 false },
    { "-s",     "--num-select", "Enable number key usage for menu options.",    false },
    { "-t",     "--testing",    "Disable menu functions.",                      false },
    { "-v",     "--version",    "Show current version.",                        false },
    { "-n",     "--show-nums",  "Display numbers before menu entries.",         false },
};

int valid_args_count = sizeof(valid_args) / sizeof(valid_args[0]);

// TODO Base output off of valid_args[]
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

void set_flags(int argc, char **argv) {
    int activated_flags = 0;

    for (int i = 1; i < argc; ++i) { // For each entry in argv[]
        for (int j = 0; j < valid_args_count; ++j) { // For each valid argument
            if (strcmp(valid_args[j].shorthand, argv[i]) == 0 || strcmp(valid_args[j].longhand, argv[i]) == 0) {
                valid_args[j].flag_enabled = true;
                ++activated_flags;
            }
        }
    }

    if (argc > 1 && activated_flags != argc - 1) {
        show_help_message();
        exit(EXIT_FAILURE);
    }
}



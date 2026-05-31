#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <menu.h>

#include "args.h"
#include "pmenu.h"

// Args
Argument valid_args[] = {
    { "  ",     "--help",       "Show this help message.",                      false },
    { "-d",     "--noconfirm",  "Disable confirmation window.",                 false },
    { "-s",     "--num-select", "Enable number key usage for menu options.",    false },
    { "-t",     "--testing",    "Disable menu functions.",                      false },
    { "-v",     "--version",    "Show current version.",                        false },
    //{ "-n",     "--show-nums ",  "Display numbers before menu entries.",         false },
};

int valid_args_count = sizeof(valid_args) / sizeof(valid_args[0]);

void show_help_message() {
    printf("Usage: pmenu [OPTIONS]...\n");
    printf("Valid arguments:\n");
    for (int i = 0; i < valid_args_count; ++i) {
        printf("   %-3s\t%-15s %s\n", valid_args[i].shorthand, valid_args[i].longhand, valid_args[i].description);
    };
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

#include <stdio.h>

void show_help_message() {
    printf("Valid arguments:\n"
            "     --help\t\tShow this help message.\n"
            "  -d --noconfirm\tDisable confirmation window.\n"
            "  -s --num-select\tEnable number key usage for menu options.\n"
            "  -t --testing\t\tEnable testing mode (disables actual menu functions).\n"
            "  -v --version\t\tShow current version.\n"
            "\nNot yet implemented (planned):\n"
            "  -h --horizontal\tSet menu to display horizontally rather than vertically.\n"
            "  -n --show-nums\tDisplay numbers before menu entries\n");
}

#ifndef ARGS_H
#define ARGS_H

// Flags
extern bool SHOW_HELP, NO_CONFIRM, NUM_SELECT, TEST_MODE, SHOW_VERSION, SHOW_NUMS;

typedef struct {
    char shorthand[3];
    char longhand[64];
    char description[512];
    bool *flag_enabled;
} Argument;

extern Argument valid_args[];

extern void show_help_message();

extern void set_flags(int argc, char *argv[]);

#endif

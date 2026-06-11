#ifndef ARGS_H
#define ARGS_H

// Flag macros
#define SHOW_HELP       valid_args[0].flag_enabled
#define NO_CONFIRM      valid_args[1].flag_enabled
#define NUM_SELECT      valid_args[2].flag_enabled
#define TEST_MODE       valid_args[3].flag_enabled
#define SHOW_VERSION    valid_args[4].flag_enabled
#define SHOW_NUMS       valid_args[5].flag_enabled

typedef struct {
    char shorthand[3];
    char longhand[64];
    char description[512];
    bool flag_enabled;
} Argument;

extern Argument valid_args[];

extern void show_help_message();

extern void set_flags(int argc, char *argv[]);

#endif

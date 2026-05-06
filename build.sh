#! /bin/sh

gcc -Wall -Wextra -Wpedantic --std=c99 -ltinfo -lmenu -lncurses ./src/main.c -o pmenu

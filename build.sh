#! /bin/sh

gcc -Wall -Wextra -Wpedantic --std=c99 -lmenu -lncurses -ltinfo ./src/main.c -o pmenu

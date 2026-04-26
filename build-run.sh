#! /bin/sh

gcc -Wall -Wextra -Wpedantic -g --std=c99 -lmenu -lncurses -ltinfo ./src/main.c -o ./output/pmenu && ./output/pmenu

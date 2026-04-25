#! /bin/sh

gcc -g --std=c99 -lmenu -lncurses -ltinfo ./src/main.c -o ./output/pmenu && ./output/pmenu

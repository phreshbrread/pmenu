#include <stdio.h>
#include <ncurses.h>

int main() {
    initscr();
    printw("Greetings, friend.");
    refresh();
    getch();
    endwin();

    return 0;
}



/* NOTES
 *
 * Need ncurses
 * Open term in middle of screen windowed
 * Linux-BSD cross platform
 * Split into multiple files later
 * Init-sys agnostic
 * Find out init with function then use correct command
*/

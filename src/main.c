#include <curses.h>
#include <stdio.h>
#include <string.h>

#include "editor.h"
#include "file_io.h"

static char s_current_file[256] = {0};

int main(int argc, char *argv[])
{
    if (argc > 1) {
        strncpy(s_current_file, argv[1], sizeof(s_current_file) - 1);
        if (load_file(s_current_file) != 0) {
            printf("Could not open file: %s\n", s_current_file);
        }
    }

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    editor_init();

    int running = 1;
    while (running) {
        editor_refresh_screen();

        int ch = getch();

        if (ch == 19) {  // Ctrl+S
            int save_result;
            if (strlen(s_current_file) > 0) {
                save_result = save_file(s_current_file);
            } else {
                save_result = save_file("unnamed.txt");
            }
            if (save_result == 0) {
                editor_show_message("File saved successfully");
            } else {
                editor_show_message("Error: Save failed");
            }
        } else if (ch == 17) { // Ctrl+Q
            running = 0;
        } else {
            editor_process_key(ch);
        }
    }

    endwin();
    free_lines();
    return 0;
}

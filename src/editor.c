#include <curses.h>
#include <stdlib.h>
#include <string.h>

#include "editor.h"

char *g_lines[MAX_LINES];
int g_total_lines = 1;

int g_cursor_y = 0;
int g_cursor_x = 0;
int g_topLine = 0;

char g_status_right[] = "[Ctrl+S: Save, Ctrl+Q: Quit]";

void editor_init() {
    g_cursor_y = 0;
    g_cursor_x = 0;
    g_topLine = 0;
}

void editor_refresh_screen() {
    erase();

    int rows = getmaxy(stdscr);
    int cols = getmaxx(stdscr);

    for (int i = 0; i < rows - 1; i++) {
        int idx = g_topLine + i;
        if (idx < g_total_lines && g_lines[idx] != NULL) {
            char temp[MAX_COLS + 1];
            strncpy(temp, g_lines[idx], cols);
            temp[cols] = '\0';
            mvprintw(i, 0, "%s", temp);
        }
    }

    char left_status[256];
    snprintf(left_status, sizeof(left_status), "Cursor: (%d,%d), Lines: %d",
             g_cursor_y + 1, g_cursor_x + 1, g_total_lines);

    move(rows - 1, 0);
    clrtoeol();
    printw("%s", left_status);

    int right_len = (int) strlen(g_status_right);
    int start_col = cols - right_len;
    if (start_col < 0) {
        start_col = 0;
    }
    mvprintw(rows - 1, start_col, "%s", g_status_right);

    int screen_y = g_cursor_y - g_topLine;
    if (screen_y < 0) {
        screen_y = 0;
    } else if (screen_y >= rows - 1) {
        screen_y = rows - 2;
    }
    move(screen_y, g_cursor_x);

    refresh();
}

void editor_adjust_scroll() {
    int rows = getmaxy(stdscr);
    if (g_cursor_y < g_topLine) {
        g_topLine = g_cursor_y;
    } else if (g_cursor_y >= g_topLine + rows - 1) {
        g_topLine = g_cursor_y - (rows - 2);
    }
    if (g_topLine < 0) {
        g_topLine = 0;
    }
    if (g_topLine > g_total_lines - 1) {
        g_topLine = g_total_lines - 1;
    }
}

void editor_insert_char(int ch) {
    if (g_cursor_y < 0 || g_cursor_y >= MAX_LINES) {
        return;
    }
    if (!g_lines[g_cursor_y]) {
        g_lines[g_cursor_y] = malloc(1);
        g_lines[g_cursor_y][0] = '\0';
    }
    size_t len = strlen(g_lines[g_cursor_y]);
    if (g_cursor_x > len) {
        g_cursor_x = (int) len;
    }

    if (len + 2 > MAX_COLS) {
        return;
    }

    g_lines[g_cursor_y] = realloc(g_lines[g_cursor_y], len + 2);
    memmove(&g_lines[g_cursor_y][g_cursor_x + 1],
            &g_lines[g_cursor_y][g_cursor_x],
            len - g_cursor_x + 1);

    g_lines[g_cursor_y][g_cursor_x] = (char) ch;
    g_cursor_x++;
}

void editor_insert_newline() {
    if (g_cursor_y < 0 || g_cursor_y >= MAX_LINES) {
        return;
    }

    if (g_total_lines < MAX_LINES - 1) {
        g_total_lines++;
    }

    if (!g_lines[g_cursor_y]) {
        g_lines[g_cursor_y] = malloc(1);
        g_lines[g_cursor_y][0] = '\0';
        g_cursor_y++;
        g_cursor_x = 0;
        return;
    }

    size_t len = strlen(g_lines[g_cursor_y]);
    if (g_cursor_x > len) {
        g_cursor_x = (int) len;
    }

    char *split_text = &g_lines[g_cursor_y][g_cursor_x];

    size_t new_len = strlen(split_text);
    g_lines[g_cursor_y + 1] = malloc(new_len + 1);
    strcpy(g_lines[g_cursor_y + 1], split_text);

    g_lines[g_cursor_y][g_cursor_x] = '\0';

    g_cursor_y++;
    g_cursor_x = 0;
}

void editor_backspace() {
    if (g_cursor_y < 0 || g_cursor_y >= MAX_LINES) {
        return;
    }
    if (!g_lines[g_cursor_y]) {
        return;
    }

    if (g_cursor_x == 0) {
        if (g_cursor_y > 0) {
            size_t prev_len = strlen(g_lines[g_cursor_y - 1]);
            size_t curr_len = strlen(g_lines[g_cursor_y]);

            g_lines[g_cursor_y - 1] = realloc(g_lines[g_cursor_y - 1], prev_len + curr_len + 1);
            strcpy(&g_lines[g_cursor_y - 1][prev_len], g_lines[g_cursor_y]);

            free(g_lines[g_cursor_y]);
            for (int i = g_cursor_y; i < g_total_lines - 1; i++) {
                g_lines[i] = g_lines[i + 1];
            }
            g_lines[g_total_lines - 1] = NULL;
            g_total_lines--;

            g_cursor_y--;
            g_cursor_x = (int) prev_len;
        }
    } else {
        size_t len = strlen(g_lines[g_cursor_y]);
        if (g_cursor_x > len) {
            g_cursor_x = (int) len;
        }
        if (g_cursor_x == 0) {
            return;
        }

        memmove(&g_lines[g_cursor_y][g_cursor_x - 1],
                &g_lines[g_cursor_y][g_cursor_x],
                len - (g_cursor_x - 1));
        g_cursor_x--;
    }
}

void editor_process_key(int ch) {
    switch (ch) {
        case KEY_UP:
            if (g_cursor_y > 0) {
                g_cursor_y--;
                size_t len = strlen(g_lines[g_cursor_y]);
                if (g_cursor_x > len) {
                    g_cursor_x = (int) len;
                }
            }
            break;
        case KEY_DOWN:
            if (g_cursor_y < g_total_lines - 1) {
                g_cursor_y++;
                if (g_lines[g_cursor_y]) {
                    size_t l2 = strlen(g_lines[g_cursor_y]);
                    if (g_cursor_x > l2) g_cursor_x = (int) l2;
                } else {
                    g_cursor_x = 0;
                }
            }
            break;
        case KEY_LEFT:
            if (g_cursor_x > 0) {
                g_cursor_x--;
            } else {
                if (g_cursor_y > 0) {
                    g_cursor_y--;
                    g_cursor_x = (int) strlen(g_lines[g_cursor_y]);
                }
            }
            break;
        case KEY_RIGHT: {
            if (!g_lines[g_cursor_y]) break;
            size_t l3 = strlen(g_lines[g_cursor_y]);
            if (g_cursor_x < l3) {
                g_cursor_x++;
            } else {
                if (g_cursor_y < g_total_lines - 1) {
                    g_cursor_y++;
                    g_cursor_x = 0;
                }
            }
        }
            break;
        case 10:
            // intentional fallthrough
        case KEY_ENTER:
            editor_insert_newline();
            break;
        case 8:
            // intentional fallthrough
        case 127:
            // intentional fallthrough
        case KEY_BACKSPACE:
            editor_backspace();
            break;
        default:
            if (ch >= 32 && ch < 127) {
                editor_insert_char(ch);
            }
            break;
    }
    editor_adjust_scroll();
}

void editor_show_message(const char *msg) {
    char old_right[256];
    strncpy(old_right, g_status_right, sizeof(old_right) - 1);
    old_right[sizeof(old_right) - 1] = '\0';

    strncpy(g_status_right, msg, sizeof(g_status_right) - 1);
    g_status_right[sizeof(g_status_right) - 1] = '\0';

    editor_refresh_screen();

    napms(1000);

    strncpy(g_status_right, "[Ctrl+S: Save, Ctrl+Q: Quit]", sizeof(g_status_right) - 1);
    g_status_right[sizeof(g_status_right) - 1] = '\0';

    editor_refresh_screen();
}

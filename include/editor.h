#ifndef EDITOR_H
#define EDITOR_H

#define MAX_LINES (1000)
#define MAX_COLS  (1024)

extern char *g_lines[MAX_LINES];
extern int g_total_lines;

extern int g_cursor_y;
extern int g_cursor_x;
extern int g_topLine;

extern char g_status_right[];

void editor_init();
void editor_refresh_screen();
void editor_adjust_scroll();
void editor_insert_char(int ch);
void editor_insert_newline();
void editor_backspace();
void editor_process_key(int ch);
void editor_show_message(const char *msg);

#endif // EDITOR_H

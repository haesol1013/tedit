#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_io.h"
#include "editor.h"

int load_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error: Could not open file: %s\n", filename);
        return -1;
    }

    free_lines();

    char buffer[MAX_COLS];
    g_total_lines = 0;

    while (fgets(buffer, sizeof(buffer), fp)) {
        size_t len = strcspn(buffer, "\r\n");
        buffer[len] = '\0';

        if (len == 0) {
            continue;
        }

        g_lines[g_total_lines] = malloc(len + 1);
        if (g_lines[g_total_lines] == NULL) {
            fprintf(stderr, "Error: Memory allocation failed for line %d\n", g_total_lines);
            break;
        }
        strcpy(g_lines[g_total_lines], buffer);

        g_total_lines++;

        if (g_total_lines >= MAX_LINES) {
            fprintf(stderr, "Error: Reached maximum g_lines limit (%d)\n", MAX_LINES);
            break;
        }
    }
    fclose(fp);
    return 0;
}

int save_file(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        return -1;
    }
    for (int i = 0; i < g_total_lines; i++) {
        if (g_lines[i]) {
            fprintf(fp, "%s\n", g_lines[i]);
        } else {
            fprintf(fp, "\n");
        }
    }
    fclose(fp);
    return 0;
}

void free_lines() {
    for (int i = 0; i < MAX_LINES; i++) {
        if (g_lines[i]) {
            free(g_lines[i]);
            g_lines[i] = NULL;
        }
    }
    g_total_lines = 0;
}

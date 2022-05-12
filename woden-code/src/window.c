#include <gtksourceview/gtksourcebuffer.h>

#include "window.h"
#include "state.h"
#include "menu.h"
#include "text_view.h"
#include "terminal.h"

static void on_run(state_t*);
static void set_filename(state_t*, char*);
static void window_quit(GtkWidget*, gpointer);

static state_t state = {
    .filename = NULL,
    .window = NULL,
    .content = NULL,
    .terminal = NULL,
    .set_filename = set_filename,
    .on_run = on_run
};

extern GtkWidget* window_new(void) {
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(window_quit), NULL);

    state.window = window;
    state.content = gtk_source_buffer_new(NULL);
    state.terminal = gtk_text_buffer_new(NULL);

    gtk_window_set_title(GTK_WINDOW(window), WINDOW_TITLE);
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(box), menu_new(&state), FALSE, FALSE, 0);

    GtkWidget* scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view_new(&state));
    gtk_box_pack_start(GTK_BOX(box), scrolled_window, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(box), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL), FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(box), terminal_new(&state), FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), box);

    return window;
}

static void set_filename(state_t* state, char* filename) {
    if (state->filename != NULL) {
        free(state->filename);
    }

    state->filename = filename;

    if (filename == NULL) {
        gtk_window_set_title(GTK_WINDOW(state->window), WINDOW_TITLE " - <Untitled>");
    } else {
        char* title = malloc(strlen(filename) + strlen(WINDOW_TITLE) + 3);
        sprintf(title, "%s - '%s'", WINDOW_TITLE, filename);
        gtk_window_set_title(GTK_WINDOW(state->window), title);
    }
}

static void on_run(state_t* state) {
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(state->terminal, &start, &end);
    gtk_text_buffer_delete(state->terminal, &start, &end);

    on_save(NULL, (gpointer) state);

    char cmd[256] = "woden ";
    strcat(cmd, state->filename);
    FILE* compiler = popen(cmd, "r");

    char text[WINDOW_CMD_RES_MAX];
    int size;
    while (!feof(compiler)) {
        size = (int) fread(text, sizeof(char), WINDOW_CMD_RES_MAX, compiler);
    }
    gtk_text_buffer_insert_at_cursor(state->terminal, text, size);
    fclose(compiler);
}

static void window_quit(GtkWidget* widget, gpointer data) {
    gtk_main_quit();
}

#include "menu.h"

#define MENU_ITEMS_SIZE 2
#define MENU_ITEM_MAX_IDS 6
#define MAX_FILE_SIZE 2048000

typedef struct menu_item menu_item_t;
typedef struct submenu_item submenu_item_t;

struct submenu_item {
    const gchar* stock_id;
    void (*on_click)(GtkWidget*, gpointer);
};

struct menu_item {
    const gchar* mnemonic;
    size_t stock_ids_size;
    submenu_item_t items[MENU_ITEM_MAX_IDS];
};

static void on_open(GtkWidget*, gpointer);
static void on_save_as(GtkWidget*, gpointer);
static void on_close(GtkWidget*, gpointer);
static void on_quit(GtkWidget*, gpointer);

static void on_cut(GtkWidget*, gpointer);
static void on_copy(GtkWidget*, gpointer);
static void on_paste(GtkWidget*, gpointer);
static void on_delete(GtkWidget*, gpointer);

const menu_item_t menu_items[MENU_ITEMS_SIZE] = {
    {
        .mnemonic = "_File",
        .stock_ids_size = 6,
        .items = {
            { GTK_STOCK_NEW, on_close },
            { GTK_STOCK_OPEN, on_open },
            { GTK_STOCK_SAVE, on_save },
            { GTK_STOCK_SAVE_AS, on_save_as },
            { GTK_STOCK_CLOSE, on_close },
            { GTK_STOCK_QUIT, on_quit }
        }
    },
    {
        .mnemonic = "_Edit",
        .stock_ids_size = 4,
        .items = {
            { GTK_STOCK_CUT, on_cut },
            { GTK_STOCK_COPY, on_copy },
            { GTK_STOCK_PASTE, on_paste },
            { GTK_STOCK_DELETE, on_delete }
        }
    }
};

extern GtkWidget* menu_new(state_t* state) {
    GtkWidget* menu = gtk_menu_bar_new();

    for (size_t i = 0; i < MENU_ITEMS_SIZE; ++i) {
        GtkWidget* menu_item = gtk_menu_item_new_with_mnemonic(menu_items[i].mnemonic);
        gtk_container_add(GTK_CONTAINER(menu), menu_item);

        GtkWidget* submenu = gtk_menu_new();
        gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item), submenu);

        for (size_t j = 0; j < menu_items[i].stock_ids_size; ++j) {
            GtkWidget* submenu_item = gtk_image_menu_item_new_from_stock(menu_items[i].items[j].stock_id, NULL);
            g_signal_connect(G_OBJECT(submenu_item), "activate", G_CALLBACK(menu_items[i].items[j].on_click), state);
            gtk_container_add(GTK_CONTAINER(submenu), submenu_item);
        }
    }

    return menu;
}

static void on_open(GtkWidget* widget, gpointer data) {
    on_close(widget, data);
    state_t* state = (state_t*) data;

    GtkWidget* window = state->window;
    GtkWidget* dialog = gtk_file_chooser_dialog_new(
        "Open File...", GTK_WINDOW(window),
        GTK_FILE_CHOOSER_ACTION_OPEN,
        "_Cancel", GTK_RESPONSE_CANCEL,
        "_Open", GTK_RESPONSE_ACCEPT,
        NULL
    );

    gint res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        char* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        state->set_filename(state, filename);

        int bytes_read;
        FILE *fp = fopen(filename, "r");
        gchar text[MAX_FILE_SIZE];

        if (fp == NULL) exit (1);
        while (!feof(fp)) {
            bytes_read = fread(&text, sizeof(gchar), MAX_FILE_SIZE, fp);
        }
        fclose(fp);

        GtkTextIter start, end;
        gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(state->content), &start);
        gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(state->content), &end);
        gtk_text_buffer_delete(GTK_TEXT_BUFFER(state->content), &start, &end);
        gtk_text_buffer_insert(GTK_TEXT_BUFFER(state->content), &end, text, bytes_read);
    }
    gtk_widget_destroy(dialog);
}

extern void on_save(GtkWidget* widget, gpointer data) {
    state_t* state = (state_t*) data;

    if (state->filename == NULL) {
        return on_save_as(widget, data);
    }

    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(state->content), &start );
    gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(state->content), &end);
    gchar* text = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(state->content), &start, &end, TRUE);

    FILE* fp = fopen(state->filename, "w");
    if (!fp) return;

    fwrite(text, strlen(text), 1, fp);
    fclose(fp);
}

static void on_save_as(GtkWidget* widget, gpointer data) {
    state_t* state = (state_t*) data;

    GtkWidget* window = state->window;
    GtkWidget* dialog = gtk_file_chooser_dialog_new(
        "Save File As...", GTK_WINDOW(window),
        GTK_FILE_CHOOSER_ACTION_SAVE,
        "_Cancel", GTK_RESPONSE_CANCEL,
        "_Save", GTK_RESPONSE_ACCEPT,
        NULL
    );

    gint res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        char* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        state->set_filename(state, filename);

        GtkTextIter start, end;
        gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(state->content), &start);
        gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(state->content), &end);
        gchar* text = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(state->content), &start, &end, TRUE);

        FILE* fp = fopen(filename, "w");
        if (!fp) return;

        fwrite(text, strlen(text), 1, fp);
        fclose(fp);
    }
    gtk_widget_destroy(dialog);
}

static void on_close(GtkWidget* widget, gpointer data) {
    state_t* state = (state_t*) data;

    state->set_filename(state, NULL);
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(state->content), &start);
    gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(state->content), &end);
    gtk_text_buffer_delete(GTK_TEXT_BUFFER(state->content), &start, &end);
}

static void on_quit(GtkWidget* widget, gpointer data) {
    gtk_main_quit();
}

static void on_cut(GtkWidget* widget, gpointer data) {
    on_copy(widget, data);
    on_delete(widget, data);
}

static void on_copy(GtkWidget* widget, gpointer data) {
    state_t* state = (state_t*) data;
    gtk_text_buffer_copy_clipboard(GTK_TEXT_BUFFER(state->content), gtk_clipboard_get(GDK_SELECTION_CLIPBOARD));
}

static void on_paste(GtkWidget* widget, gpointer data) {
    state_t* state = (state_t*) data;
    gtk_text_buffer_paste_clipboard(GTK_TEXT_BUFFER(state->content), gtk_clipboard_get(GDK_SELECTION_CLIPBOARD), NULL, TRUE);
}

static void on_delete(GtkWidget* widget, gpointer data) {
    state_t* state = (state_t*) data;
    gtk_text_buffer_delete_selection(GTK_TEXT_BUFFER(state->content), FALSE, FALSE);
}

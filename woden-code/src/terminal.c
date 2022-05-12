#include "terminal.h"

static void on_run_click(GtkWidget*, gpointer);

extern GtkWidget* terminal_new(state_t* state) {
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    GtkWidget* toolbar = gtk_toolbar_new();
    gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_BOTH);

    GtkToolItem* button = gtk_tool_button_new(NULL, "Run");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_run_click), state);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), button, 0);

    GtkWidget* scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    GtkWidget* text_view = gtk_text_view_new_with_buffer(state->terminal);

    gtk_text_view_set_top_margin(GTK_TEXT_VIEW(text_view), TERMINAL_PADDING);
    gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(text_view), TERMINAL_PADDING);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(text_view), TERMINAL_PADDING);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(text_view), TERMINAL_PADDING);

    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);

    gtk_box_pack_start(GTK_BOX(box), toolbar, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), scrolled_window, TRUE, TRUE, 0);

    gtk_widget_set_size_request(box, 0, 200);

    return box;
}

static void on_run_click(GtkWidget* widget, gpointer data) {
    state_t* state = (state_t*) data;
    state->on_run(state);
}

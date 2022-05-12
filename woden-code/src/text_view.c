#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksourcebuffer.h>
#include <gtksourceview/gtksourcelanguagemanager.h>

#include "text_view.h"

extern GtkWidget* text_view_new(state_t* state) {
    GtkWidget* text_view = gtk_source_view_new_with_buffer(state->content);

    GtkSourceLanguageManager* language_manager = gtk_source_language_manager_new();
    GtkSourceLanguage* language = gtk_source_language_manager_get_language(language_manager, "groovy");

    gtk_source_buffer_set_language(state->content, language);
    gtk_source_buffer_set_implicit_trailing_newline(state->content, TRUE);
    gtk_source_buffer_set_highlight_syntax(state->content, TRUE);
    gtk_source_buffer_set_highlight_matching_brackets(state->content, TRUE);

    gtk_source_view_set_auto_indent(GTK_SOURCE_VIEW(text_view), TRUE);
    gtk_source_view_set_indent_width(GTK_SOURCE_VIEW(text_view), 4);
    gtk_source_view_set_show_line_numbers(GTK_SOURCE_VIEW(text_view), TRUE);
    gtk_source_view_set_show_line_marks(GTK_SOURCE_VIEW(text_view), TRUE);
    gtk_source_view_set_smart_backspace(GTK_SOURCE_VIEW(text_view), TRUE);


    gtk_text_view_set_top_margin(GTK_TEXT_VIEW(text_view), TEXT_VIEW_PADDING);
    gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(text_view), TEXT_VIEW_PADDING);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(text_view), TEXT_VIEW_PADDING);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(text_view), TEXT_VIEW_PADDING);

    return text_view;
}

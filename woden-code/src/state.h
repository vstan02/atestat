#ifndef WCODE_STATE_H
#define WCODE_STATE_H

#include <gtk/gtk.h>
#include <gtksourceview/gtksourceview.h>

typedef struct state state_t;

struct state {
    char* filename;
    GtkWidget* window;
    GtkSourceBuffer* content;
    GtkTextBuffer* terminal;
    void (*set_filename)(state_t*, char*);
    void (*on_run)(state_t*);
};

#endif // WCODE_STATE_H

#include <gtk/gtk.h>

#include "app.h"
#include "window.h"

extern void app_run(int argc, char** argv) {
    gtk_init(&argc, &argv);
    gtk_widget_show_all(window_new());
    gtk_main();
}

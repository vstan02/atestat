#ifndef WCODE_MENU_H
#define WCODE_MENU_H

#include <gtk/gtk.h>

#include "state.h"

extern GtkWidget* menu_new(state_t*);

extern void on_save(GtkWidget*, gpointer);

#endif // WCODE_MENU_H

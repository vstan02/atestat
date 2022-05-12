#ifndef WCODE_TERMINAL_H
#define WCODE_TERMINAL_H

#include <gtk/gtk.h>

#include "state.h"

#define TERMINAL_PADDING 10

extern GtkWidget* terminal_new(state_t*);

#endif // WCODE_TERMINAL_H

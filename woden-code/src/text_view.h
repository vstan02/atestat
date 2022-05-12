#ifndef WCODE_TEXT_VIEW_H
#define WCODE_TEXT_VIEW_H

#include <gtk/gtk.h>

#include "state.h"

#define TEXT_VIEW_PADDING 10

extern GtkWidget* text_view_new(state_t*);

#endif // WCODE_TEXT_VIEW_H

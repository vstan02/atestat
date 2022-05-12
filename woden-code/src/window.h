#ifndef WCODE_WINDOW_H
#define WCODE_WINDOW_H

#include <gtk/gtk.h>

#define WINDOW_TITLE "Woden Code"
#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600

#define WINDOW_CMD_MAX 256
#define WINDOW_CMD_RES_MAX 2000

extern GtkWidget* window_new(void);

#endif // WCODE_WINDOW_H

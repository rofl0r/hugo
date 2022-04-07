#ifndef _INCLUDE_GTK_MAIN_H
#define _INCLUDE_GTK_MAIN_G

#include <gtk/gtk.h>

extern GtkWidget* fileselector_window;
extern GtkWidget* fileselector_cd_path;
extern GtkWidget* fileselector_cd_system;
extern GtkWidget* fileselector_rom_path;

extern GtkWidget* input_settings_window;
extern GtkWidget* general_settings_window;
extern GtkWidget* main_window;
extern GtkWidget* about_window;
extern GtkWidget* manual_window;

void build_gtk_interface (int argc, char* argv[]);

#endif

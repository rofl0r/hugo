#ifndef _INCLUDE_GTK_MAIN_H
#define _INCLUDE_GTK_MAIN_H

#ifndef S_SPLINT_S
/* Gtk headers return splint errors */
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

#endif

void gtk_general_settings_set();
void gtk_general_settings_grab();
void set_gui_configuration_index(int new_index);
void build_gtk_interface (int argc, char* argv[]);
int get_gui_configuration_index();
void gtk_grab_control(int direction_index, int player_number);
void gtk_copy_current_configuration();
void gtk_confirm_configuration();
void set_gui_joydev(int player_number,int new_value);
void gtk_update_configuration(gboolean);

#endif

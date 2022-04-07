#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

#include "gtk_main.h"
#include "pce.h"
#include "iniconfig.h"

gboolean
on_mainWindow_key_press_event          (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data)
{

  return FALSE;
}


gboolean
on_mainWindow_key_release_event        (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data)
{

  return FALSE;
}


void
on_open1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  gtk_file_selection_set_filename(fileselector_window, initial_path);
  gtk_widget_show(fileselector_window);
}


void
on_quit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_save1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    savegame();
}


void
on_load1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	loadgame();
}


void
on_input_setting_1_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  gtk_widget_show(input_settings_window);
}


void
on_hugo_manual1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	gtk_widget_show(manual_window);
}


void
on_pc_engine_story1_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_about_1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	gtk_widget_show(about_window);
}


void
on_mainWindow_destroy                  (GtkObject       *object,
                                        gpointer         user_data)
{
  gtk_main_quit();
}


void
on_ok_button1_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{	
	gtk_widget_hide(fileselector_window);
	strcpy( cart_name, gtk_file_selection_get_filename(fileselector_window));

	/*
	 * We need to flush any gtk events waiting to happen (like the widget hide
         * from above) to avoid a deadlock when starting a game fullscreen (at least
         * in linux).
         */
	while (gtk_events_pending())
	  gtk_main_iteration();

	play_game();
}


void
on_button1_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
  on_open1_activate(NULL, NULL);
}


void
on_cancel_button1_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_hide(fileselector_window);
}


void
on_option_config_number_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{
	printf("option clicked\n");
}


void
on_option_device_number_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_option_device_type_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button_close_input_settings_window_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_hide(input_settings_window);
}


void
on_button_grab_up_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
	/*
	GtkWidget * label = lookup_widget(GTK_WIDGET(button), "label_up");
	SDLKey key = get_keysym();

	printf("pressed key = %d\n", key);	
	
	gtk_label_set_text(label, SDL_GetKeyName(key));
	*/
}


void
on_button2_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
  CD_emulation = 1;
  play_game();
}


void
on_button_grab_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{	
	gtk_grab_config_for_key(user_data);
}


void
on_button_use_current_config_clicked   (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkOptionMenu* option_menu;
    GtkWidget *active_item;
    int item_index;  
	
	option_menu = lookup_widget(input_settings_window, "option_config_number");
  
    active_item = gtk_menu_get_active (GTK_MENU (option_menu->menu));  
    item_index = g_list_index ( GTK_MENU(option_menu->menu)->menu_shell.children, active_item);  
  
    gtk_show_config(item_index);

}


void
on_button_configure_this_player_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkOptionMenu* option_menu;
    GtkWidget *active_item;
    int item_index;  
	
	option_menu = lookup_widget(input_settings_window, "option_player_number");
  
    active_item = gtk_menu_get_active (GTK_MENU (option_menu->menu));  
    item_index = g_list_index ( GTK_MENU(option_menu->menu)->menu_shell.children, active_item);  
  
    gtk_show_config_player(item_index);
}


void
on_button_use_this_device_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkOptionMenu* option_menu;
    GtkWidget *active_item;
    int item_index;  
	
	option_menu = lookup_widget(input_settings_window, "option_device_type");
  
    active_item = gtk_menu_get_active (GTK_MENU (option_menu->menu));  
    item_index = g_list_index ( GTK_MENU(option_menu->menu)->menu_shell.children, active_item);  
  
    gtk_select_config_device(item_index);
}


void
on_general_settings_1_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	gtk_widget_show(general_settings_window);
}


void
on_button_close_about_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_hide(about_window);
}


void
on_button_browse_cd_system_clicked     (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_show(fileselector_cd_system);
}


void
on_buttongeneral_config_close_clicked  (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_hide(general_settings_window);
	gtk_general_settings_grab();
}


void
on_button_general_config_save_clicked  (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_general_settings_grab();
	save_config();
}


void
on_button_general_config_cancel_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_hide(general_settings_window);
}


void
on_general_settings_window_show        (GtkWidget       *widget,
                                        gpointer         user_data)
{
	gtk_general_settings_set();
}


void
on_ok_button_cd_system_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkEntry* temp_entry;

	gtk_widget_hide(fileselector_cd_system);
	strncpy (cdsystem_path, gtk_file_selection_get_filename(fileselector_cd_system), PATH_MAX);
	temp_entry = (GtkEntry*)lookup_widget(general_settings_window, "entry_cd_system_filename");
	gtk_entry_set_text(temp_entry, cdsystem_path);	
}


void
on_cancel_button_cd_system_clicked     (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_hide(fileselector_cd_system);
}


void
on_ok_button_cd_path_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkEntry* temp_entry;	
	
	gtk_widget_hide(fileselector_cd_path);
	strcpy (ISO_filename, gtk_file_selection_get_filename(fileselector_cd_path));
	temp_entry = (GtkEntry*)lookup_widget(general_settings_window, "entry_cd_path");
	gtk_entry_set_text(temp_entry, ISO_filename);

}


void
on_cancel_button_cd_path_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_hide(fileselector_cd_path);
}

void
on_button_browse_rom_dirname_clicked   (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_show(fileselector_rom_path);
}


void
on_button_browse_cd_path_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_show(fileselector_cd_path);
}


void
on_ok_button_rom_path_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkEntry* temp_entry;
	
	gtk_widget_hide(fileselector_rom_path);
	strcpy (initial_path, gtk_file_selection_get_filename(fileselector_rom_path));
	if (strrchr(initial_path, '/') != NULL)
		*strrchr(initial_path, '/') = 0;
	temp_entry = (GtkEntry*)lookup_widget(general_settings_window, "entry_rom_basedir");
	gtk_entry_set_text(temp_entry, initial_path);
}


void
on_cancel_button_rom_path_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_hide(fileselector_rom_path);
}


void
on_button_manual_close_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_hide(manual_window);
}

void
on_input_settings_window_show          (GtkWidget       *widget,
                                        gpointer         user_data)
{
	gtk_show_config(0);
}


#include "gtk_main.h"

#include "sys_inp.h"

#include "iniconfig.h"

#if defined(SDL)
  #include "SDL.h"
#endif

GtkWidget* fileselector_window = NULL;
GtkWidget* fileselector_cd_path = NULL;
GtkWidget* fileselector_cd_system = NULL;
GtkWidget* fileselector_rom_path = NULL;

GtkWidget* input_settings_window = NULL;
GtkWidget* general_settings_window = NULL;
GtkWidget* main_window = NULL;
GtkWidget* about_window = NULL;
GtkWidget* manual_window = NULL;

void gtk_show_config_player(int player_index);

#include "manual.h"

//! Construct the gtk interface
void build_gtk_interface (int argc, char* argv[])
{
	
  GtkText* manual_text = NULL;
	
  gtk_set_locale ();
  gtk_init (&argc, &argv);

  add_pixmap_directory (".");
  add_pixmap_directory ("/usr/share/hugo/pixmaps");
  add_pixmap_directory (PACKAGE_DATA_DIR "/pixmaps");
  add_pixmap_directory ("./pixmaps");
  add_pixmap_directory(short_exe_name);
  /*
  add_pixmap_directory (PACKAGE_SOURCE_DIR "/pixmaps");
  */
	
  fileselector_window = create_fileselection1();
  fileselector_cd_path = create_fileselection_cd_path();
  fileselector_cd_system = create_fileselection_cd_system();
  fileselector_rom_path = create_fileselection_rom_path();
	
  input_settings_window = create_input_settings_window();
  about_window = create_window_about();
  general_settings_window = create_general_settings_window();
  manual_window = create_window_manual();

  manual_text = (GtkText*)lookup_widget(manual_window, "text_manual");
  
#if !defined(LINUX)
  gtk_text_insert(manual_text, NULL, NULL, NULL, "The manual is called README and it available in the same directory as the executable", 84);
#else
  gtk_text_insert(manual_text, NULL, NULL, NULL, manual_content, sizeof(manual_content));
#endif
  
  gtk_file_selection_set_filename(fileselector_window, initial_path);

  main_window = create_mainWindow ();
  gtk_widget_show (main_window);

  gtk_main ();
}

//! Convert a device index from hugo notation into flat one
int
hugo_device_to_flat_index (int hugo_index) {
	switch (hugo_index)
	{
		case KEYBOARD1:
			return 1;
		case KEYBOARD2:
			return 2;
		case KEYBOARD3:
			return 3;
		case KEYBOARD4:
			return 4;
		case KEYBOARD5:
			return 5;
		case JOYPAD1:
			return 6;
		case JOYPAD2:
			return 7;
		case JOYPAD3:
			return 8;
		case JOYPAD4:
			return 9;
		case MOUSE1:
			return 10;
		case MOUSE2:
			return 11;
		case SYNAPLINK:
			return 12;
	}
	
	return 0;
}

//! Tells which mapping is used with the given device
int
hugo_device_to_joymap_index(int hugo_index)
{
	switch (hugo_index)
	{
		case KEYBOARD1:
			return 0;
		case KEYBOARD2:
			return 1;
		case KEYBOARD3:
			return 2;
		case KEYBOARD4:
			return 3;
		case KEYBOARD5:
			return 4;
		case JOYPAD1:
			return 0;
		case JOYPAD2:
			return 1;
		case JOYPAD3:
			return 2;
		case JOYPAD4:
			return 3;
		case MOUSE1:
			return 0;
		case MOUSE2:
			return 1;
		case SYNAPLINK:
			return 0;
	}
	
	return 0;
	
}

static int config_index = 0;

static int player_index = 0;

//! Show the right input configuration
void gtk_show_config(int config_index_local)
{
	GtkOptionMenu* option_player_menu = lookup_widget(input_settings_window, "option_player_number");
	
	config_index = config_index_local;
	
	gtk_option_menu_set_history(option_player_menu, 0);
	
	gtk_show_config_player(0);
	
}

//! Show the configuration for the current input configuration and the given player
void gtk_show_config_player(int player_index_local)
{

	int flat_index = hugo_device_to_flat_index(config[config_index].input_type[player_index_local]);
	
	GtkOptionMenu* option_device_type_menu = lookup_widget(input_settings_window, "option_device_type");

	player_index = player_index_local;
	
	gtk_option_menu_set_history(option_device_type_menu, flat_index);	
	
#if !defined(FINAL_RELEASE)	
	printf("hugo_index = %d, flat_index = %d\n", config[config_index].input_type[player_index_local], flat_index);	
#endif
	
	gtk_show_config_device(flat_index);
	
}

//! Show the configuration for the given device on the current player and configuration
void gtk_show_config_device(int device_index)
{
	int index;
	GtkWidget* label = NULL;
	int mapping_index = hugo_device_to_joymap_index(config[config_index].input_type[player_index]);
	
#if !defined(FINAL_RELEASE)
	
	printf("config_index = %d\nmapping_index = %d\nplayer_index = %d\n", config_index, mapping_index,player_index);
	
	for (index = 0; index < J_MAX; index++)
	{
		printf("key for action %d = %d [%s]\n", index, config[config_index].joy_mapping[mapping_index][index], SDL_GetKeyName(config[config_index].joy_mapping[mapping_index][index]));
	}
#endif
	
	label = lookup_widget(input_settings_window, "label_up");
	gtk_label_set_text(label, SDL_GetKeyName(config[config_index].joy_mapping[mapping_index][0]));
	
	label = lookup_widget(input_settings_window, "label_down");
	gtk_label_set_text(label, SDL_GetKeyName(config[config_index].joy_mapping[mapping_index][1]));	

	label = lookup_widget(input_settings_window, "label_left");
	gtk_label_set_text(label, SDL_GetKeyName(config[config_index].joy_mapping[mapping_index][2]));

	label = lookup_widget(input_settings_window, "label_right");
	gtk_label_set_text(label, SDL_GetKeyName(config[config_index].joy_mapping[mapping_index][3]));	
	
	label = lookup_widget(input_settings_window, "label_select");
	gtk_label_set_text(label, SDL_GetKeyName(config[config_index].joy_mapping[mapping_index][6]));	

	label = lookup_widget(input_settings_window, "label_run");
	gtk_label_set_text(label, SDL_GetKeyName(config[config_index].joy_mapping[mapping_index][7]));	
	
	label = lookup_widget(input_settings_window, "label_i");
	gtk_label_set_text(label, SDL_GetKeyName(config[config_index].joy_mapping[mapping_index][4]));	

	label = lookup_widget(input_settings_window, "label_ii");
	gtk_label_set_text(label, SDL_GetKeyName(config[config_index].joy_mapping[mapping_index][5]));	
	
	label = lookup_widget(input_settings_window, "label_auto_i");
	gtk_label_set_text(label, SDL_GetKeyName(config[config_index].joy_mapping[mapping_index][8]));

	label = lookup_widget(input_settings_window, "label_auto_ii");
	gtk_label_set_text(label, SDL_GetKeyName(config[config_index].joy_mapping[mapping_index][9]));
	
}

void
gtk_select_config_device(int device_index)
{
	switch (device_index) {
		case 1: // KEYBOARD1
			config[config_index].input_type[player_index] = KEYBOARD1;
			config[config_index].pad[player_index] = input1;
		break;
		case 2: // KEYBOARD2
			config[config_index].input_type[player_index] = KEYBOARD2;
			config[config_index].pad[player_index] = input2;
		break;
		case 3: // KEYBOARD3
			config[config_index].input_type[player_index] = KEYBOARD3;
			config[config_index].pad[player_index] = input3;
		break;
		case 4: // KEYBOARD4
			config[config_index].input_type[player_index] = KEYBOARD4;
			config[config_index].pad[player_index] = input4;
		break;
		case 5: // KEYBOARD5
			config[config_index].input_type[player_index] = KEYBOARD5;
			config[config_index].pad[player_index] = input5;
		break;
		case 6: // JOYPAD1
			config[config_index].input_type[player_index] = JOYPAD1;
			config[config_index].pad[player_index] = joypad1;
		break;
		case 7: // JOYPAD2
			config[config_index].input_type[player_index] = JOYPAD2;
			config[config_index].pad[player_index] = joypad2;
		break;
		case 8: // JOYPAD3
			config[config_index].input_type[player_index] = JOYPAD3;
			config[config_index].pad[player_index] = joypad3;
		break;
		case 9: // JOYPAD4
			config[config_index].input_type[player_index] = JOYPAD4;
			config[config_index].pad[player_index] = joypad4;
		break;
		case 10: // MOUSE1
			config[config_index].input_type[player_index] = MOUSE1;
			config[config_index].pad[player_index] = mouse1;
		break;
		case 11: // MOUSE2
			config[config_index].input_type[player_index] = MOUSE2;
			config[config_index].pad[player_index] = mouse2;
		break;
		case 12: // SYNAPLINK
			config[config_index].input_type[player_index] = SYNAPLINK;
			config[config_index].pad[player_index] = synaplink;
		break;
		default:
			config[config_index].input_type[player_index] = NONE;
			config[config_index].pad[player_index] = noinput;
	}

	gtk_show_config_device(device_index);
	
}

void gtk_grab_config_for_key(int key_index)
{

#if !defined(SDL)
	  #warning Implement interactive key grabbing without SDL
#endif
	SDL_Surface *grab_window;
	SDL_Event event;

	SDL_PauseAudio(SDL_ENABLE);

	if (physical_screen == NULL)
        {
	  SDL_InitSubSystem(SDL_INIT_VIDEO);

	  if ((grab_window = SDL_SetVideoMode(200, 50, 8, 0)) == NULL)
	  {
		fprintf(stderr,"Couldn't create the window for grabbing key\n");
		Log("Couldn't create the window for grabbing key\n");
	  }
	  SDL_WM_SetCaption("Press a key", NULL);
	}
	else
	{
	  grab_window = physical_screen;
	}

	if (SDL_WM_GrabInput(SDL_GRAB_ON) != SDL_GRAB_ON)
	  printf("Couldn't grab input focus.\n");

	while (SDL_WaitEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			if (physical_screen == NULL)
			{
                	  SDL_QuitSubSystem(SDL_INIT_VIDEO);
			}

			config[config_index].joy_mapping[
				hugo_device_to_joymap_index(config[config_index].input_type[player_index])
			][key_index] = event.key.keysym.sym;
			gtk_show_config_player(player_index);
			break;
		}
	}

	if (SDL_WM_GrabInput(SDL_GRAB_OFF) != SDL_GRAB_OFF)
	  printf("Couldn't release window focus.\n");

	SDL_PauseAudio(SDL_DISABLE);
}

//! Read configuration done in the general settings window to alter real variables used in emulation
void
gtk_general_settings_grab()
{
	GtkCheckButton* temp_check_button;
	GtkEntry* temp_entry;
	GtkSpinButton* temp_spin_button;
	
	// get start at fullscreen mode setting
	temp_check_button = (GtkCheckButton*)lookup_widget(general_settings_window, "checkbutton_start_fullscreen");
	option.want_fullscreen = gtk_toggle_button_get_active(temp_check_button) ? 1 : 0;

	// want fullscreen aspect?
	temp_check_button = (GtkCheckButton *) lookup_widget(general_settings_window, "checkbutton_maintain_aspect");
	option.want_fullscreen_aspect = gtk_toggle_button_get_active(temp_check_button) ? 1 : 0;
	
	// get cd system filename setting
	temp_entry = (GtkEntry*)lookup_widget(general_settings_window, "entry_cd_system_filename");
	strcpy(cdsystem_path, gtk_entry_get_text(temp_entry));

	// get rom initial path setting
	temp_entry = (GtkEntry*)lookup_widget(general_settings_window, "entry_rom_basedir");
	strcpy(initial_path, gtk_entry_get_text(temp_entry));	
	
	// get minimal bios setting
	temp_check_button = (GtkCheckButton*)lookup_widget(general_settings_window, "checkbutton_minimal_bios");
	minimum_bios_hooking = gtk_toggle_button_get_active(temp_check_button) ? 1 : 0;
	
	// get cd path setting
	temp_entry = (GtkEntry*)lookup_widget(general_settings_window, "entry_cd_path");
	strcpy(ISO_filename, gtk_entry_get_text(temp_entry));	

    // want stereo sound?
    temp_check_button = (GtkEntry *) lookup_widget(general_settings_window, "checkbutton_stereo_sound");
    option.want_stereo = gtk_toggle_button_get_active(temp_check_button) ? 1 : 0;

	// get output frequency
	temp_entry = (GtkEntry*)lookup_widget(general_settings_window, "entry_output_frequency");
	freq_int = atoi(gtk_entry_get_text(temp_entry));
	
	// get sound buffer size
	temp_entry = (GtkEntry*)lookup_widget(general_settings_window, "entry_buffer_size");
	sbuf_size = atoi(gtk_entry_get_text(temp_entry));
	
	// get window size
	temp_spin_button = (GtkSpinButton *)lookup_widget(general_settings_window, "spinbutton_window_size");
    option.window_size = gtk_spin_button_get_value_as_int(temp_spin_button);
	
	// get fullscreen height
	temp_entry = (GtkEntry*)lookup_widget(general_settings_window, "entry_fullscreen_height");
	option.fullscreen_height = atoi(gtk_entry_get_text(temp_entry));

	// get fullscreen height
	temp_entry = (GtkEntry*)lookup_widget(general_settings_window, "entry_fullscreen_width");
	option.fullscreen_width = atoi(gtk_entry_get_text(temp_entry));

}

//! Read current configuration and put the widgets in the corresponding mode
void
gtk_general_settings_set()
{
	GtkCheckButton* temp_check_button;
	GtkEntry* temp_entry;
	GtkSpinButton* temp_spin_button;
	char temp_string[10];

	temp_check_button = (GtkCheckButton*)lookup_widget(general_settings_window, "checkbutton_start_fullscreen");
	gtk_toggle_button_set_active(temp_check_button, option.want_fullscreen ? TRUE : FALSE);

	temp_check_button = (GtkCheckButton *) lookup_widget(general_settings_window, "checkbutton_maintain_aspect");
	gtk_toggle_button_set_active(temp_check_button, option.want_fullscreen_aspect ? TRUE : FALSE);

	temp_entry = (GtkEntry*)lookup_widget(general_settings_window, "entry_cd_system_filename");
	gtk_entry_set_text(temp_entry, cdsystem_path);

	temp_entry = (GtkEntry*)lookup_widget(general_settings_window, "entry_rom_basedir");
	gtk_entry_set_text(temp_entry, initial_path);

	temp_check_button = (GtkCheckButton*)lookup_widget(general_settings_window, "checkbutton_minimal_bios");
	gtk_toggle_button_set_active(temp_check_button, minimum_bios_hooking ? TRUE : FALSE);

	temp_entry = (GtkEntry*)lookup_widget(general_settings_window, "entry_cd_path");
	gtk_entry_set_text(temp_entry, ISO_filename);

	temp_check_button = (GtkEntry *) lookup_widget(general_settings_window, "checkbutton_stereo_sound");
	gtk_toggle_button_set_active(temp_check_button, option.want_stereo ? TRUE : FALSE);

	temp_entry = (GtkEntry*)lookup_widget(general_settings_window, "entry_output_frequency");
	if (freq_int == 0)
		strcpy(temp_string, "");
	else
	  sprintf(temp_string, "%d", freq_int);
	gtk_entry_set_text(temp_entry, temp_string);

	temp_entry = (GtkEntry*)lookup_widget(general_settings_window, "entry_buffer_size");
	if (sbuf_size == 0)
		strcpy(temp_string, "");
	else
	  sprintf(temp_string, "%d", sbuf_size);
	gtk_entry_set_text(temp_entry, temp_string);	
	
	// set window size
	temp_spin_button = (GtkSpinButton*)lookup_widget(general_settings_window, "spinbutton_window_size");  
	gtk_spin_button_set_value (temp_spin_button, option.window_size);

	// set window width
	temp_entry = (GtkEntry*)lookup_widget(general_settings_window, "entry_fullscreen_width");
	if (option.fullscreen_width == 0)
		strcpy(temp_string, "640");
	else
	  sprintf(temp_string, "%d", option.fullscreen_width);
	gtk_entry_set_text(temp_entry, temp_string);	

	// set window height
	temp_entry = (GtkEntry*)lookup_widget(general_settings_window, "entry_fullscreen_height");
	if (option.fullscreen_height == 0)
		strcpy(temp_string, "480");
	else
	  sprintf(temp_string, "%d", option.fullscreen_height);
	gtk_entry_set_text(temp_entry, temp_string);

}

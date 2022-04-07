#include "config.h"

char sCfgFileLine[BUFSIZ];
// line buffer for config reading

char config_file[PATH_MAX];
// name of the config file

char* config_buffer;
// content of the config file

UInt32 config_buffer_length;
// size of config_buffer

char*
get_config_var(char* section, char* cfgId )
{

   FILE*  FCfgFile = NULL;
   char* pWrd = NULL;
   char* pRet;
	
   /* open config file for reading */
   if ((FCfgFile = fopen(config_file, "r")) != NULL)
   {
      /* read through the file looking for our keyword */
      do
      {
	 memset( sCfgFileLine, '\0', BUFSIZ );
	 /* scan for keyword */
	 /* note.. line must NOT be a comment */
	 pRet = fgets(sCfgFileLine, BUFSIZ, FCfgFile );
		  
     if (section == NULL)
	 {	 
	   if((( pWrd = strstr( sCfgFileLine, cfgId ) ) != NULL && sCfgFileLine[0] != '#' ) 
		     && pWrd != NULL  && pWrd == sCfgFileLine )
	   {
	      // pWrd = strchr( sCfgFileLine, ' ' ) + 1;
		   pWrd = strchr( sCfgFileLine, '=' ) + 1;

	      pRet =  strchr(pWrd, '\n' );
	      if( pRet != NULL )
	         *pRet = '\0';

	      break;
	   }
   }
	 else
		 if (!strncmp(section,sCfgFileLine + 1,strlen(section)) && sCfgFileLine[0] == '[' && sCfgFileLine[strlen(section) + 1] == ']')
			 section = NULL;

      } while ( pRet != NULL);

      fclose( FCfgFile );
   }
   if(pWrd != NULL)
   { 
      while( (*pWrd==' ') || *pWrd=='\t')
                                pWrd++;
   }
   
   return pWrd;
}

#if !defined(ALLEGRO)

// Let's redefine the old allegro parsing function

int get_config_int(char* section, char* keyword, int default_value)
{
	char* p = get_config_var(section, keyword);
	if (p == NULL)
		return default_value;
	return atoi(p);
}

char* get_config_string(char* section, char* keyword, char* default_value)
{
	char* p = get_config_var(section, keyword);
	return (p == NULL ? default_value : p);
}

#endif

const char *joy_str[] =
  { "UP", "DOWN", "LEFT", "RIGHT", "I", "II", "SELECT", "START", "AUTOI",
  "AUTOII", "PI", "PII", "PSELECT", "PSTART", "PAUTOI", "PAUTOII"
};

void
read_joy_mapping ()
{
  UInt32 dum;
  char tmp_str[10], tmp_str2[10], section_name[10], x, y, z;
  unsigned short temp_val;

  Log ("--[ JOYPAD MAPPING ]-------------------------------\n");
  Log ("Loading default values\n");
  
#ifdef ALLEGRO

  // Default value for first input
  for (dum = 0; dum < 5; dum++)
    {
      joy_mapping[dum][J_DOWN] = KEY_DOWN;
      joy_mapping[dum][J_UP] = KEY_UP;
      joy_mapping[dum][J_LEFT] = KEY_LEFT;
      joy_mapping[dum][J_RIGHT] = KEY_RIGHT;
      joy_mapping[dum][J_SELECT] = KEY_TAB;
      joy_mapping[dum][J_START] = KEY_ENTER;
      joy_mapping[dum][J_I] = KEY_ALT;
      joy_mapping[dum][J_II] = KEY_SPACE;
      joy_mapping[dum][J_AUTOI] = KEY_C;
      joy_mapping[dum][J_AUTOII] = KEY_X;
      joy_mapping[dum][J_PI] = 0;
      joy_mapping[dum][J_PII] = 1;
      joy_mapping[dum][J_PSELECT] = 10;
      joy_mapping[dum][J_PSTART] = 8;
      joy_mapping[dum][J_PAUTOI] = 6;
      joy_mapping[dum][J_PAUTOII] = 7;
    }

#endif

  memset (tmp_str, 0, 16);

  for (x = 1; x <= 5; x++)
    {				// for each joystick
      for (y = 0; y < 16; y++)
	{
	  strcpy (tmp_str, joy_str[y]);
	  tmp_str[strlen (tmp_str) + 1] = 0;
	  tmp_str[strlen (tmp_str)] = x + 0x30;

	  if ((temp_val = get_config_int (NULL, tmp_str, 0xffff)) != 0xffff)
	    {
	      joy_mapping[x - 1][y] = temp_val;
	      Log ("Setting joy_mapping[%d][%d] to %d\n", x - 1, y, temp_val);
	    }

	}
    }


  strcpy (section_name, "CONFIG1");
  for (z = 1; z < 16; z++)
    {
      if (z < 10)
	section_name[6] = '0' + z;
      else
	section_name[6] = z - 10 + 'a';

      Log (" * Looking for section %s\n", section_name);

      for (x = 1; x <= 5; x++)
	{			// for each joystick

	  strcpy (tmp_str2, "input1");
	  tmp_str2[5] = '0' + x;

	  config[z].input_type[x - 1] = NONE;

	  strcpy (tmp_str, get_config_string (section_name, tmp_str2, ""));

	  Log ("Setting PCE pad %d to %s\n", x, tmp_str);

	  if (!stricmp (tmp_str, "key1"))
	    config[z].input_type[x - 1] = KEYBOARD1;
	  else if (!stricmp (tmp_str, "key2"))
	    config[z].input_type[x - 1] = KEYBOARD2;
	  else if (!stricmp (tmp_str, "key3"))
	    config[z].input_type[x - 1] = KEYBOARD3;
	  else if (!stricmp (tmp_str, "key4"))
	    config[z].input_type[x - 1] = KEYBOARD4;
	  else if (!stricmp (tmp_str, "key5"))
	    config[z].input_type[x - 1] = KEYBOARD5;
	  else if (!stricmp (tmp_str, "joy1"))
	    config[z].input_type[x - 1] = JOYPAD1;
	  else if (!stricmp (tmp_str, "joy2"))
	    config[z].input_type[x - 1] = JOYPAD2;
	  else if (!stricmp (tmp_str, "joy3"))
	    config[z].input_type[x - 1] = JOYPAD3;
	  else if (!stricmp (tmp_str, "joy4"))
	    config[z].input_type[x - 1] = JOYPAD4;
	  else if (!stricmp (tmp_str, "mouse1"))
	    config[z].input_type[x - 1] = MOUSE1;
	  else if (!stricmp (tmp_str, "mouse2"))
	    config[z].input_type[x - 1] = MOUSE2;
	  else if (!stricmp (tmp_str, "synaplink"))
	    config[z].input_type[x - 1] = SYNAPLINK;

	  for (y = 0; y < 16; y++)
	    {
	      strcpy (tmp_str, joy_str[y]);
	      tmp_str[strlen (tmp_str) + 1] = 0;
	      tmp_str[strlen (tmp_str)] = x + 0x30;

	      if ((temp_val = get_config_int (section_name, tmp_str, 0xffff)) != 0xffff)
		{
		  config[z].joy_mapping[x - 1][y] = temp_val;
		  Log ("    %s set to %d\n", joy_str[y], temp_val);

		}

	    }
	}
    }

  // If there, we haven't found any input related stuff

  Log ("End of joypad mapping\n\n");

}


char
set_arg (char nb_arg, char *val)
{
  if (!val)
    {
#ifndef FINAL_RELEASE
      fprintf (stderr, "No value for %c arg\n", nb_arg);
#endif
      Log ("No value for arg %c\n", nb_arg);
      return 1;
    };
  switch (nb_arg)
    {
    case 'c':
      CD_emulation = atoi (val);
      Log ("CD emulation set to %d\n", CD_emulation);
      return 0;
    case 'd':
      debug_on_beginning = atoi (val);
      Log ("Debug on beginning set to %d\n", debug_on_beginning);
      return 0;
    case 'e':
      use_eagle = atoi (val);
      Log ("Eagle mode set to %d\n", use_eagle);
      return 0;
    case 'i':
      strcpy (ISO_filename, val);
      Log ("ISO filename is %s\n", ISO_filename);
      return 0;
    case 'm':
      minimum_bios_hooking = atoi (val);
      Log ("Minimum Bios hooking set to %d\n", minimum_bios_hooking);
      return 0;
    case 's':
      smode = atoi (val);
      Log ("Sound mode set to %d\n", smode);
      return 0;
    case 'S':
      use_scanline = atoi (val);
      Log ("Scanline mode set to %d\n", use_scanline);
      return 0;
    case 'u':
      US_encoded_card = atoi (val);
      Log ("US Card encoding set to %d\n", US_encoded_card);
      return 0;
    case 'v':
      vmode = atoi (val);
      Log ("Video mode set to %d\n", vmode);
      return 0;
    case 't':
      nb_max_track = atoi (val);
      Log ("Number of tracks set to %d\n", nb_max_track);
      return 0;
    default:
#ifndef FINAL_RELEASE
      fprintf (stderr, "Unrecognized option : %c\n", nb_arg);
#endif
      Log ("Unrecognize option : %c\n", nb_arg);
      return 1;
    }
};

void
parse_commandline (int argc, char **argv)
{
  char next_arg, i, arg_error = 0;

  Log ("--[ PARSING COMMAND LINE ]--------------------------\n");

  next_arg = 0;
  for (i = 1; i < argc; i++)
    if (!next_arg)
      {
	if (argv[i][0] == '-')
	  {
	    if (strlen (argv[i]) == 2)
	      next_arg = argv[i][1];
	    else
	      arg_error |= set_arg (argv[i][1], (char *) &argv[i][2]);
	  }
	else
	  {
	    if (!cart_name[0])
	      {
		strcpy (cart_name, argv[i]);
		Log ("Setting card name to %s\n", cart_name);
		{
		  int x;
		  for (x = 0; x < strlen (cart_name); x++)
		    if (cart_name[x] == '\\')
		      cart_name[x] = '/';
		}
	      }
	    else if (!bmdefault)
	      {
		Log ("Setting backup mem file name to %s\n", argv[i]);
		bmdefault = argv[i];
	      }
	    else
	      {
		Log ("Unrecognized option : %s\n", argv[i]);
		arg_error = 1;
	      };
	  }
      }
    else
      {
	arg_error |= set_arg (next_arg, argv[i]);
	next_arg = 0;
      }

  if (next_arg)
    {
      Log ("No value for last arg : %c\n", next_arg);
      next_arg = 0;
      arg_error = 1;
    };

  Log ("End of parsing command line\n");

  video_driver = 0;
  if (use_eagle)
    video_driver = 1;
  else if (use_scanline)
    video_driver = 2;

  return;
}

void
parse_INIfile ()
{
  char tmp_path[80];
  char x;

  Log ("--[ PARSING INI FILE ]------------------------------\n");

#ifndef LINUX
  strcpy (config_file, short_exe_name);
  strcat (config_file, "HU-GO!.INI");
#else
  {

    char tmp_home[256];
	  FILE* f;

    sprintf (tmp_home, "%s/.hugo/hugo.ini", getenv ("HOME"));

	  f = fopen(tmp_home,"rb");
	  
    if (f != NULL)
	{
		strcpy (config_file, tmp_home);
		fclose(f);		
	}
    else
      strcpy (config_file, "/etc/hugo.ini");
	
  }
#endif

  Log ("Looking in %s\n", config_file);

  read_joy_mapping ();

  strcpy (initial_path, get_config_string ("main", "rom_dir", "."));
  if ((initial_path[0]) && (initial_path[strlen (initial_path) - 1] != '/')
      && (initial_path[strlen (initial_path) - 1] != '\\'))
    strcat (initial_path, "/");
  // rom_dir setting

  Log ("Setting initial path to %s\n", initial_path);

#if defined(ALLEGRO)
  
  strcpy (skin_filename, get_config_string ("main", "skin", "skin_h~1.bmp"));
  // skin filename to look for

  Log ("Skin filename set to %s\n", skin_filename);

#endif  
  
  current_config = get_config_int ("main", "config", 0);
  // choose input config

  Log ("Setting joypad config number to %d\n", current_config);

  language = min (get_config_int ("main", "language", 0), NB_LANG - 1);
  // language setting

  Log ("Setting language to %d\n", language);

  vmode = get_config_int ("main", "vmode", 0);
  // video mode setting

  Log ("Setting video mode to %d\n", vmode);

  smode = get_config_int ("main", "smode", -1);
  // sound mode setting

  Log ("Setting sound mode to %d\n", smode);

#if defined(ALLEGRO)

  static_refresh = get_config_int ("main", "static_refresh", 0);
  // file selector refreshment

  Log ("Setting static refresh to %d\n", static_refresh);

#endif

  use_eagle = get_config_int ("main", "eagle", 0);
  // do we use EAGLE ?

  Log ("Setting eagle mode to %d\n", use_eagle);

  use_scanline = get_config_int ("main", "scanline", 0);
  // do we use EAGLE ?

  Log ("Setting scanline mode to %d\n", use_scanline);

  freq_int = get_config_int ("main", "snd_freq", 11025);
  // frequency of the sound generator

  Log ("Setting default frequency to %d\n", freq_int);

  sbuf_size = get_config_int ("main", "buffer_size", 10 * 1024);
  // size of the sound buffer

  Log ("Setting sound buffer size to %d bytes\n", sbuf_size);

  gamepad_driver = get_config_int ("main", "joy_type", -1);

  Log ("Setting joy type to %d\n", gamepad_driver);

  sound_driver = get_config_int ("main", "sound_driver", 1);

  Log ("Setting sound driver to %d\n", sound_driver);

#if defined(ALLEGRO)

  zip_support_in_fs = get_config_int ("main", "zip_support", 1);

  Log ("Setting zip support in File Selector to %d\n", zip_support_in_fs);

#endif

  synchro = get_config_int ("main", "limit_fps", 0);

  Log ("Setting fps limitation to %d\n", synchro);

  /* TODO : remove this stuff */
#ifdef MSDOS

  x = get_config_int ("main", "cd_driver", 0);

  if (x)
    {
      osd_cd_driver = aspi_driver;
      Log ("Setting cd driver to ASPI\n");
    }
  else
    {
      osd_cd_driver = mscdex_driver;
      Log ("Setting cd driver to MSCDEX\n");
    }

#endif

  minimum_bios_hooking = get_config_int ("main", "minimum_bios_hooking", 0);

  Log ("Minimum Bios hooking set to %d\n", minimum_bios_hooking);

  for (x = 0; x < 5; x++)
    {
      if (config[current_config].input_type[x] == NONE)
	config[current_config].pad[x] = noinput;
      else if (config[current_config].input_type[x] == KEYBOARD1)
	config[current_config].pad[x] = keyboard1;
      else if (config[current_config].input_type[x] == KEYBOARD2)
	config[current_config].pad[x] = keyboard2;
      else if (config[current_config].input_type[x] == KEYBOARD3)
	config[current_config].pad[x] = keyboard3;
      else if (config[current_config].input_type[x] == KEYBOARD4)
	config[current_config].pad[x] = keyboard4;
      else if (config[current_config].input_type[x] == KEYBOARD5)
	config[current_config].pad[x] = keyboard5;
      else if (config[current_config].input_type[x] == JOYPAD1)
	config[current_config].pad[x] = joypad1;
      else if (config[current_config].input_type[x] == JOYPAD2)
	config[current_config].pad[x] = joypad2;
      else if (config[current_config].input_type[x] == JOYPAD3)
	config[current_config].pad[x] = joypad3;
      else if (config[current_config].input_type[x] == JOYPAD4)
	config[current_config].pad[x] = joypad4;
      else if (config[current_config].input_type[x] == MOUSE1)
	config[current_config].pad[x] = mouse1;
      else if (config[current_config].input_type[x] == MOUSE2)
	config[current_config].pad[x] = mouse2;
      else if (config[current_config].input_type[x] == SYNAPLINK)
	config[current_config].pad[x] = synaplink;
    }

  Log ("End of parsing INI file\n\n");

  return;
}

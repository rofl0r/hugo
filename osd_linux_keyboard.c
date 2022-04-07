#include "osd_linux_keyboard.h"

Uint16 read_input(Uint16 port);
Uint16 input1(void), input2(void), input3(void), input4(void), input5(void);

#ifdef ALLEGRO
input_config config[16] =
{
  {
    { input1, input2, input3, input4, input5 },
    { KEYBOARD1, NONE, NONE, NONE, NONE },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    {
      { KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_SPACE, KEY_ALT, KEY_TAB, KEY_ENTER, KEY_C, KEY_X },
      { },
      { },
      { },
      { }
    }
  }
}; 


// info about the input config 
#elif SDL

Uint8 *key;
Int16 joy[J_MAX];

input_config config[16] =
{
  {
    { input1, input2, input3, input4, input5 },
    { KEYBOARD1, NONE, NONE, NONE, NONE },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    {
      { SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_SPACE, SDLK_LALT, SDLK_TAB, SDLK_RETURN, SDLK_c, SDLK_x },
      { },
      { },
      { },
      { }
    }
  }
};


// info about the input config
#endif


UChar current_config;

// the number of the current config
char tmp_buf[100];

#define	JOY_A		0x01
#define	JOY_B		0x02
#define	JOY_SELECT	0x04
#define	JOY_START	0x08
#define	JOY_UP		0x10
#define	JOY_RIGHT	0x20
#define	JOY_DOWN	0x40
#define	JOY_LEFT	0x80

extern int UPeriod;

/* for nothing */
UInt16 noinput()
{
  return 0;
}


/* For joypad */
#ifndef SDL

#if !defined(FREEBSD)
struct js_event e;
#endif

js_status joypad[5];

#else

SDL_Joystick *joypad[5];

unsigned char joypad_devs[5] =
{
  1,
  0,
  0,
  0,
  0
};

#endif

/* for keyboard */
#if defined(ALLEGRO)
UInt16 read_input(UInt16 port)
{
  UInt16 tmp = 0;
	
  if (key[config[current_config].joy_mapping[port][J_DOWN]])
    tmp |= JOY_DOWN;
  else if (key[config[current_config].joy_mapping[port][J_UP]])
    tmp |= JOY_UP;

  if (key[config[current_config].joy_mapping[port][J_LEFT]])
    tmp |= JOY_LEFT;
  else if (key[config[current_config].joy_mapping[port][J_RIGHT]])
    tmp |= JOY_RIGHT;

  if (key[config[current_config].joy_mapping[port][J_II]])
  {
    if (!config[current_config].autoII[port])
      tmp |= JOY_A;
    else
    {
      config[current_config].firedII[port] = !config[current_config].firedII[port];
      if (!config[current_config].firedII[port])
        tmp |= JOY_A;
    }
  }

  if (key[config[current_config].joy_mapping[port][J_I]])
  {
    if (!config[current_config].autoI[port])
      tmp |= JOY_B;
    else
    {
      config[current_config].firedI[port] = !config[current_config].firedI[port];
      if (!config[current_config].firedI[port])
        tmp |= JOY_B;
    }
  }

  if (key[config[current_config].joy_mapping[port][J_SELECT]])
    tmp |= JOY_SELECT;

  if (key[config[current_config].joy_mapping[port][J_START]])
    tmp |= JOY_START;

  if (key[config[current_config].joy_mapping[port][J_AUTOI]])
  {
    if (!key_delay)
    {
      config[current_config].autoI[port] = !config[current_config].autoI[port];
      key_delay = 10;
    }
  }

  if (key[config[current_config].joy_mapping[port][J_AUTOII]])
  {
    if (!key_delay)
    {
      config[current_config].autoII[port] = !config[current_config].autoII[port];
      key_delay = 10;
    }
  }

  return tmp;
}

#elif SDL

UInt16 read_input(UInt16 port)
{
  static char autoI_delay = 0, autoII_delay = 0;
  UInt16 tmp;

#warning check where to place SDL_PumpEvents more efficiently
  SDL_PumpEvents();

  key = SDL_GetKeyState(NULL);

  for (tmp = J_PAD_START; tmp < J_MAX; tmp++)
    joy[tmp] = 0;

  tmp = 0;

  if (joypad[port])
  {
    joy[J_PXAXIS] = SDL_JoystickGetAxis(joypad[port], config[current_config].joy_mapping[port][J_PXAXIS]);
    joy[J_PYAXIS] = SDL_JoystickGetAxis(joypad[port], config[current_config].joy_mapping[port][J_PYAXIS]);
    joy[J_PSTART] = SDL_JoystickGetButton(joypad[port], config[current_config].joy_mapping[port][J_PSTART]);
    joy[J_PSELECT] = SDL_JoystickGetButton(joypad[port], config[current_config].joy_mapping[port][J_PSELECT]);
    joy[J_PI] = SDL_JoystickGetButton(joypad[port], config[current_config].joy_mapping[port][J_PI]);
    joy[J_PII] = SDL_JoystickGetButton(joypad[port], config[current_config].joy_mapping[port][J_PII]);
    joy[J_PAUTOI] = SDL_JoystickGetButton(joypad[port], config[current_config].joy_mapping[port][J_PAUTOI]);
    joy[J_PAUTOII] = SDL_JoystickGetButton(joypad[port], config[current_config].joy_mapping[port][J_PAUTOII]);
  }

  if (key[config[current_config].joy_mapping[port][J_UP]] || (joy[J_PYAXIS] < -16384))
    tmp |= JOY_UP;
  else if (key[config[current_config].joy_mapping[port][J_DOWN]] || (joy[J_PYAXIS] > 16383))
    tmp |= JOY_DOWN;

  if (key[config[current_config].joy_mapping[port][J_LEFT]] || (joy[J_PXAXIS] < -16384))
    tmp |= JOY_LEFT;
  else if (key[config[current_config].joy_mapping[port][J_RIGHT]] || (joy[J_PXAXIS] > 16383))
    tmp |= JOY_RIGHT;

  if (key[config[current_config].joy_mapping[port][J_II]] || joy[J_PII])
  {
    if (!config[current_config].autoII[port])
      tmp |= JOY_B;
    else
    {
      config[current_config].firedII[port] = !config[current_config].firedII[port];
      if (!config[current_config].firedII[port])
        tmp |= JOY_B;
    }
  }

  if (key[config[current_config].joy_mapping[port][J_I]] || joy[J_PI])
  {
    if (!config[current_config].autoI[port])
      tmp |= JOY_A;
    else
    {
      config[current_config].firedI[port] = !config[current_config].firedI[port];
      if (!config[current_config].firedI[port])
        tmp |= JOY_A;
    }
  }

  if (key[config[current_config].joy_mapping[port][J_SELECT]] || joy[J_PSELECT])
    tmp |= JOY_SELECT;

  if (key[config[current_config].joy_mapping[port][J_START]] || joy[J_PSTART])
    tmp |= JOY_START;

  if (key[config[current_config].joy_mapping[port][J_AUTOI]] || joy[J_PAUTOI])
  {
    if (!autoI_delay)
    {
      config[current_config].autoI[port] = !config[current_config].autoI[port];
      autoI_delay = 20;
    }
  }

  if (key[config[current_config].joy_mapping[port][J_AUTOII]] || joy[J_PAUTOII])
  {
    if (!autoII_delay)
    {
      config[current_config].autoII[port] = !config[current_config].autoII[port];
      autoII_delay = 20;
    }
  }

  if (autoI_delay)
    autoI_delay--;

  if (autoII_delay)
    autoII_delay--;

  return tmp;
}

#endif


UInt16 input1(void)
{
  return read_input(0);
}

UInt16 input2(void)
{
  return read_input(1);
}

UInt16 input3(void)
{
  return read_input(2);
}

UInt16 input4(void)
{
  return read_input(3);
}

UInt16 input5(void)
{
  return read_input(4);
}


#ifndef SDL

/* for joypad */
void poll_joypad(int port)
{
#if !defined(FREEBSD)
  while (read((int) fd[port], &e, sizeof (e)) > 0)
  {
    if (e.type & JS_EVENT_BUTTON)
      joypad[port].button[e.number] = e.value;
    else if (e.type & JS_EVENT_AXIS)
      joypad[port].axis[e.number] = e.value;
  }
#endif	
}

#else

void sdl_config_joypad_axis(short which, joymap axis)
{
  int *bad_axes, num_axes, done;
  unsigned char t;

  num_axes = SDL_JoystickNumAxes(joypad[which]);

  bad_axes = (int *) malloc(sizeof(int) * num_axes);

  for (done = 0; done < num_axes; done++)
    bad_axes[done] = 0;

  done = 0;

  while (1)
  {
    if (read(fileno(stdin), &t, 1) == -1)
      break;
  }

  while (!done)
  {
    time_t stime, ntime;
    int axes;

    if (read(fileno(stdin), &t, 1) == -1)
    {
      if (errno != EAGAIN)
        break;
    }
    else
      break;

    SDL_JoystickUpdate();

    for (axes = num_axes - 1; axes >= 0; axes--)
    {
      if (bad_axes[axes])
        continue;

      if (abs(SDL_JoystickGetAxis(joypad[which], axes)) > 16384)
      {
        printf("    -- Activity reported on axis %d;  Please release axis . . .\n", axes);

        config[current_config].joy_mapping[which][axis] = axes;

        done = 1;

        stime = time(NULL);

        while (abs(SDL_JoystickGetAxis(joypad[which], axes)) > 16384)
        {
          SDL_JoystickUpdate();
          SDL_Delay(50);
          ntime = time(NULL);

          if ((ntime - stime) > 3)
          {
            bad_axes[axes] = 1;
            done = 0;
            printf("    ** Activity still reported on axis after 3 seconds, marking as invalid\n\n"
                   "    Please try another axis now . . .\n");
            break;
          }
        }
        printf("\n");
      }
    }
  }

  while (1)
  {
    if (read(fileno(stdin), &t, 1) == -1)
      break;
  }

  free(bad_axes);
}


void sdl_config_joypad_button(short which, joymap button)
{
  unsigned char done = 0, t;

  while (1)
  {
    if (read(fileno(stdin), &t, 1) == -1)
      break;
  }

  while (!done)
  {
    int buttons;

    if (read(fileno(stdin), &t, 1) == -1)
    {
      if (errno != EAGAIN)
        break;
    }
    else
      break;

    SDL_JoystickUpdate();

    for (buttons = SDL_JoystickNumButtons(joypad[which]) - 1; buttons >= 0; buttons--)
    {
      if (SDL_JoystickGetButton(joypad[which], buttons))
      {
        config[current_config].joy_mapping[which][button] = buttons;

        while (SDL_JoystickGetButton(joypad[which], buttons))
        {
          SDL_JoystickUpdate();
          SDL_Delay(50);
        }

        done = 1;
      }
    }
  }

  while (1)
  {
    if (read(fileno(stdin), &t, 1) == -1)
      break;
  }
}


void sdl_config_joypad(short which)
{
  int sa;

  printf("    ^ We need to configure this joypad ^\n"
         "    Press [ENTER] when ready to begin, [ENTER] can also be used to skip configuration steps.\n");
  getchar();

  sa = fcntl(fileno(stdin), F_GETFL);
  fcntl(fileno(stdin), F_SETFL, O_NONBLOCK);

  printf("    - Press axis to use as x-plane (left/right)\n");
  sdl_config_joypad_axis(which, J_PXAXIS);

  printf("    - Press axis to use as y-plane (up/down)\n");
  sdl_config_joypad_axis(which, J_PYAXIS);

  printf("    - Press button to use for 'RUN'\n");
  sdl_config_joypad_button(which, J_PSTART);

  printf("    - Press button to use for 'SELECT'\n");
  sdl_config_joypad_button(which, J_PSELECT);

  printf("    - Press button to use for 'I' (right-most button on pce pad)\n");
  sdl_config_joypad_button(which, J_PI);

  printf("    - Press button to use for autofire toggle on 'I'\n");
  sdl_config_joypad_button(which, J_PAUTOI);

  printf("    - Press button to use for 'II' (left-most button on pce pad)\n");
  sdl_config_joypad_button(which, J_PII);

  printf("    - Press button to use for autofire toggle on 'II'\n");
  sdl_config_joypad_button(which, J_PAUTOII);

  printf("                    Player %d\n"
         "      *------------------------------------*\n"
         "      |    %2d                    %2d   %2d   |\n"
         "      |    __                  Turbo Turbo |\n"
         "      |    ||                    __   __   |\n"
         "      |  |====| %2d              (%2d) (%2d)  |\n"
         "      |    ||      [%2d]  [%2d]    ~~   ~~   |\n"
         "      |    ~~    Select  Run     II    I   |\n"
         "      *------------------------------------*\n\n", which + 1,
         config[current_config].joy_mapping[which][J_PYAXIS], config[current_config].joy_mapping[which][J_PAUTOII],
         config[current_config].joy_mapping[which][J_PAUTOI], config[current_config].joy_mapping[which][J_PXAXIS],
         config[current_config].joy_mapping[which][J_PII], config[current_config].joy_mapping[which][J_PI],
         config[current_config].joy_mapping[which][J_PSELECT],
         config[current_config].joy_mapping[which][J_PSTART]);

  fcntl(fileno(stdin), F_SETFL, sa);
}


void sdl_init_joypads(void)
{
  int n;

  for (n = 0; n < 5; n++)
  {
    if (joypad_devs[n] == 0)
    {
      joypad[n] = NULL;
      continue;
    }

    if ((joypad[n] = SDL_JoystickOpen(joypad_devs[n] - 1)) == NULL)
    {
      printf("SDL could not open system joystick device %d (%s)\n", joypad_devs[n] - 1, SDL_GetError());
      continue;
    }

    printf(" * PCE joypad %d: %s, %d axes, %d buttons\n", n + 1, SDL_JoystickName(joypad_devs[n] - 1),
           SDL_JoystickNumAxes(joypad[n]), SDL_JoystickNumButtons(joypad[n]));

    if (!config[current_config].joy_mapping[n][J_PXAXIS] && !config[current_config].joy_mapping[n][J_PYAXIS] &&
        !config[current_config].joy_mapping[n][J_PSTART] && !config[current_config].joy_mapping[n][J_PSELECT] &&
        !config[current_config].joy_mapping[n][J_PI] && !config[current_config].joy_mapping[n][J_PII] &&
        !config[current_config].joy_mapping[n][J_PAUTOI] && !config[current_config].joy_mapping[n][J_PAUTOII])
      sdl_config_joypad(n);
  }
}

#endif


UInt16 read_joypad(int port)
{
  UInt16 tmp = 0;

#ifndef SDL
  poll_joypad(port);

  if (joypad[port].axis[0] < 0)
    tmp |= JOY_LEFT;
  else if (joypad[port].axis[0] > 0)
    tmp |= JOY_RIGHT;

  if (joypad[port].axis[1] < 0)
    tmp |= JOY_UP;
  else if (joypad[port].axis[1] > 0)
    tmp |= JOY_DOWN;

  if (joypad[port].button[config[current_config].joy_mapping[port][J_PSTART]])
    tmp |= JOY_START;

  if (joypad[port].button[config[current_config].joy_mapping[port][J_PSELECT]])
    tmp |= JOY_SELECT;

  // Set Autofire I to Autofire II button status
  config[current_config].autoI[port] = joypad[port].button[config[current_config].joy_mapping[port][J_PAUTOI]];

  // Set Autofire II to Autofire II button status
  config[current_config].autoII[port] = joypad[port].button[config[current_config].joy_mapping[port][J_PAUTOII]];

  // Button I, fixed Autofire
  if (joypad[port].button[config[current_config].joy_mapping[port][J_PI]])
  {
    if (config[current_config].autoI[port])
    {
      config[current_config].firedI[port] = !config[current_config].firedI[port];
      if (!config[current_config].firedI[port])
        tmp |= JOY_A;
    }
    else
      tmp |= JOY_A;
  }

  // Button II, fixed Autofire
  if (joypad[port].button[config[current_config].joy_mapping[port][J_PII]])
  {
    if (config[current_config].autoII[port])
    {
      config[current_config].firedII[port] = !config[current_config].firedII[port];
      if (!config[current_config].firedII[port])
        tmp |= JOY_B;
    }
    else
      tmp |= JOY_B;
  }

#else

  if (joypad[port] != NULL)
  {
    int pos;

    pos = SDL_JoystickGetAxis(joypad[port], config[current_config].joy_mapping[port][J_PXAXIS]);

    if (pos < -16384)
      tmp |= JOY_LEFT;
    else if (pos > 16383)
      tmp |= JOY_RIGHT;

    pos = SDL_JoystickGetAxis(joypad[port], config[current_config].joy_mapping[port][J_PYAXIS]);

    if (pos < -16384)
      tmp |= JOY_UP;
    else if (pos > 16383)
      tmp |= JOY_DOWN;

    if (SDL_JoystickGetButton(joypad[port], config[current_config].joy_mapping[port][J_PSTART]))
      tmp |= JOY_START;

    if (SDL_JoystickGetButton(joypad[port], config[current_config].joy_mapping[port][J_PSELECT]))
      tmp |= JOY_SELECT;

    config[current_config].autoI[port] = SDL_JoystickGetButton(joypad[port], config[current_config].joy_mapping[port][J_PAUTOI]);
    config[current_config].autoII[port] = SDL_JoystickGetButton(joypad[port], config[current_config].joy_mapping[port][J_PAUTOII]);

    if (SDL_JoystickGetButton(joypad[port], config[current_config].joy_mapping[port][J_PI]))
    {
      if (config[current_config].autoI[port])
      {
        config[current_config].firedI[port] = !config[current_config].firedI[port];
        if (!config[current_config].firedI[port])
          tmp |= JOY_A;
      }
      else
        tmp |= JOY_A;
    }

    if (SDL_JoystickGetButton(joypad[port], config[current_config].joy_mapping[port][J_PII]))
    {
      if (config[current_config].autoII[port])
      {
        config[current_config].firedII[port] = !config[current_config].firedII[port];
        if (!config[current_config].firedII[port])
          tmp |= JOY_B;
      }
      else
        tmp |= JOY_B;
    }
  }

#endif

  return tmp;
}


UInt16 joypad1(void)
{
  return read_joypad(0);
}

UInt16 joypad2(void)
{
  return read_joypad(1);
}

UInt16 joypad3(void)
{
  return read_joypad(2);
}

UInt16 joypad4(void)
{
  return read_joypad(3);
}


/* for mouse */
UInt16 mouse1(void)
{
  return 0;
}

UInt16 mouse2(void)
{
  return 0;
}


/* for synaptic link */
UInt16 synaplink(void)
{
  return 0;
}


#ifdef ALLEGRO

int osd_keyboard(void)
{

// char tmp_joy;


	while (key[KEY_PAUSE])
		pause ();

	if (key[KEY_ASTERISK])
	{
		UInt32 sav_timer = timer_60;
		if (sound_driver == 1)
			osd_snd_set_volume (0);
		disass_menu ();
		if (sound_driver == 1)
			osd_snd_set_volume (gen_vol);
		timer_60 = sav_timer;
		key_delay = 10;

		break;
	}
	if ((key[KEY_F4]) && (!key_delay))

	{
		UInt32 sav_timer = timer_60;
		UChar error_code;
		error_code = gui ();
		key_delay = 10;
		timer_60 = sav_timer;
		if (error_code)
			return 1;

		break;
	}
	if ((key[KEY_ESC]) && (!key_delay))

	{
		UInt32 sav_timer = timer_60;

/*
     old_vol=voice_get_volume(PCM_voice);
     voice_set_volume(PCM_voice,0);
*/
		if (sound_driver == 1)
			osd_snd_set_volume (0);

		//menu(); //There will be the GUI
		while (keypressed ())
			readkey ();
		select_rom ("*.pce");
		while (keypressed ())
			readkey ();
		key_delay = 10;
		timer_60 = sav_timer;
		if (!strcmp ("TRUE EXIT", selected_rom))

		{
			cart_reload = 0;
			return 1;
		}

/*
     voice_set_volume(PCM_voice,old_vol);
*/
		if (sound_driver == 1)
			osd_snd_set_volume (gen_vol);
		if (strcmp ("NO FILE", selected_rom))

		{
			cart_reload = 1;
			strcpy (cart_name, selected_rom);
			return 1;
		}
	}
	if (key[KEY_F12])
		return 1;
	if (!key_delay)

	{
		if (key[KEY_F6])

		{
			UInt32 sav_timer = timer_60;

			{

//            AGetVoiceVolume(hVoice,&vol);
//            ASetVoiceVolume(hVoice,0);
				if (!silent)

				{

/*
         voice_stop(PCM_voice);
         set_volume(1,1);
         voice_set_volume(PCM_voice,1);
         voice_start(PCM_voice);
*/
					if (sound_driver == 1)
						osd_snd_set_volume (0);
				}
			}
			if (!savegame ())

			{
				osd_gfx_set_message (MESSAGE[language]
						     [game_save]);
				message_delay = 180;
			}
			if (!silent)

			{
				if (sound_driver == 1)
					osd_snd_set_volume (gen_vol);
			}
			timer_60 = sav_timer;
		};
		if (key[KEY_F5])

		{
			UInt32 sav_timer = timer_60;
			sprintf (tmp_buf, MESSAGE[language][screen_shot],
				 osd_gfx_savepict ());
			osd_gfx_set_message (tmp_buf);
			message_delay = 180;
			key_delay = 10;
			timer_60 = sav_timer;
		};
		if (key[KEY_F10])

		{
			synchro = !synchro;
			osd_gfx_set_message (MESSAGE[language]
					     [full_speed + synchro]);
			message_delay = 180;
			key_delay = 10;
		};
		if (key[KEY_NUMLOCK])

		{
			if (dump_snd)

			{
				UInt32 dummy;
				dummy = filesize (out_snd);
				fseek (out_snd, 4, SEEK_SET);
				fwrite (&dummy, 1, 4, out_snd);
				dummy -= 0x2C;
				fseek (out_snd, 0x28, SEEK_SET);
				fwrite (&dummy, 1, 4, out_snd);
				fclose (out_snd);
				osd_gfx_set_message (MESSAGE[language]
						     [dump_off]);
			}

			else

			{
				unsigned short tmp = 0;
				strcpy (tmp_buf, "SND0000.WAV");
				while ((tmp < 0xFFFF)
				       && ((out_snd = fopen (tmp_buf, "rb"))
					   != NULL))
				{
					fclose (out_snd);
					sprintf (tmp_buf, "SND%04X.WAV",
						 ++tmp);
				}
				out_snd = fopen (tmp_buf, "wb");
				fwrite ("RIFF\145\330\073\0WAVEfmt ", 16, 1,
					out_snd);
				putc (0x10, out_snd);	// size
				putc (0x00, out_snd);
				putc (0x00, out_snd);
				putc (0x00, out_snd);
				putc (1, out_snd);	// PCM data
				putc (0, out_snd);
				putc (1, out_snd);	// mono
				putc (0, out_snd);
				putc (freq_int, out_snd);	// frequency
				putc (freq_int >> 8, out_snd);
				putc (freq_int >> 16, out_snd);
				putc (freq_int >> 24, out_snd);
				putc (freq_int, out_snd);	// frequency
				putc (freq_int >> 8, out_snd);
				putc (freq_int >> 16, out_snd);
				putc (freq_int >> 24, out_snd);
				putc (1, out_snd);
				putc (0, out_snd);
				putc (8, out_snd);	// 8 bits
				putc (0, out_snd);
				fwrite ("data\377\377\377\377", 1, 9,
					out_snd);
				osd_gfx_set_message (MESSAGE[language]
						     [dump_on]);
			}
			dump_snd = !dump_snd;
			key_delay = 10;
			message_delay = 180;
		}
		if (key[KEY_PLUS_PAD])

		{
			if (UPeriod < 15)

			{
				UPeriod++;
				sprintf (tmp_buf,
					 MESSAGE[language][frame_skip],
					 UPeriod);
				osd_gfx_set_message (tmp_buf);
				message_delay = 180;
			};
			key_delay = 10;
		};
		if (key[KEY_MINUS_PAD])

		{
			if (UPeriod)

			{
				UPeriod--;
				if (!UPeriod)
					osd_gfx_set_message (MESSAGE[language]
							     [all_frame]);

				else

				{
					sprintf (tmp_buf,
						 MESSAGE[language]
						 [frame_skip], UPeriod);
					osd_gfx_set_message (tmp_buf);
				};
				message_delay = 180;
			};
			key_delay = 10;
		};
		if (key[KEY_1])

		{
			SPONSwitch = !SPONSwitch;
			key_delay = 10;
		}
		if (key[KEY_2])

		{
			BGONSwitch = !BGONSwitch;
			key_delay = 10;
		}
	}
	if (key[KEY_F1])

	{
		UInt32 sav_timer = timer_60;
		if (sound_driver == 1)
			osd_snd_set_volume (0);
		searchbyte ();
		if (sound_driver == 1)
			osd_snd_set_volume (gen_vol);
		timer_60 = sav_timer;
		return 0;
	}
	if (key[KEY_F2])

	{
		UInt32 sav_timer = timer_60;
		if (sound_driver == 1)
			osd_snd_set_volume (0);
		pokebyte ();
		if (sound_driver == 1)
			osd_snd_set_volume (gen_vol);
		timer_60 = sav_timer;
		return 0;
	}
	if (key[KEY_F7])

	{
		UInt32 sav_timer = timer_60;
		if (!loadgame ())

		{
			osd_gfx_set_message (MESSAGE[language][game_load]);
			message_delay = 180;
		}
		timer_60 = sav_timer;
	};
	if (key[KEY_F3])

	{
		UInt32 sav_timer = timer_60;
		if (sound_driver == 1)
			osd_snd_set_volume (0);
		freeze_value ();
		if (sound_driver == 1)
			osd_snd_set_volume (gen_vol);
		timer_60 = sav_timer;
		return 0;
	}
	if (key[KEY_TILDE])

	{
		char *tmp = (char *) alloca (100);
		sprintf (tmp, "FRAME DELTA = %d",
			 frame - HCD_frame_at_beginning_of_track);
		osd_gfx_set_message (tmp);
		message_delay = 240;
	}
	if (key[KEY_3])

		//cd_port_1800 = 0xD0;
		cd_port_1800 &= ~0x40;
	if (key[KEY_4])
		_Wr6502 (0x2066, Rd6502 (0x2066) | 32);
	if (key[KEY_5])
		cd_port_1800 = 0xD8;
	if (key[KEY_6])
		_Wr6502 (0x22D6, 1);
	if (key[KEY_8])
		_Wr6502 (0x20D8, 128);

	//_Wr6502(0x20D8,Rd6502(0x20D8) | 128);
	if (key[KEY_MINUS])

	{
		if (gen_vol)
			gen_vol--;
		if (sound_driver == 1)

		{
			osd_snd_set_volume (gen_vol);
			sprintf (tmp_buf, MESSAGE[language][volume_set_to],
				 gen_vol);
			osd_gfx_set_message (tmp_buf);
			message_delay = 60;
		}
	}
	if (key[KEY_EQUALS])

	{
		if (gen_vol < 255)
			gen_vol++;
		if (sound_driver == 1)

		{
			osd_snd_set_volume (gen_vol);
			sprintf (tmp_buf, MESSAGE[language][volume_set_to],
				 gen_vol);
			osd_gfx_set_message (tmp_buf);
			message_delay = 60;
		}
	}
	if (key[KEY_0])

	{
		if (freq_int < 44100)
			freq_int += 50;
		if (sound_driver == 1)

		{
			voice_set_frequency (PCM_stream->voice, freq_int);
			sprintf (tmp_buf, MESSAGE[language][freq_set_to],
				 freq_int);
			osd_gfx_set_message (tmp_buf);
			message_delay = 60;
		}
	}
	if (key[KEY_9])

	{
		if (freq_int > 11025)
			freq_int -= 50;
		if (sound_driver == 1)

		{
			voice_set_frequency (PCM_stream->voice, freq_int);
			sprintf (tmp_buf, MESSAGE[language][freq_set_to],
				 freq_int);
			osd_gfx_set_message (tmp_buf);
			message_delay = 60;
		}
	}
	io.JOY[0] = read_input(0);
	io.JOY[1] = read_input(1);
	io.JOY[2] = read_input(2);
	io.JOY[3] = read_input(3);
	io.JOY[4] = read_input(4);
	return 0;
}

#elif SDL

int osd_keyboard(void)
{
// char tmp_joy;

	SDL_Event event;

#warning need for pause support
				/* 
				 * while (key[KEY_PAUSE])
				 * pause ();
				 */

	while (SDL_PollEvent (&event))
	{

		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
				
			case SDLK_ASTERISK:
			{
				UInt32 sav_timer = timer_60;
				if (sound_driver == 1)
					osd_snd_set_volume (0);
				disass_menu ();
				if (sound_driver == 1)
					osd_snd_set_volume (gen_vol);
				timer_60 = sav_timer;
				key_delay = 10;
				break;
			}

			case SDLK_F4:
			{
				if (key[SDLK_LSHIFT]) {
					io.psg_channel_disabled[3] = !io.psg_channel_disabled[3];					
				}
				break;
			}
			
			case SDLK_F9:				
				SDL_ShowCursor(ToggleFullScreen());
				break;

#warning reenable file selection on the fly
/*
  	case SDLK_ESCAPE:    
    {
      UInt32 sav_timer = timer_60;      
	if (sound_driver == 1)
	osd_snd_set_volume (0);
	
      select_rom ("*.pce");
	
      key_delay = 10;
      timer_60 = sav_timer;
      if (!strcmp ("TRUE EXIT", selected_rom))	
	{
	  cart_reload = 0;
	  return 1;
	}
      
	if (sound_driver == 1)
	osd_snd_set_volume (gen_vol);
      if (strcmp ("NO FILE", selected_rom))
	
	{
	  cart_reload = 1;
	  strcpy (cart_name, selected_rom);
	  return 1;
	}
    break;}
*/
			case SDLK_F12:
			case SDLK_ESCAPE:
				return 1;

			case SDLK_F6:
			{
				if (key[SDLK_LSHIFT]) {
					io.psg_channel_disabled[5] = !io.psg_channel_disabled[5];
				} else {
					
					
					
					UInt32 sav_timer = timer_60;
					{
	//            AGetVoiceVolume(hVoice,&vol);
	//            ASetVoiceVolume(hVoice,0);
						if (!silent)
						{
	/*
			 voice_stop(PCM_voice);
			 set_volume(1,1);
			 voice_set_volume(PCM_voice,1);
			 voice_start(PCM_voice);
	*/
							if (sound_driver == 1)
								osd_snd_set_volume
									(0);
						}
					}
					if (!savegame ())
	
					{
						osd_gfx_set_message (MESSAGE[language]
									 [game_save]);
						message_delay = 180;
					}
					if (!silent)
	
					{
						if (sound_driver == 1)
							osd_snd_set_volume (gen_vol);
					}
					timer_60 = sav_timer;
				}
				break;
			}

			case SDLK_F5:

			{
				if (key[SDLK_LSHIFT]) {
					io.psg_channel_disabled[4] = !io.psg_channel_disabled[4];
				} else {					
					UInt32 sav_timer = timer_60;
					sprintf (tmp_buf,
						 MESSAGE[language][screen_shot],
						 osd_gfx_savepict ());
					osd_gfx_set_message (tmp_buf);
					message_delay = 180;
					key_delay = 10;
					timer_60 = sav_timer;
				}
				break;
			}
			case SDLK_F10:
			{
				synchro = !synchro;
				osd_gfx_set_message (MESSAGE[language]
						     [full_speed + synchro]);
				message_delay = 180;
				key_delay = 10;
				break;
			}

			case SDLK_NUMLOCK:
			{
				if (dump_snd)

				{
					UInt32 dummy;
					dummy = filesize (out_snd);
					fseek (out_snd, 4, SEEK_SET);
					fwrite (&dummy, 1, 4, out_snd);
					dummy -= 0x2C;
					fseek (out_snd, 0x28, SEEK_SET);
					fwrite (&dummy, 1, 4, out_snd);
					fclose (out_snd);
					osd_gfx_set_message (MESSAGE[language]
							     [dump_off]);
				}

				else

				{
					unsigned short tmp = 0;
					strcpy (tmp_buf, "SND0000.WAV");
					while ((tmp < 0xFFFF)
					       &&
					       ((out_snd =
						 fopen (tmp_buf,
							"rb")) != NULL))
					{
						sprintf (tmp_buf,
							 "SND%04X.WAV",
							 ++tmp);
						fclose (out_snd);
					}
					out_snd = fopen (tmp_buf, "wb");
					fwrite ("RIFF\145\330\073\0WAVEfmt ",
						16, 1, out_snd);
					putc (0x10, out_snd);	// size
					putc (0x00, out_snd);
					putc (0x00, out_snd);
					putc (0x00, out_snd);
					putc (1, out_snd);	// PCM data
					putc (0, out_snd);
					putc (1, out_snd);	// mono
					putc (0, out_snd);
					putc (freq_int, out_snd);	// frequency
					putc (freq_int >> 8, out_snd);
					putc (freq_int >> 16, out_snd);
					putc (freq_int >> 24, out_snd);
					putc (freq_int, out_snd);	// frequency
					putc (freq_int >> 8, out_snd);
					putc (freq_int >> 16, out_snd);
					putc (freq_int >> 24, out_snd);
					putc (1, out_snd);
					putc (0, out_snd);
					putc (8, out_snd);	// 8 bits
					putc (0, out_snd);
					fwrite ("data\377\377\377\377", 1, 9,
						out_snd);
					osd_gfx_set_message (MESSAGE[language]
							     [dump_on]);
				}
				dump_snd = !dump_snd;
				key_delay = 10;
				message_delay = 180;
				break;
			}

			case SDLK_KP_PLUS:
			{
				if (UPeriod < 15)

				{
					UPeriod++;
					sprintf (tmp_buf,
						 MESSAGE[language]
						 [frame_skip], UPeriod);
					osd_gfx_set_message (tmp_buf);
					message_delay = 180;
				};
				key_delay = 10;
				break;
			}

			case SDLK_KP_MINUS:
			{
				if (UPeriod)

				{
					UPeriod--;
					if (!UPeriod)
						osd_gfx_set_message (MESSAGE
								     [language]
								     [all_frame]);

					else

					{
						sprintf (tmp_buf,
							 MESSAGE[language]
							 [frame_skip],
							 UPeriod);
						osd_gfx_set_message (tmp_buf);
					};
					message_delay = 180;
				};
				key_delay = 10;
			}
				break;

			case SDLK_1:

			{
				if (key[SDLK_LSHIFT])
				{
					io.psg_channel_disabled[0] = !io.psg_channel_disabled[0];
				}
				else
				{
					SPONSwitch = !SPONSwitch;
					key_delay = 10;
				}
				break;
			}

			case SDLK_2:
			{
				if (key[SDLK_LSHIFT])
				{
					io.psg_channel_disabled[1] = !io.psg_channel_disabled[1];
				}
				else
				{
					BGONSwitch = !BGONSwitch;
					key_delay = 10;
				}
				break;
			}
			
			case SDLK_F1:
			{
				if (key[SDLK_LSHIFT]) {
					io.psg_channel_disabled[0] = !io.psg_channel_disabled[0];					
				} else {				
					UInt32 sav_timer = timer_60;
					if (sound_driver == 1)
						osd_snd_set_volume (0);
					searchbyte ();
					if (sound_driver == 1)
						osd_snd_set_volume (gen_vol);
					timer_60 = sav_timer;
					return 0;
				}
				break;
			}

			case SDLK_F2:

			{
				if (key[SDLK_LSHIFT]) {
					io.psg_channel_disabled[1] = !io.psg_channel_disabled[1];
				} else {								
					UInt32 sav_timer = timer_60;
					if (sound_driver == 1)
						osd_snd_set_volume (0);
					pokebyte ();
					if (sound_driver == 1)
						osd_snd_set_volume (gen_vol);
					timer_60 = sav_timer;
					return 0;
				}
				break;
			}
			case SDLK_F7:

			{
				UInt32 sav_timer = timer_60;
				if (!loadgame ())

				{
					osd_gfx_set_message (MESSAGE[language]
							     [game_load]);
					message_delay = 180;
				}
				timer_60 = sav_timer;
				break;
			}
			case SDLK_F3:

			{
				if (key[SDLK_LSHIFT]) {
					io.psg_channel_disabled[2] = !io.psg_channel_disabled[2];
				} else {								
					UInt32 sav_timer = timer_60;
					if (sound_driver == 1)
						osd_snd_set_volume (0);
					freeze_value ();
					if (sound_driver == 1)
						osd_snd_set_volume (gen_vol);
					timer_60 = sav_timer;
					return 0;
				}
				break;
			}

			case SDLK_BACKQUOTE: /* TILDE */
			{
				char *tmp = (char *) alloca (100);
				sprintf(tmp, "FRAME DELTA = %d", frame - HCD_frame_at_beginning_of_track);
				osd_gfx_set_message (tmp);
				message_delay = 240;
				break;
			}

			case SDLK_3:
				if (key[SDLK_LSHIFT])
				{
					io.psg_channel_disabled[2] = !io.psg_channel_disabled[2];
				}
				else
				{
					//cd_port_1800 = 0xD0;
					cd_port_1800 &= ~0x40;
				}
				break;

			case SDLK_4:
				if (key[SDLK_LSHIFT])
				{
					io.psg_channel_disabled[3] = !io.psg_channel_disabled[3];
				}
				else
				{
					_Wr6502 (0x2066, Rd6502 (0x2066) | 32);
				}
				break;
			case SDLK_5:
				if (key[SDLK_LSHIFT])
				{
					io.psg_channel_disabled[4] = !io.psg_channel_disabled[4];
				}
				else
				{
					cd_port_1800 = 0xD8;
				}
				break;
			case SDLK_6:
				if (key[SDLK_LSHIFT])
				{
					io.psg_channel_disabled[5] = !io.psg_channel_disabled[5];
				}
				else
				{
					_Wr6502 (0x22D6, 1);
				}
				break;

			case SDLK_8:
				_Wr6502 (0x20D8, 128);
				break;

			case SDLK_MINUS:
			{
				if (gen_vol)
					gen_vol--;
				if (sound_driver == 1)

				{
					osd_snd_set_volume (gen_vol);
					sprintf (tmp_buf,
						 MESSAGE[language]
						 [volume_set_to], gen_vol);
					osd_gfx_set_message (tmp_buf);
					message_delay = 60;
				}

				break;
			}
			case SDLK_EQUALS:
			{
				if (gen_vol < 255)
					gen_vol++;
				if (sound_driver == 1)

				{
					osd_snd_set_volume (gen_vol);
					sprintf (tmp_buf,
						 MESSAGE[language]
						 [volume_set_to], gen_vol);
					osd_gfx_set_message (tmp_buf);
					message_delay = 60;
				}

				break;
			}
			case SDLK_0:
			{
				if (freq_int < 44100)
					freq_int += 50;
				if (sound_driver == 1)

				{
#warning need to implement downmixing
					// voice_set_frequency (PCM_stream->voice, freq_int);
					sprintf (tmp_buf,
						 MESSAGE[language]
						 [freq_set_to], freq_int);
					osd_gfx_set_message (tmp_buf);
					message_delay = 60;
				}
				break;
			}
			case SDLK_9:
			{
				if (freq_int > 11025)
					freq_int -= 50;
				if (sound_driver == 1)

				{
#warning need to implement upmixing
					//voice_set_frequency (PCM_stream->voice, freq_int);
					sprintf (tmp_buf,
						 MESSAGE[language]
						 [freq_set_to], freq_int);
					osd_gfx_set_message (tmp_buf);
					message_delay = 60;
				}
				break;
			}
			}
			break;
		case SDL_QUIT:
			return 1;
		}
	}

	io.JOY[0] = read_input(0);	
	io.JOY[1] = read_input(1);
	io.JOY[2] = read_input(2);
	io.JOY[3] = read_input(3);
	io.JOY[4] = read_input(4);

	return 0;
}


int osd_init_input(void)
{
  if (SDL_InitSubSystem(SDL_INIT_JOYSTICK))
    return -1;

  sdl_init_joypads();

  return 0;
}


#endif // if ALLEGRO elif SDL

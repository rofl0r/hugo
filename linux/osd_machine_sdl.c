#include "osd_machine_sdl.h"

char initial_path[128] = "";
// prefered path for for searching

UChar* osd_gfx_buffer = NULL;

UChar gamepad = 0;
// gamepad detected ?

UChar* XBuf;
// The screen buffer where we draw before blitting it on screen

int gamepad_driver = 0;
// what kind of jypad must we have to handle

char dump_snd = 0;
// Do we write sound to file

char synchro;
// � fond, � fond, � fond? (french joke ;)

int vwidth, vheight;
// size of visible part of the screen (I got troubles with allegro screen->* values!)

int vmode = 0;
// video mode to use
// 1 -> first 224*240 then 256*240 then 256*256 then 320*240 and 320*200
// 2 -> first 256*240 then 256*256 then 320*240 and 320*200
// 3 -> first 256*256 then 320*240 and 320*200
// 4 -> first 320*240 and 320*200
// 5 -> only try 320*200
// 6 -> only 800*600
// If you think others modes would be interesting, let me know

int *fd[4];
// handle for joypad devices

int osd_init_machine()
{

  int result;

  Log ("\n--[ INITIALISE MACHINE ]--------------------------\n");
		
  if (SDL_Init (SDL_INIT_VIDEO)) {
	  Log("Could not initialise SDL : %s\n",SDL_GetError());
	  return 0;
  }	

  printf (MESSAGE[language][init_allegro]);

  printf (MESSAGE[language][translated_by]);

  if (!(XBuf = (UChar*)malloc(XBUF_WIDTH* XBUF_HEIGHT)))
    {
      printf (MESSAGE[language][failed_init]);
      return (0);
    }

  printf (MESSAGE[language][clear_buffer]);
  bzero (XBuf, XBUF_WIDTH * XBUF_HEIGHT);

  Log ("Initiating sound\n");
  printf (MESSAGE[language][init_sound]);
  InitSound ();

  /* Opening joypad number 0 */
  (int)fd[0] = open ("/dev/js0", O_NONBLOCK);

#warning enable eagle with sdl
/*
  if (use_eagle)
    {
      printf (MESSAGE[language][eagle_asked]);
      if (!set_gfx_mode (GFX_AUTODETECT, 640, 480, 0, 0))
	{
	  vwidth = 640;
	  vheight = 480;
	  blit_x = (320 - 256) / 2;
	  blit_y = (240 - 216) / 2;
	  screen_blit_x = (WIDTH - io.screen_h) / 2;
	  screen_blit_y = (HEIGHT - io.screen_w) / 2;
	  SetPalette ();

	  EAGLE_buf = create_bitmap (640, 480);
	}
      else
	printf (MESSAGE[language][eagle_mode_not_init]);
    }
  else
  */
  
  if (!(*osd_gfx_driver_list[video_driver].init) ())
    {
      Log ("Can't set graphic mode\n");
      printf (MESSAGE[language][cant_set_gmode]);
      return 0;
    }

  osd_gfx_buffer = XBuf;

  Log ("End of initialisation of the machine\n");
  return 1;

}

/*****************************************************************************

    Function: osd_shut_machine

    Description: Deinitialize all stuff that have been inited in osd_int_machine
    Parameters: none
    Return: nothing

*****************************************************************************/
void
osd_shut_machine (void)
{
 
	free(XBuf);

#warning enable eagle with sdl
/*	
  if (EAGLE_buf)
    destroy_bitmap (EAGLE_buf);
*/
  
  if (sound_driver == 1)
    osd_snd_set_volume (0);

  /* closing joypad device */
  close ((int)fd[0]);
    
/*  (*fade_out_proc[rand () % nb_fadeout]) (0, 0, vwidth, vheight); */

  TrashSound ();

  SDL_Quit();
  
  return;
}

/*****************************************************************************

    Function: osd_keypressed

    Description: Tells if a key is available for future call of osd_readkey
    Parameters: none
    Return: 0 is no key is available
            else any non zero value

*****************************************************************************/
SChar osd_keypressed(void)
{

#warning implement keypressed with sdl	
	
 }

/*****************************************************************************

    Function: osd_readkey

    Description: Return the first available key stroke, waiting if needed
    Parameters: none
    Return: the key value (currently, lower byte is ascii and higher is scancode)

*****************************************************************************/
UInt16 osd_readkey(void)
{
	SDL_Event event;
	while ( SDL_PollEvent( &event ))
	{
		switch (event.type)
		{
			case SDL_KEYDOWN:
				return event.key.keysym.unicode;
			case SDL_QUIT:
				return 0;
		}
	}
 }

 /*****************************************************************************

    Function: osd_fix_filename_slashes

    Description: Changes slashes in a filename to correspond to an os need
    Parameters: char* s
    Return: nothing but the char* is updated

*****************************************************************************/
void osd_fix_filename_slashes(char* s)
{
	while (*s)
	{
		if (*s == '\\')
			*s = '/';
		s++;
	}
}
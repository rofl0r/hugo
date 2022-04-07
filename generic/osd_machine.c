#include "osd_machine.h"

UChar* osd_gfx_buffer = NULL;

UChar gamepad = 0;
// gamepad detected ?

int gamepad_driver = 0;
// what kind of jypad must we have to handle

char dump_snd = 0;
// Do we write sound to file

char synchro;
// à fond, à fond, à fond? (french joke ;)

int vwidth, vheight;
// size of visible part of the screen (I got troubles with allegro screen->* values!)

SInt32 vmode = 0;
// video mode to use
// 1 -> first 224*240 then 256*240 then 256*256 then 320*240 and 320*200
// 2 -> first 256*240 then 256*256 then 320*240 and 320*200
// 3 -> first 256*256 then 320*240 and 320*200
// 4 -> first 320*240 and 320*200
// 5 -> only try 320*200
// 6 -> only 800*600
// If you think others modes would be interesting, let me know

int osd_init_machine()
{

  int result;

  Log ("\n--[ INITIALISE MACHINE ]--------------------------\n");

  /*
  set_gfx_mode (GFX_TEXT, 0, 0, 0, 0);
  printf (MESSAGE[language][init_allegro]);

*/
  printf (MESSAGE[language][translated_by]);

  /*
  if (!(XBuf = create_bitmap (XBUF_WIDTH, XBUF_HEIGHT)))
    {
      printf (MESSAGE[language][failed_init]);
      return (0);
    }

  printf (MESSAGE[language][clear_buffer]);
  clear (XBuf);

  Log ("Allocating sprite array\n");
  printf (MESSAGE[language][alloc_spr]);
  {
    UInt32 x;
    for (x = 0; x < VRAMSIZE / 32; x++)
      {
	if (!(dirty_spr[x] = create_bitmap (16, 16)))
	  {
	    printf (MESSAGE[language][failed_init]);
	    return 0;
	  }
	clear (dirty_spr[x]);
      }
  }
*/

  Log ("Initiating sound\n");
  printf (MESSAGE[language][init_sound]);
  InitSound ();

  if (!(*osd_gfx_driver_list[video_driver].init) ())
    {
      Log ("Can't set graphic mode\n");
      printf (MESSAGE[language][cant_set_gmode]);
      return 0;
    }
/*
  text_mode (-1);
  install_keyboard ();
  clear (screen);

  osd_gfx_buffer = XBuf->line[0];
*/

  osd_gfx_buffer = malloc(XBUF_WIDTH * XBUF_HEIGHT);

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


  free(osd_gfx_buffer);

  (*fade_out_proc[rand () % nb_fadeout]) (0, 0, vwidth, vheight);

  TrashSound ();

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
 return 0;
 }

/*****************************************************************************

    Function: osd_readkey

    Description: Return the first available key stroke, waiting if needed
    Parameters: none
    Return: the key value (currently, lower byte is ascii and higher is scancode)

*****************************************************************************/
UInt16 osd_readkey(void)
{
 return getch();
 }


#include "osd_linux_sdl_gfx.h"

//! PC Engine rendered screen
SDL_Surface *screen = NULL;

//! Host machine rendered screen
SDL_Surface *physical_screen;

SDL_Rect physical_screen_rect;

int blit_x,blit_y;
// where must we blit the screen buffer on screen

int screen_blit_x, screen_blit_y;
// where on the screen we must blit XBuf

UChar* XBuf;
// buffer for video flipping

UChar index_to_RGB[256];
// convertion array from bero's reduced pce palette to x11 palette

int osd_gfx_init();
int osd_gfx_init_normal_mode();
void osd_gfx_put_image_normal();
void osd_gfx_shut_normal_mode();

void osd_gfx_dummy_func();

osd_gfx_driver osd_gfx_driver_list[3] =
{
  { osd_gfx_init, osd_gfx_init_normal_mode, osd_gfx_put_image_normal, osd_gfx_shut_normal_mode },
  { osd_gfx_init, osd_gfx_init_normal_mode, osd_gfx_put_image_normal, osd_gfx_shut_normal_mode },
  { osd_gfx_init, osd_gfx_init_normal_mode, osd_gfx_put_image_normal, osd_gfx_shut_normal_mode }
};

void osd_gfx_dummy_func(void)
{
 return;
 }

 void DrawPixel(SDL_Surface *screen, int x, int y,
                                    Uint8 R, Uint8 G, Uint8 B)
{
  Uint32 color = SDL_MapRGB(screen->format, R, G, B);
  switch (screen->format->BytesPerPixel)
  {
    case 1: // Assuming 8-bpp
      {
        Uint8 *bufp;
        bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
        *bufp = color;
      }
      break;
    case 2: // Probably 15-bpp or 16-bpp
      {
        Uint16 *bufp;
        bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
        *bufp = color;
      }
      break;
    case 3: // Slow 24-bpp mode, usually not used
      {
        Uint8 *bufp;
        bufp = (Uint8 *)screen->pixels + y*screen->pitch + x * 3;
        if(SDL_BYTEORDER == SDL_LIL_ENDIAN)
        {
          bufp[0] = color;
          bufp[1] = color >> 8;
          bufp[2] = color >> 16;
        } else {
          bufp[2] = color;
          bufp[1] = color >> 8;
          bufp[0] = color >> 16;
        }
      }
      break;
    case 4: // Probably 32-bpp
      {
        Uint32 *bufp;
        bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
        *bufp = color;
      }
      break;
  }
}
 
void Slock(SDL_Surface *screen)
{
  if ( SDL_MUSTLOCK(screen) )
  {
    if ( SDL_LockSurface(screen) < 0 )
    {
      return;
    }
  }
}

void Sulock(SDL_Surface *screen)
{
  if ( SDL_MUSTLOCK(screen) )
  {
    SDL_UnlockSurface(screen);
  }
}

/*****************************************************************************

    Function: osd_gfx_put_image_normal

    Description: draw the raw computed picture to screen, without any effect
       trying to center it (I bet there is still some work on this, maybe not
                            in this function)
    Parameters: none
    Return: nothing

*****************************************************************************/
void osd_gfx_put_image_normal(void)
{

  int dum;
	
	Slock(screen);
	
	for (dum = 0; dum < io.screen_h; dum++)
 	  memcpy(screen->pixels + dum * io.screen_w , XBuf + dum * XBUF_WIDTH + (WIDTH - io.screen_w) / 2, io.screen_w);	

	Sulock(screen);
	
	if (physical_screen->flags & SDL_FULLSCREEN)
	  SDL_SoftStretch(screen, NULL, physical_screen, &physical_screen_rect);
	else if (option.window_size > 1)
	  SDL_SoftStretch(screen, NULL, physical_screen, NULL);
	else
	  memmove(physical_screen->pixels, screen->pixels, io.screen_w * io.screen_h);
	
	SDL_Flip(physical_screen);
}

/*****************************************************************************

    Function: osd_gfx_set_message

    Description: compute the message that will be displayed to create a sprite
       to blit on screen
    Parameters: char* mess, the message to display
    Return: nothing but set OSD_MESSAGE_SPR

*****************************************************************************/
void osd_gfx_set_message(char* mess)
{
/*
 if (OSD_MESSAGE_SPR)
   destroy_bitmap(OSD_MESSAGE_SPR);

 OSD_MESSAGE_SPR=create_bitmap(text_length(font,mess)+1,text_height(font)+1);
 clear(OSD_MESSAGE_SPR);
 textout(OSD_MESSAGE_SPR,font,mess,1,1,3);
 textout(OSD_MESSAGE_SPR,font,mess,0,0,255);
*/
 
	 #warning implement set_message
  printf("%s\n",mess);
	
 return;

 }


/*
 * osd_gfx_init:
 *   One time initialization of the main output screen.
 */
int osd_gfx_init(void)
{
  io.screen_w = 256;
  io.screen_h = 224;

  if (SDL_InitSubSystem(SDL_INIT_VIDEO))
  {
    printf("SDL_InitSubSystem(VIDEO) failed at %s:%d - %s\n", __FILE__, __LINE__, SDL_GetError());
    return 0;
  }

  if ((physical_screen = SDL_SetVideoMode(option.want_fullscreen ? option.fullscreen_width : io.screen_w * option.window_size,
                                          option.want_fullscreen ? option.fullscreen_height : io.screen_h * option.window_size,
                                          8, (option.want_fullscreen ? SDL_FULLSCREEN : 0) |
                                          SDL_SWSURFACE | SDL_HWPALETTE)) == NULL)
  {
    printf("SDL_SetVideoMode failed at %s:%d - %s\n", __FILE__, __LINE__, SDL_GetError());
    return 0;
  }

  SDL_WM_SetCaption("Hu-Go! (www.zeograd.com)",NULL);	  
  
  if (option.want_fullscreen)
  {
    struct generic_rect rect;

    SDL_ShowCursor(SDL_DISABLE);

    calc_fullscreen_aspect(physical_screen->w, physical_screen->h, &rect);

    physical_screen_rect.x = rect.start_x;
    physical_screen_rect.y = rect.start_y;
    physical_screen_rect.w = rect.end_x;
    physical_screen_rect.h = rect.end_y;
  }

  SetPalette();

  if ((screen = SDL_CreateRGBSurface(SDL_SWSURFACE, io.screen_w, io.screen_h, 8, 0, 0, 0, 0)) == NULL)
  {
    printf("SDL_CreateRGBSurface failed at %s:%d - %s\n", __FILE__, __LINE__, SDL_GetError());
    return 0;
  }

  return 1;
}


/*****************************************************************************

    Function:  osd_gfx_init_normal_mode

    Description: initialize the classic 256*224 video mode for normal video_driver
    Parameters: none
    Return: 0 on error
            1 on success

*****************************************************************************/
int osd_gfx_init_normal_mode()
{
  if (screen != NULL)
    SDL_FreeSurface(screen);
  else
    printf("screen is NULL in osd_gfx_init_normal_mode, shouldn't happen\n");

#ifndef FINAL_RELEASE
  printf("Mode change: %dx%d\n", io.screen_w, io.screen_h);
#endif

  if (io.screen_w == 0)
  {
    printf("This shouldn't happen? (io.screen_w == 0 in osd_gfx_init_normal_mode)\n");
    io.screen_w = 256;
  }

  if (io.screen_h == 0)
  {
    printf("This shouldn't happen? (io.screen_h == 0 in osd_gfx_init_normal_mode)\n");
    io.screen_h = 224;
  }

  screen = SDL_CreateRGBSurface(SDL_SWSURFACE, io.screen_w, io.screen_h, 8, 0, 0, 0, 0);

  if (physical_screen->flags & SDL_FULLSCREEN)
  {
    struct generic_rect rect;

    calc_fullscreen_aspect(physical_screen->w, physical_screen->h, &rect);

    physical_screen_rect.x = rect.start_x;
    physical_screen_rect.y = rect.start_y;
    physical_screen_rect.w = rect.end_x;
    physical_screen_rect.h = rect.end_y;

    SDL_FillRect(physical_screen, NULL, 0);
  }
  else if (((physical_screen->w / option.window_size) != io.screen_w) ||
           ((physical_screen->h / option.window_size) != io.screen_h))
  {
    physical_screen = SDL_SetVideoMode(io.screen_w * option.window_size, io.screen_h * option.window_size, 8,
                                       SDL_SWSURFACE | SDL_HWPALETTE);
    SetPalette();
  }

  return (screen && physical_screen) ? 1 : 0;
}

//! Delete the window
void 
osd_gfx_shut_normal_mode()
{
	SDL_FreeSurface(screen);
	screen = NULL;
	
	/* SDL will free physical_screen internally */
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}
 
/*****************************************************************************

    Function: osd_gfx_savepict

    Description: save a picture in the current directory
    Parameters: none
    Return: the numeric part of the created filename, 0xFFFF meaning that no more
      names were available

*****************************************************************************/
UInt16 osd_gfx_savepict()
  {
          short unsigned tmp=0;

#warning implement save picture feature		  
		  
          return tmp;
   }

/*****************************************************************************

    Function:  osd_gfx_set_hugo_mode

    Description: change the video mode
    Parameters: mode : mode of video screen
                width, height : minimum size of screen required
    Return: 0 on success
                 1 on error

*****************************************************************************/
SInt32 osd_gfx_set_hugo_mode(SInt32 mode,SInt32 width,SInt32 height)
{

  screen = SDL_SetVideoMode(320,200, 8, SDL_SWSURFACE);
  SetPalette();	
  return !screen;
	 
 }

/*****************************************************************************

    Function: osd_gfx_set_color

    Description: Change the component of the choosen color
    Parameters: UChar index : index of the color to change
    			UChar r	: new red component of the color
                UChar g : new green component of the color
                UChar b : new blue component of the color
    Return:

*****************************************************************************/
void osd_gfx_set_color(UChar index,
                       UChar r,
                       UChar g,
                       UChar b)
{
	
	SDL_Color R;

	r <<= 2;
	g <<= 2;
	b <<= 2;
	 
	R.r=r; 
	R.g=g; 
	R.b=b;
	
	SDL_SetColors(physical_screen, &R, index, 1);	

}

int ToggleFullScreen(void)
{
  SDL_PauseAudio(SDL_ENABLE);

  if (physical_screen->flags & SDL_FULLSCREEN)
  {
    if ((physical_screen = SDL_SetVideoMode(io.screen_w * option.window_size, io.screen_h * option.window_size,
                                            8, SDL_SWSURFACE | SDL_HWPALETTE)) == NULL)
    {
      Log("Can't get physical_screen for full screen\n");
      printf("Can't get physical screen\n");
    }
  }
  else
  {
    struct generic_rect rect;

    if ((physical_screen = SDL_SetVideoMode(option.fullscreen_width, option.fullscreen_height, 8, SDL_SWSURFACE | SDL_HWPALETTE | SDL_FULLSCREEN)) == NULL)
    {
      Log("Can't get physical_screen for full screen\n");
      printf("Can't get physical screen\n");
    }

    calc_fullscreen_aspect(physical_screen->w, physical_screen->h, &rect);

    physical_screen_rect.x = rect.start_x;
    physical_screen_rect.y = rect.start_y;
    physical_screen_rect.w = rect.end_x;
    physical_screen_rect.h = rect.end_y;
  }

  SetPalette();

  SDL_PauseAudio(SDL_DISABLE);
		
  return (physical_screen->flags & SDL_FULLSCREEN) ? 0 : 1;
}

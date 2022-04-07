#include "osd_gfx.h"

/* TEST, to handle allegro wip errors .. */
#define GFX_SVGALIB              AL_ID('S','V','G','A')   

SDL_Surface *screen;

int blit_x,blit_y;
// where must we blit the screen buffer on screen

int screen_blit_x, screen_blit_y;
// where on the screen we must blit XBuf

UChar* XBuf;
// buffer for video flipping

UChar index_to_RGB[256];
// convertion array from bero's reduced pce palette to x11 palette

int osd_gfx_init_normal_mode();
void osd_gfx_put_image_normal();

void osd_gfx_dummy_func();

osd_gfx_driver osd_gfx_driver_list[3] =
  {
  {osd_gfx_init_normal_mode, osd_gfx_put_image_normal, osd_gfx_dummy_func},
  {osd_gfx_init_normal_mode, osd_gfx_put_image_normal, osd_gfx_dummy_func},  
  {osd_gfx_init_normal_mode, osd_gfx_put_image_normal, osd_gfx_dummy_func}
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
	

/*
 if (message_delay)
   draw_sprite(XBuf,OSD_MESSAGE_SPR,screen_blit_x + 8,min(io.screen_h,vheight)-16);
*/

	Slock(screen);
	
	for (dum = 0; dum < io.screen_h; dum++)
 	  memcpy(screen->pixels + dum * io.screen_w , XBuf + dum * XBUF_WIDTH + (WIDTH - io.screen_w) / 2, io.screen_w);	
/*
	{
		int x,y;
		for (x = 0; x < io.screen_w; x++)
			for (y = 0; y < io.screen_h; y++)
			{
				((UChar*)screen->pixels)[x + y * io.screen_w] = ((UChar*)XBuf)[y * XBUF_WIDTH + WIDTH - io.screen_w / 2 + x];
				if (((UChar*)XBuf)[y * XBUF_WIDTH + WIDTH - io.screen_w / 2 + x])
				  printf("putting %d at (%d,%d)\n",index_to_RGB[((UChar*)XBuf)[y * XBUF_WIDTH + WIDTH - io.screen_w / 2 + x]],x,y);
			}
	}
*/
	Sulock(screen);
	SDL_Flip(screen);	

 //	printf("blitting done\n");
	
 // blit(XBuf,screen,screen_blit_x,screen_blit_y,blit_x,blit_y - 8,io.screen_w,io.screen_h);

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
  printf(mess);
	
 return;

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
	if ((io.screen_w == 0) || (io.screen_h== 0))
		screen = SDL_SetVideoMode(256, 224, 8, SDL_SWSURFACE | SDL_HWPALETTE);
	else
		screen = SDL_SetVideoMode(io.screen_w, io.screen_h, 8, SDL_SWSURFACE | SDL_HWPALETTE);
	SetPalette();		
	return screen != NULL;
 
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
	
	SDL_SetColors(screen, &R, index, 1);	
	
}

void ToggleFullScreen()
{
	SDL_WM_ToggleFullScreen(screen);
}

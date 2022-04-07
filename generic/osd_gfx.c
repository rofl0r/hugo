#include "osd_gfx.h"

int blit_x,blit_y;
// where must we blit the screen buffer on screen

int screen_blit_x, screen_blit_y;
// where on the screen we must blit XBuf


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

  /*
 int dum;

 if (message_delay)
   draw_sprite(XBuf,OSD_MESSAGE_SPR,screen_blit_x + 8,min(io.screen_h,vheight)-16);

 blit(XBuf,screen,screen_blit_x,screen_blit_y,blit_x,blit_y - 8,io.screen_w,io.screen_h);
*/

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

  fprintf(stderr,mess);

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

 return 0;
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
          char* final="PICT0000.PCX";
          short unsigned tmp=0;

          while ((tmp!=0xFFFF) && (exists(final)))
              sprintf(final,"PICT%04X.PCX",++tmp);
          if (tmp==0xFFFF)
            return tmp;
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

  /*
 if (!set_gfx_mode(mode,width,height,0,0))
            {
             Log("Changing video mode to %dx%d\nPCE screen size is %dx%d\n",
                 width,
                 height,
                 io.screen_w,
                 io.screen_h);
             vwidth=width;
             vheight=height;

             blit_x=(width-io.screen_w)/2;
             blit_y=(height-io.screen_h)/2;

             screen_blit_x=(WIDTH-io.screen_w)/2;
             screen_blit_y=(224-io.screen_h)/2;

             Log("Now blit_x = %d\nblit_y = %d\nscreen_blit_x = %d\nscreen_blit_y = %d\n\n",
                 blit_x,
                 blit_y,
                 screen_blit_x,
                 screen_blit_y);

             SetPalette();
             return 0;
             }
	     */
 return 1;
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


 }

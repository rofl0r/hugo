/****************************************************************************/
/*                                                                          */
/*                       Interface source code                              */
/*                                                                          */
/* This file contains the code for using a skinable gui. Designed by        */
/* Zeograd (zeograd@caramail.com) and primarily intended for Hu-Go!, this   */
/* could be released as a stand alone customisable skinable allegro using   */
/* gui. Please report any improvement made to this code.                    */
/*                                                                          */
/****************************************************************************/

#include "interf.h"

#ifndef PI
#define PI 3.14159265358979323846
#endif

//------------------- Constant section ------------------------------

const int gui_width = 640;
const int gui_height = 480;
const int gui_border_width = 32;
const int width_char_gui = 8;
const int height_char_gui = 16;
const int max_step = 63;
const int HIGHLIGHT_COLOR = 191;

//-------- Forward function declaration to use in the specific part ---------

// There's a good reason why I've not put them in the interf.h, I don't
// want this to be available to others modules (that's also why I've included
// a .c file instead of making another object file)

void output_string (BITMAP * dest, int x, int y, unsigned char *s,
		    void (*output_func) (BITMAP *, int, int, unsigned char),
		    int (*output_func_width) (unsigned char));

void output_char (BITMAP * dest, int x, int y, unsigned char ch);
int output_char_height (unsigned char ch);
int output_char_width (unsigned char ch);

void output_char_highlight (BITMAP * dest, int x, int y, unsigned char ch);
// use same height and width function as the normal output_char

void output_char_double (BITMAP * dest, int x, int y, unsigned char ch);
int output_char_double_height (unsigned char ch);
int output_char_double_width (unsigned char ch);

//---------- Global variable section needed in the specific part ------------

PALETTE current_palette;
// The palette actually used by the interface

BITMAP *main_menu_src = NULL;
// The bitmap used as the main menu background, comes directly from the skin

BITMAP *screen_menu_src = NULL;
// The bitmap used as the background of the upper right panel, only the edges
// are keeped from the skin. Edges width is gui_border_width.
// The interior is filled with the current displayed image in the emulator.

BITMAP *desc_menu_src = NULL;
// The bitmap used as the background of the lower right panel, only the edges
// are keeped from the skin. Edges width is gui_border_width.

BITMAP *sub_menu_src = NULL;
// The bitmap used as the backgruond of the upper right panel when displaying
// text, only the edges are keeped from the skin. Edges width is
// gui_border_width.

BITMAP *main_menu = NULL;

BITMAP *sub_menu = NULL;

BITMAP *screen_menu = NULL;

BITMAP *desc1_menu = NULL;

BITMAP *desc2_menu = NULL;

BITMAP *main_menu_txt = NULL;

BITMAP *sub_menu_txt = NULL;

BITMAP *desc1_menu_txt = NULL;

BITMAP *desc2_menu_txt = NULL;

int current_depth;
// The current depth in the menu

int previous_depth;
// The previous depth we were in

extern int current_choice_in_depth[];
// Forward definition to use in refresh_screen function, but the actual
// static allocation need stuff from the very same file

UChar exit_asked;
// A boolean value indicating whether it's time to exit the interface.

UChar must_quit_emu = 0;
// The value returned by the gui function, if non zero, ask to kill the emu

//------------------- Dirty section ----------------------------------------

#include "specific_interface.c"

//------------------- Other Global Variable section ------------------------

BITMAP *font_spr[256];
// An array of bitmap allowing to represent each character by a bitmap using
// its ascii code. Drawing 'A' is achieved with font_spr['A'] e.g.

UChar idx_col;
// Index of the current displayed value for the color cycling.

PALETTE temporary_palette;
// The palette used when the interface was invoqued in order to restore it
// when exiting

RGB cycle[32];
// The array containing all color to display when color cycling, the current
// displayed color is indicated by idx_col

int current_choice_in_depth[MAX_DEPTH_MENU + 1];
// The currently selected choice in each depth is stored there

char skin_filename[256];
// filename of the skin to use

//------------------- Functions section -----------------------------

/*****************************************************************************

    Function: sin_sqr

    Description: return almost siný of the argument
    Parameters: float argument of the math function
    Return: result

*****************************************************************************/
float
sin_sqr (float x)
{
  float t = sin (x * PI / 2);

  return t * t;

}

/*****************************************************************************

    Function: output_char

    Description: display a single char from the font on a bitmap, no FX
    Parameters: BITMAP *, bitmap to display the char onto
                int x,y , position in the bitmap
                unsigned char ch, the char to show
    Return: nothing

*****************************************************************************/
void
output_char (BITMAP * dest, int x, int y, unsigned char ch)
{

  masked_blit (font_spr[ch], dest, 0, 0, x, y, width_char_gui,
	       height_char_gui);

}

/*****************************************************************************

    Function: output_char_highlight

    Description: display a single char from the font on a bitmap,
                 converting its color to the HIGHLIGHT_COLOR
    Parameters: BITMAP *, bitmap to display the char onto
                int x,y , position in the bitmap
                unsigned char ch, the char to show
    Return: nothing

*****************************************************************************/
void
output_char_highlight (BITMAP * dest, int x, int y, unsigned char ch)
{
  BITMAP *tmp = create_bitmap (width_char_gui, height_char_gui);
  int xx, yy;

  clear (tmp);

  for (xx = 0; xx < width_char_gui; xx++)
    {
      for (yy = 0; yy < height_char_gui; yy++)
	{
	  if (getpixel (font_spr[ch], xx, yy))
	    putpixel (tmp, xx, yy, HIGHLIGHT_COLOR);
	}
    }

  masked_blit (tmp, dest, 0, 0, x, y, width_char_gui, height_char_gui);

}


/*****************************************************************************

    Function: output_char_width

    Description: return the width of the character that will be displayed
                 with the output_char function
    Parameters: unsigned char, the char to display
    Return: int, the width of the displayed char

*****************************************************************************/
int
output_char_width (unsigned char ch)
{

  return width_char_gui;

}

/*****************************************************************************

    Function: output_char_height

    Description: return the height of the character that will be displayed
                 with the output_char function
    Parameters: unsigned char, the char to display
    Return: int, the height of the displayed char

*****************************************************************************/
int
output_char_height (unsigned char ch)
{

  return height_char_gui;

}

/*****************************************************************************

    Function: output_char_double

    Description: display a single char from the font on a bitmap,
                 double it size both in width and height
    Parameters: BITMAP *, bitmap to display the char onto
                int x,y , position in the bitmap
                unsigned char ch, the char to show
    Return: nothing

*****************************************************************************/
void
output_char_double (BITMAP * dest, int x, int y, unsigned char ch)
{
  BITMAP *tmp = create_bitmap (width_char_gui << 1, height_char_gui << 1);

  stretch_blit (font_spr[ch], tmp, 0, 0, width_char_gui, height_char_gui,
		0, 0, width_char_gui << 1, height_char_gui << 1);

  masked_blit (tmp, dest, 0, 0, x, y, width_char_gui << 1,
	       height_char_gui << 1);

}

/*****************************************************************************

    Function: output_char_double_width

    Description: return the width of the character that will be displayed
                 with the output_char_double function
    Parameters: unsigned char, the char to display
    Return: int, the width of the displayed char

*****************************************************************************/
int
output_char_double_width (unsigned char ch)
{

  return width_char_gui << 1;

}

/*****************************************************************************

    Function: output_char_double_height

    Description: return the height of the character that will be displayed
                 with the output_char_double function
    Parameters: unsigned char, the char to display
    Return: int, the height of the displayed char

*****************************************************************************/
int
output_char_double_height (unsigned char ch)
{

  return height_char_gui << 1;

}


/*****************************************************************************

    Function: output_string

    Description: display a whole string, without any effect
    Parameters: BITMAP*, the bitmap to draw into
                int x, y , position of the string on the screen
                unsigned char*, the string to display
                void (*output_func)(BITMAP*,int,int,unsigned char), the funtion
                                                             used to draw char
    Return: nothing

*****************************************************************************/
void
output_string (BITMAP * dest, int x, int y, unsigned char *s,
	       void (*output_func) (BITMAP *, int, int, unsigned char),
	       int (*output_func_width) (unsigned char))
{

  x -= output_func_width (*s);

  while (*s)
    output_func (dest, x += output_func_width (*s), y, *s++);

}



/*****************************************************************************

    Function: what_char

    Description: Little silly function to know what letter must be in what case
    Parameters: int i, the number of the read case
    Return: int, the ascii code of the letter in it

*****************************************************************************/
int
what_char (int i)
{
  if (i < 26)
    return 'A' + i;
  if ((i > 31) && (i < 42))
    return '0' + i - 32;
  switch (i)
    {
    case 26:
      return ':';
    case 27:
      return '.';
    case 28:
      return ',';
    case 29:
      return ' ';
    case 30:
      return '"';
    case 31:
      return '\'';
    }

  return 0;

}



/*****************************************************************************

    Function: load_skin

    Description: This function read the bitmap containing the skin and store it
                 in global variables after having allocating them
    Parameters: nothing
    Return: nothing, but allocates and fill *_menu_src, also loads current_palette

*****************************************************************************/

void
load_skin ()
{

  BITMAP *skin = load_bitmap (skin_filename, current_palette);

  UChar convert_color[256];

  int x, y, i;

  for (i = 0; i <= 255; i++)
    {
      RGB tmp;
      get_color (i, &tmp);

      convert_color[i] =
	255 - 63 +
	(int) ((float) ((tmp.r & 63) + (tmp.g & 63) + (tmp.b & 63)) / 3.0);

#ifndef FINAL_RELEASE
      Log ("Convert color %d = %d\nr = %d, b = %d, g = %d\n",
	   i, convert_color[i], tmp.r, tmp.g, tmp.b);
#endif

    }


  for (i = 255 - 63; i <= 255; i++)
    {
      current_palette[i].r = i & 63;
      current_palette[i].b = i & 63;
      current_palette[i].g = i & 63;
    }

  for (i = 0; i <= 255; i++)
    font_spr[i] = NULL;

  for (i = 0, y = 32;
       y < 32 + 9 * (height_char_gui + 1); y += height_char_gui + 1)
    for (x = 320 + 32;
	 x < 320 + 32 + 5 * (width_char_gui + 1);
	 x += width_char_gui + 1, i++)
      {
	font_spr[what_char (i)] =
	  create_bitmap (width_char_gui, height_char_gui);

	blit (skin, font_spr[what_char (i)], x + 1, y + 1, 0, 0,
	      width_char_gui, height_char_gui);

      }

  main_menu_src = create_bitmap (gui_width / 2, gui_height);

  screen_menu_src = create_bitmap (gui_width / 2, gui_height * 2 / 3);

  sub_menu_src = create_bitmap (gui_width / 2, gui_height * 2 / 3);

  desc_menu_src = create_bitmap (gui_width / 2, gui_height / 3);

  blit (skin, main_menu_src, 0, 0, 0, 0, gui_width / 2, gui_height);

  blit (skin,
	sub_menu_src,
	gui_width / 2, 0, 0, 0, gui_width / 2, gui_height * 2 / 3);

  rectfill (sub_menu_src, gui_border_width - 1, gui_border_width - 1,
	    sub_menu_src->w - gui_border_width,
	    sub_menu_src->h - gui_border_width, 0);

  blit (skin,
	screen_menu_src,
	gui_width / 2, 0, 0, 0, gui_width / 2, gui_height * 2 / 3);

  rectfill (screen_menu_src, gui_border_width - 1, gui_border_width - 1,
	    screen_menu_src->w - gui_border_width,
	    screen_menu_src->h - gui_border_width, 0);


  blit (screen, screen_menu_src, 0, 0, gui_border_width, gui_border_width,
	screen_menu_src->w - 2 * gui_border_width,
	screen_menu_src->h - 2 * gui_border_width);

  for (x = gui_border_width; x <= screen_menu_src->w - gui_border_width; x++)
    for (y = gui_border_width; y <= screen_menu_src->h - gui_border_width;
	 y++)
      {
	putpixel (screen_menu_src, x, y,
		  convert_color[getpixel (screen_menu_src, x, y)]);
      }


  blit (skin,
	desc_menu_src,
	gui_width / 2,
	gui_height * 2 / 3, 0, 0, gui_width / 2, gui_height / 3);

  rectfill (desc_menu_src, gui_border_width - 1, gui_border_width - 1,
	    desc_menu_src->w - gui_border_width,
	    desc_menu_src->h - gui_border_width, 0);

  destroy_bitmap (skin);

}

/*****************************************************************************

    Function: blit_move

    Description: allows slide bliting using an user function for slide speed
    Parameters: BITMAP* source,       (like in blit)
                BITMAP* dest,               --
                int source_x,               --
                int source_y,               --
                int dest_x_1,   position of beginning dest blitting
                int dest_y_1,               --
                int dest_x_2,   position of ending dest blitting
                int dest_y_2,               --
                int width,            (like in blit)
                int height,                 --
                int step,             current step (in 0..max_step)
                int max_step,         number of steps
                float (*slide_func)(float) function from 0..1 to 0..1
                                           to determine slide speed

    Return: nothing

*****************************************************************************/

void
blit_move (BITMAP * source,
	   BITMAP * dest,
	   int source_x,
	   int source_y,
	   int dest_x_1,
	   int dest_y_1,
	   int dest_x_2,
	   int dest_y_2,
	   int width,
	   int height, int step, int max_step, float (*slide_func) (float))
{

  int x =
    dest_x_1 +
    (int) ((dest_x_2 - dest_x_1) *
	   ((*slide_func) ((float) (step) / max_step)));
  int y =
    dest_y_1 +
    (int) ((dest_y_2 - dest_y_1) *
	   ((*slide_func) ((float) (step) / max_step)));

  blit (source, dest, source_x, source_y, x, y, width, height);

}

/*****************************************************************************

    Function: handle_keyboard

    Description: This function handle the keyboard and the eventual effects
    Parameters: nothing but use global variables :
                        MAX_DEPTH_MENU
                        MAX_CHOICE_IN_DEPTH[MAX_DEPTH_MENU]
                        int current_depth_menu (in 0..MAX_DEPTH_MENU)
                        int current_choice_in_depth[MAX_DEPTH_MENU]
                        void (*action_func()[MAX_DEPTH_MENU + 1]
                              [MAX_CHOICE + 1])
                        this function will be called in case of pressing
                        SPACE or RETURN, may just change of option or
                        perform another kind of action

    Return: nothing but exit_asked may be set to 1 to quit the interface

*****************************************************************************/
void
handle_keyboard ()
{

  int key;

  while (!osd_keypressed ())
    {
      vsync ();
      set_color (HIGHLIGHT_COLOR, &cycle[idx_col++, idx_col &= 0x1F]);
    }

  key = osd_readkey ();

  switch (key >> 8)		// We are reading by scancode
    {
    case KEY_ESC:
    case KEY_LEFT:
      if (!current_depth)	// We are at the top level, let's quit
	exit_asked = 1;
      else			// Hop to the previous depth, but we can't do more
	current_depth = previous_depth;
      break;
    case KEY_DOWN:
      if (current_choice_in_depth[current_depth] <
	  MAX_CHOICE_IN_DEPTH[current_depth])
	current_choice_in_depth[current_depth]++;
      break;
    case KEY_UP:
      if (current_choice_in_depth[current_depth] != 0)
	current_choice_in_depth[current_depth]--;
      break;
    case KEY_ENTER:
    case KEY_RIGHT:
    case KEY_SPACE:
      (action_func[current_depth][current_choice_in_depth[current_depth]] ());
      break;
    default:
    }

}


/*****************************************************************************

    Function: gui

    Description: This function displays a skined gui and may change options
    Parameters: nothing
    Return: nothing, may change global option variables

*****************************************************************************/

UChar gui ()
{

  int dum;

  BITMAP *temporary_buffer = create_bitmap (screen->w, screen->h);

  main_menu = create_bitmap (gui_width / 2, gui_height);

  sub_menu = create_bitmap (gui_width / 2, gui_height * 2 / 3);

  screen_menu = create_bitmap (gui_width / 2, gui_height * 2 / 3);

  desc1_menu = create_bitmap (gui_width / 2, gui_height / 3);

  desc2_menu = create_bitmap (gui_width / 2, gui_height / 3);

  main_menu_txt = create_sub_bitmap (main_menu,
				     gui_border_width - 1,
				     gui_border_width - 1,
				     (main_menu->w) - 2 * gui_border_width,
				     (main_menu->h) - 2 * gui_border_width);

  sub_menu_txt = create_sub_bitmap (sub_menu,
				    gui_border_width - 1,
				    gui_border_width - 1,
				    (sub_menu->w) - 2 * gui_border_width,
				    (sub_menu->h) - 2 * gui_border_width);

  desc1_menu_txt = create_sub_bitmap (desc1_menu,
				      gui_border_width - 1,
				      gui_border_width - 1,
				      (desc1_menu->w) - 2 * gui_border_width,
				      (desc1_menu->h) - 2 * gui_border_width);

  desc2_menu_txt = create_sub_bitmap (desc2_menu,
				      gui_border_width - 1,
				      gui_border_width - 1,
				      (desc2_menu->w) - 2 * gui_border_width,
				      (desc2_menu->h) - 2 * gui_border_width);

  idx_col = 0;

  get_palette (temporary_palette);

  blit (screen, temporary_buffer, 0, 0, 0, 0, screen->w, screen->h);

  load_skin ();

  for (dum = 0; dum < 16; dum++)
    {
      cycle[31 - dum].r = cycle[dum].r =
	(63 +
	 (255 -
	  63) * (sin ((PI * dum) / 16)) * (sin ((PI * dum) / 16))) / 4;
      cycle[31 - dum].g = cycle[dum].g =
	(13 +
	 (255 -
	  13) * (sin ((PI * dum) / 16)) * (sin ((PI * dum) / 16))) / 4;
      cycle[31 - dum].b = cycle[dum].b =
	(227 +
	 (255 -
	  227) * (sin ((PI * dum) / 16)) * (sin ((PI * dum) / 16))) / 4;
    }


  set_gfx_mode (GFX_AUTODETECT, gui_width, gui_height, 0, 0);

  set_palette (current_palette);

/*
 blit(main_menu_src, screen, 0, 0, 0, 0, gui_width / 2, gui_height);

 blit(sub_menu_src, screen, 0, 0, gui_width / 2, 0, gui_width / 2, gui_height * 2 / 3);

 blit(desc_menu_src, screen, 0, 0, gui_width / 2, gui_height * 2 / 3,
                                   gui_width / 2, gui_height / 3);
*/

  for (dum = 0; dum <= max_step; dum++)
    {
      blit_move (main_menu_src, screen, 0, 0, -gui_width / 2, 0, 0, 0,
		 gui_width / 2, gui_height, dum, max_step, sin_sqr);
      blit_move (screen_menu_src, screen, 0, 0, gui_width / 2,
		 -gui_height * 2 / 3, gui_width / 2, 0, gui_width / 2,
		 gui_height * 2 / 3, dum, max_step, sin_sqr);
      blit_move (desc_menu_src, screen, 0, 0, gui_width, gui_height * 4 / 3,
		 gui_width / 2, gui_height * 2 / 3, gui_width / 2,
		 gui_height / 3, dum, max_step, sin_sqr);
      vsync ();
    }

/*
 blit(main_menu_src, main_menu, 0, 0, 0, 0, main_menu_src -> w, main_menu_src -> h);

 output_string(main_menu_txt, 8, 8, "TEST TEXT",
               output_char_double, output_char_double_width);

 output_string(main_menu_txt, 8, 40, "ABCDEFGHIJKLOMNOPQRSTUVW",
               output_char_highlight, output_char_width);

 blit(main_menu, screen, 0, 0, 0, 0, gui_width / 2, gui_height);
*/

  exit_asked = 0;

  current_depth = 0;

  previous_depth = 0;

  for (dum = 0; dum <= MAX_DEPTH_MENU; dum++)
    current_choice_in_depth[dum] = 0;

  do
    {

      handle_keyboard ();

      redraw_screen (main_menu,
		     sub_menu,
		     screen_menu,
		     desc1_menu,
		     desc2_menu,
		     main_menu_txt,
		     sub_menu_txt, desc1_menu_txt, desc2_menu_txt);

    }
  while (!exit_asked);

  (*osd_gfx_driver_list[video_driver].init) ();
  // This instruction only restore the correct video more to continue emulation
  // Can be easily replaced by nothing or a restore text mode instruction for tests
  // Don't forget to remove also the blit function or it'll crash

  set_palette (temporary_palette);

  blit (temporary_buffer, screen, 0, 0, 0, 0, screen->w, screen->h);

  destroy_bitmap (temporary_buffer);

  destroy_bitmap (main_menu);
  destroy_bitmap (sub_menu);
  destroy_bitmap (screen_menu);
  destroy_bitmap (desc1_menu);
  destroy_bitmap (desc2_menu);

  destroy_bitmap (main_menu_txt);
  destroy_bitmap (sub_menu_txt);
  destroy_bitmap (desc1_menu_txt);
  destroy_bitmap (desc2_menu_txt);

  destroy_bitmap (main_menu_src);
  destroy_bitmap (sub_menu_src);
  destroy_bitmap (screen_menu_src);
  destroy_bitmap (desc_menu_src);

  for (dum = 0; dum <= 255; dum++)
    if (font_spr[dum])
      destroy_bitmap (font_spr[dum]);

  return must_quit_emu;

}

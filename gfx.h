#ifndef _GFX_H_
#define _GFX_H_

#include "sys_dep.h"

#define XBUF_WIDTH (320+64+40)

#define textoutshadow(bmp,f,s,x,y,color1,color2,offsetx,offsety) { textout(bmp,f,s,x+offsetx,y+offsety,color2); textout(bmp,f,s,x,y,color1); }
// Just a little define to avoid too many keystrokes ;)

/*
 * generic_rect - Used to keep calc_fullscreen_aspect gfx lib independant.  Currently
 *   used to remap values to an SDL_Rect structure.
 */
struct generic_rect
{
  unsigned short start_x, start_y;
  unsigned short end_x, end_y;
};

#ifdef SDL

#include <SDL/SDL.h>

extern SDL_Surface *physical_screen;
extern SDL_Rect physical_screen_rect;

#endif

#endif

#ifndef _GFX_H_
#define _GFX_H_

#include "sys_dep.h"

#define XBUF_WIDTH (320+64+40)

#define textoutshadow(bmp,f,s,x,y,color1,color2,offsetx,offsety) { textout(bmp,f,s,x+offsetx,y+offsety,color2); textout(bmp,f,s,x,y,color1); }
// Just a little define to avoid too many keystrokes ;)

#endif

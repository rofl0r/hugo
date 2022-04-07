#ifndef _INCLUDE_KEYBOARD_H
#define _INCLUDE_KEYBOARD_H

#include "pce.h"

#if defined(SDL)
#include "osd_win_sdl_machine.h"
#else
  #if defined(ALLEGRO)
  #include "osd_win_allegro_machine.h"
  #endif
#endif

extern char auto_fire_A[5];
/* Is auto fire on */

extern char auto_fire_B[5];
/* Is auto fire on */

#endif
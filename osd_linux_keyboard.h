#ifndef _INCLUDE_LINUX_KEYBOARD_H
#define _INCLUDE_LINUX_KEYBOARD_H

#include "pce.h"


#if defined(SDL)
#include "osd_linux_sdl_machine.h"
#else
  #if defined(ALLEGRO)
  #include "osd_linux_allegro_machine.h"
  #endif

  #if !defined(FREEBSD)
    #include <linux/joystick.h>
  #endif
#endif

typedef struct {
        SInt16 axis[4];
        SInt16 button[16];
        } js_status;

extern char auto_fire_A[5];
/* Is auto fire on */

extern char auto_fire_B[5];
/* Is auto fire on */

#endif

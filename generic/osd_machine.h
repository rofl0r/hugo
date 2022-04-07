#ifndef _INCLUDE_LINUX_OSD_INIT_MACHINE
#define _INCLUDE_LINUX_OSD_INIT_MACHINE

#include "sys_dep.h"
#include "lang.h"
#include "pce.h"
#include "osd_gfx.h"
#include "gfx.h"

extern UChar gamepad;
// gamepad detected ?

extern int gamepad_driver;
// what kind of jypad must we have to handle

extern char synchro;
// à fond, à fond, à fond? (french joke ;)

extern char dump_snd;
// Do we write sound to file

#endif

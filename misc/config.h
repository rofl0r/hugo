#ifndef _DJGPP_INCLUDE_CONFIG_H
#define _DJGPP_INCLUDE_CONFIG_H

#include "pce.h"
#include "debug.h"

#include "osd_keyboard.h"
#include "osd_cd.h"

#include "interf.h"
#include "lang.h"
#include "osd_machine.h"
#include "sys/param.h"

void parse_INIfile();
/* check the configuration file for options
   also make some global initialisations */

void parse_commandline(int argc,char** argv);
/* check the command line for options */

extern unsigned char joy_mapping[5][16];


extern SInt32 smode,vmode;

extern char short_exe_name[80];

extern char cart_name[256];

extern char* bmdefault;

extern char initial_path[128];

#endif

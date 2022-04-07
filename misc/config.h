#ifndef _INCLUDE_CONFIG_H
#define _INCLUDE_CONFIG_H

#include "pce.h"
#include "debug.h"

#include "osd_keyboard.h"
#include "osd_cd.h"

#include "interf.h"
#include "lang.h"
#include "osd_machine.h"

#if defined(LINUX)

#include "sys/param.h"

#endif

void parse_INIfile();
/* check the configuration file for options
   also make some global initialisations */

void parse_commandline(int argc,char** argv);
/* check the command line for options */

extern unsigned char joy_mapping[5][16];

extern SInt32 smode,vmode;

extern char* bmdefault;

#endif

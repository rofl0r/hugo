/**************************************************************************/
/*                                                                        */
/*                       List of Rom header                               */
/*                                                                        */
/*              Made by Zeograd (zeograd@caramail.com)                    */
/*                                                                        */
/*    11 June '99 : 521 recognized roms                                   */
/*    24 June '99 : 526 recognized roms                                   */
/*    27 June '99 : 541 recognized roms + separation of rom name/producer */
/*                                                                        */
/**************************************************************************/
#ifndef _DJ_INCLUDE_LIST_ROM_H
#define _DJ_INCLUDE_LIST_ROM_H

//#include "CRC_ctl.h"
#include <stdio.h>
#include <stdlib.h>

#define SUPER_GRAPHX 0x0001
#define TWO_PART_ROM 0x0002
#define PINBALL_KEY  0x0004
#define CD_SYSTEM    0x0008

#define ORIGINAL     0x0010
#define MODIFIED     0x0020

#define US_ENCODED   0x0040
#define POPULOUS     0x0080

#define USA          0x4000
#define JAP          0x8000

typedef struct {
	unsigned long CRC;
	char *name,*producer,*serial,*date,*size,*note;
   unsigned short flags;
} ROM_TYPE;

#define NB_ROM 541

#include "cleantyp.h"

extern ROM_TYPE ROM_LIST[NB_ROM];

unsigned long filesize(FILE *F);
UInt32 CRC_file(char *name);

#endif

// pce.h
#ifndef _INCLUDE_PCE_H
#define _INCLUDE_PCE_H

#include "hard_pce.h"
#include "cheat.h"

#define	WIDTH	(360+64)

/* TEST */
// #define HEIGHT    214
/* TEST */

// #define	XBUF_WIDTH	(360+64)
#define	XBUF_HEIGHT	256
#define XBUF_WIDTH (320+64+40)

#define	HEIGHT	256

#include "cleantyp.h"

#ifdef MSDOS

#include <crt0.h>
#include <conio.h>
#include <dir.h>
#include <dos.h>

#elif defined(LINUX)

#include <sys/param.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/cdrom.h>

#elif defined(WIN32)

#define PATH_MAX 255

#endif



#ifdef KERNEL_DS

#include "h6280.h"
#include "globals.h"
#include "interupt.h"

#define Wr6502(A,V) _Wr6502((A),(V))

#define Rd6502(A) _Rd6502(A)

#else

#include "m6502.h"

#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "cd.h"

#ifdef ALLEGRO

#undef VGA
// Avoid both VGA and ALLEGRO define

#define	alleg_mouse_unused
#define	alleg_flic_unused
#define	alleg_gui_unused

#include "allegro.h"
#include "gfx.h"
#include "info_dat.h"
#include "data.h"
// Include some informations to correctly use the datafile

extern DATAFILE* datafile;

#endif

#ifdef __GNUC__
UChar _Rd6502(UInt16 A) __attribute__ ((const,regparm (1)))  ;
void _Wr6502(UInt16 A,UChar V) __attribute__ ((regparm (2))) ;
void bank_set(UChar P,UChar V) __attribute__ ((regparm (2)));
#endif

SInt32  CheckSprites(void);
void    RefreshLine(int Y1,int Y2);
void    RefreshScreen(void);
UInt32  CRC_file(char*);
SInt32  CartLoad(char *name);
#ifndef KERNEL_DS
int     ResetPCE(M6502 *M);
#else
int     ResetPCE();
#endif
SInt32  InitMachine(void);
void    TrashMachine(void);
SInt32  Joysticks(void);
UInt32  msf2nb_sect(UChar min, UChar sec, UChar fra);
void    fill_cd_info();
void    nb_sect2msf(UInt32 lsn,UChar *min, UChar *sec, UChar *frm);
void    Log(char*, ...);
void    delete_file_tmp(char* name,int dummy,int dummy2);

extern FILE* out_snd;
// The file used to put sound into

extern SChar volatile key_delay;
// are we allowed to press another 'COMMAND' key ?

extern volatile UInt32 message_delay;
// if different of zero, we must display the message pointed by pmessage

extern char short_cart_name[PATH_MAX];
// Just the filename without the extension (with a dot)
// you just have to add your own extension...

extern char short_iso_name[PATH_MAX];
// Added for ISO save support

extern char cdsystem_path[PATH_MAX];
// The path of the cdsystem to launch automaticaly

extern char sav_path[PATH_MAX];
// The place where to keep saved games
// currently a subdir a the EXE path named 'SAV'

extern char video_path[PATH_MAX];
// The place where to keep output pictures

extern char ISO_filename[PATH_MAX];
// the name of the ISO file

extern char cart_name[PATH_MAX];
// the name of the rom to load

extern char initial_path[PATH_MAX];
// initial path for rom seeking

extern char short_exe_name[PATH_MAX];

extern UInt32 scanline;

extern IO io;

#ifndef KERNEL_DS

extern M6502 M;

#endif

extern UChar populus;

extern UChar *PopRAM;
// Now dynamicaly allocated
// ( size of popRAMsize bytes )
// If someone could explain me why we need it
// the version I have works well without this trick

extern const UInt32 PopRAMsize;
// I don't really know if it must be 0x8000 or 0x10000

/*
####################################
####################################
####################################
####################################
2KILL :: BEGIN
####################################
####################################
####################################
####################################
*/
#ifdef ALLEGRO

extern BITMAP* XBuf;

#endif
/*
####################################
####################################
####################################
####################################
2KILL :: END
####################################
####################################
####################################
####################################
*/

extern SInt32 smode;
// what sound card type should we use? (0 means the silent one,
// my favorite : the fastest!!! ; and -1 means AUTODETECT;
// later will avoid autodetection if wanted)

extern UInt32 freq_int;
// frequency of interrupt to be used to make sound

extern SChar silent;
// use sound?

extern UChar language;
// the current language

extern int BaseClock, UPeriod;

extern UChar	US_encoded_card;
// Do we have to swap even and odd bytes in the rom

extern UChar debug_on_beginning;
// Do we have to set a bp on the reset IP

extern UChar CD_emulation;
// Do we handle CDs

extern UChar CDBIOS_replace[0x4d][2];
// Used to know what byte do we have replaced

extern UChar use_eagle;
// Do we use eagle ?

extern UChar *Page[8],*ROMMap[256];

extern UInt32 timer_60;
// how many times do the interrupt have been called

extern UChar bcdbin[0x100];

extern UChar binbcd[0x100];

extern UInt32 pce_cd_sectoraddy;

typedef struct
    {

     UInt32 offset;

     UChar  new_val;

     } PatchEntry;

typedef struct
    {

     UInt32 StartTime;
     UInt32 Duration;
     char data[32];

     } SubtitleEntry;


typedef enum {
	HCD_SOURCE_REGULAR_FILE,
        HCD_SOURCE_CD_TRACK
	} hcd_source_type;

typedef struct
    {
     UChar beg_min;
     UChar beg_sec;
     UChar beg_fra;

     UChar type;

     UInt32 beg_lsn;
     UInt32 length;

     hcd_source_type source_type;
     char filename[256];

     UInt32 patch_number;
     UInt32 subtitle_number;

     UChar subtitle_synchro_type;

     PatchEntry *patch;
     SubtitleEntry *subtitle;

     } Track;

extern Track CD_track[0x100];

extern UChar nb_max_track;

extern UChar video_driver;

extern UChar use_scanline;

extern UChar minimum_bios_hooking;

extern volatile char can_blit;
// can we draw or not ? (that's all the question)

extern UChar cart_reload;

#define min(a,b) ({typedef _ta = (a), _tb = (b); _ta _a = (a); _tb _b = (b);_a < _b ? _a : _b; })

#define max(a,b) ({typedef _ta = (a), _tb = (b); _ta _a = (a); _tb _b = (b);_a > _b ? _a : _b; })

// Video related defines

extern UChar can_write_debug;

#define	SpHitON		   (io.VDC[CR].W&0x01)
#define	OverON		   (io.VDC[CR].W&0x02)
#define	RasHitON	   (io.VDC[CR].W&0x04)
#define	VBlankON	   (io.VDC[CR].W&0x08)
#define	SpriteON	   (io.VDC[CR].W&0x40)
#define	ScreenON	   (io.VDC[CR].W&0x80)

#define	VRAMSIZE	   0x20000

#define	VDC_CR	   0x01
#define	VDC_OR	   0x02
#define	VDC_RR	   0x04
#define	VDC_DS	   0x08
#define	VDC_DV	   0x10
#define	VDC_VD	   0x20
#define	VDC_BSY	   0x40
#define	VDC_SpHit	VDC_CR
#define	VDC_Over	   VDC_OR
#define	VDC_RasHit	VDC_RR
#define	VDC_InVBlank	VDC_VD
#define	VDC_DMAfinish	VDC_DV
#define	VDC_SATBfinish	VDC_DS

#define	SATBIntON (io.VDC[DCR].W&0x01)
#define	DMAIntON	 (io.VDC[DCR].W&0x02)

#define	IRQ2	1
#define	IRQ1	2
#define	TIRQ	4

// Joystick related defines

#define J_UP      0
#define J_DOWN    1
#define J_LEFT    2
#define J_RIGHT   3
#define J_I       4
#define J_II      5
#define J_SELECT  6
#define J_START   7
#define J_AUTOI   8
#define J_AUTOII  9
#define J_PI      10
#define J_PII     11
#define J_PSELECT 12
#define J_PSTART  13
#define J_PAUTOI  14
#define J_PAUTOII 15

// Post include to avoid circular definitions

#include	"list_rom.h" // List of known rom

#include "gui.h"

#include "config.h"

#include "sprite.h"

#include "hcd.h"

#if defined(SEAL_SOUND)

#include	</djgpp/audio/include/audio.h> // SEAL include

#endif /* SEAL sound */

#include <time.h>

#ifdef SOUND
#include	"sound.h"
#endif // SOUND

#ifndef	TRUE
#define	TRUE	1
#define	FALSE	0
#endif

#endif

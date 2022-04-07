// pce.h
#ifndef _DJGPP_INCLUDE_PCE_H
#define _DJGPP_INCLUDE_PCE_H

#define	WIDTH	(360+64)

/* TEST */
// #define HEIGHT    214
/* TEST */

#define	XBUF_HEIGHT	256

#define	HEIGHT	256

#include "cleantyp.h"

#ifdef MSDOS

#include <crt0.h>
#include <conio.h>
#include <dir.h>
#include <dos.h>

#elif defined(LINUX)

#include <sys/types.h>
#include <linux/cdrom.h>
#include <sys/ioctl.h>

#elif defined(WIN32)

#endif

#ifdef KERNEL_DS

#include "h6280.h"
#include "globals.h"
#include "interupt.h"

#define Wr6502(A,V) _Wr6502((A),(V))

#define Rd6502(A) _Rd6502(A)

typedef union
{
  struct { UChar l,h; } B;
  UInt16 W;
} pair;

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

#include <allegro.h>
#include "gfx.h"

#endif

#include "cleantyp.h"
// A 'quite portable' type definition

#include "cheat.h"

#ifdef ALLEGRO

#include "info_dat.h"
#include "data.h"
// Include some informations to correctly use the datafile

extern DATAFILE* datafile;

#endif


typedef struct tagIO {
	pair VDC[32];
	pair VCE[0x200];
	pair vce_reg;
	/* VDC */
	UInt16 vdc_inc,vdc_raster_count;
	UChar vdc_reg,vdc_status,vdc_ratch,vce_ratch;
	UChar vdc_satb;
	UChar vdc_pendvsync;
	SInt32 bg_h,bg_w;
	SInt32 screen_w,screen_h;
	SInt32 scroll_y;
	SInt32 minline, maxline;
	/* joypad */
	UChar JOY[16];
	UChar joy_select,joy_counter;
	/* PSG */
	UChar PSG[6][8],wave[6][32],wavofs[6];
        // PSG STRUCTURE
        // 0 : dda_out
        // 2 : freq (lo byte)  | In reality it's a divisor
        // 3 : freq (hi byte)	 | 3.7 Mhz / freq => true snd freq
        // 4 : dda_ctrl
        //     000XXXXX
        //     ^^  ^
        //     ||  ch. volume
        //     ||
        //     |direct access (everything at byte 0)
        //     |
        //    enable
        // 5 : pan (left vol = hi nibble, right vol = low nibble)
        // 7 : noise_ctrl
	UChar psg_ch,psg_volume,psg_lfo_freq,psg_lfo_ctrl;
	/* TIMER */
	UChar timer_reload,timer_start,timer_counter;
	/* IRQ */
	UChar irq_mask,irq_status;
	/* CDROM extention */
	SInt32 backup,adpcm_firstread;
	pair adpcm_ptr;
	UInt16 adpcm_rptr,adpcm_wptr;

/* CAREFUL, added variable */
   UInt16 adpcm_dmaptr;

/* CAREFUL, added variable */
   UChar adpcm_rate;

/* CAREFUL, added variable */
   UInt32 adpcm_pptr; /* to know where to begin playing adpcm (in nibbles) */

/* CAREFUL, added variable */
   UInt32 adpcm_psize; /* to know how many 4-bit samples to play */

} IO;

#ifdef __GNUC__
UChar _Rd6502(UInt16 A) __attribute__ ((const,regparm (1)))  ;
void _Wr6502(UInt16 A,UChar V) __attribute__ ((regparm (2))) ;
void bank_set(UChar P,UChar V) __attribute__ ((regparm (2)));
#endif

SInt32  CheckSprites(void);
void    IO_write(UInt16 A,UChar V);
UChar   IO_read(UInt16 A);
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

extern UChar RAM[0x8000];
// mem where variables are stocked (well, RAM... )

extern UChar *WRAM;
// extra backup memory

extern FILE* out_snd;
// The file used to put sound into

extern SChar volatile key_delay;
// are we allowed to press another 'COMMAND' key ?

extern volatile UInt32 message_delay;
// if different of zero, we must display the message pointed by pmessage

extern char short_cart_name[80];
// Just the filename without the extension (with a dot)
// you just have to add your own extension...

extern char short_iso_name[80];
// Added for ISO save support

extern char cdsystem_path[256];
// The path of the cdsystem to launch automaticaly

extern char sav_path[80];
// The place where to keep saved games
// currently a subdir a the EXE path named 'SAV'

extern char video_path[80];
// The place where to keep output pictures

extern char ISO_filename[256];
// the name of the ISO file

extern UChar *VRAM;
// Video mem

extern UInt16 SPRAM[64*4];
// SPRAM = sprite RAM

extern UChar Pal[512];
// PCE Palette

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

extern UChar *vchange,*vchanges;
//typedef char BOOL;
//typedef unsigned char BYTE;

extern UInt32 *VRAM2,*VRAMS;


#ifdef ALLEGRO

extern BITMAP* XBuf;

#endif


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

extern UChar *PCM;
// ADPCM buffer

extern UChar cd_port_1800;

extern UChar cd_port_1801;

extern UChar cd_port_1802;

extern UChar cd_port_1804;

extern UChar bcdbin[0x100];

extern UChar binbcd[0x100];

extern UChar *cd_sector_buffer;

extern UChar *cd_extra_mem;
// extra ram provided by the system CD card

extern UChar *cd_read_buffer;

extern UChar *cd_extra_super_mem;

extern UInt32 pce_cd_read_datacnt;

extern UInt32 pce_cd_sectoraddy;

extern UChar cd_sectorcnt;

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

extern char can_blit;
// can we draw or not ? (that's all the question)

extern UChar cart_reload;

#define min(a,b) ({typedef _ta = (a), _tb = (b); _ta _a = (a); _tb _b = (b);_a < _b ? _a : _b; })

#define max(a,b) ({typedef _ta = (a), _tb = (b); _ta _a = (a); _tb _b = (b);_a > _b ? _a : _b; })

// Video related defines

extern UChar can_write_debug;

#define	VRR	2
enum _VDC_REG {
	MAWR,MARR,VWR,vdc3,vdc4,CR,RCR,BXR,
	BYR,MWR,HSR,HDR,VPR,VDW,VCR,DCR,
	SOUR,DISTR,LENR,SATB};


#define	SpHitON		   (io.VDC[CR].W&0x01)
#define	OverON		   (io.VDC[CR].W&0x02)
#define	RasHitON	   (io.VDC[CR].W&0x04)
#define	VBlankON	   (io.VDC[CR].W&0x08)
#define	SpriteON	   (io.VDC[CR].W&0x40)
#define	ScreenON	   (io.VDC[CR].W&0x80)

#define	VRAMSIZE	   0x20000

#define	NODATA	   0xff
#define	ENABLE	   1
#define	DISABLE	   0

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

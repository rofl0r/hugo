/***************************************************************************/
/*                                                                         */
/*                         HARDware PCEngine                               */
/*                                                                         */
/* This header deals with definition of structure and functions used to    */
/* handle the pc engine hardware in itself (RAM, IO ports, ...) which were */
/* previously found in pce.h                                               */
/*                                                                         */
/***************************************************************************/
#ifndef _INCLUDE_HARD_PCE_H
#define _INCLUDE_HARD_PCE_H

#include "cleantyp.h"

/**
  * Exported structure types
  **/

typedef union
{
  struct { UChar l,h; } B;
  UInt16 W;
} pair;

/* The structure containing all variables relatives to Input and Output */
typedef struct tagIO {
        /* VCE */
	pair VCE[0x200]; /* palette info */
	pair vce_reg;    /* currently selected color */
	UChar vce_ratch; /* temporary value to keep track of the first byte
                          * when setting a 16 bits value with two byte access
                          */
	/* VDC */
        pair VDC[32];    /* value of each VDC register */
	UInt16 vdc_inc;  /* VRAM pointer increment once accessed */
	UInt16 vdc_raster_count; /* unused as far as I know */
	UChar vdc_reg;   /* currently selected VDC register */
	UChar vdc_status; /* current VCD status (end of line, end of screen, ...) */
	UChar vdc_ratch; /* temporary value to keep track of the first byte
                          * when setting a 16 bits value with two byte access
                          */
	UChar vdc_satb;  /* boolean which keeps track of the need to copy
                          * the SATB from VRAM to internal SATB
                          */
	UChar vdc_pendvsync; /* unsure, set if a end of screen IRQ is waiting */
	Int32 bg_h;      /* number of tiles vertically in virtual screen */
	Int32 bg_w;      /* number of tiles horizontaly in virtual screen */
	Int32 screen_w;  /* size of real screen in pixels */
	Int32 screen_h;  /* size of real screen in pixels */
	Int32 scroll_y;
	Int32 minline;
        Int32 maxline;
	/* joypad */
	UChar JOY[16];   /* value of pressed button/direct for each pad
                          * (why 16 ? 5 should be enough for everyone :)
                          */
	UChar joy_select; /* used to know what nibble we must return */
	UChar joy_counter; /* current addressed joypad */
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

        /* Adpcm related variables */
	pair adpcm_ptr;
	UInt16 adpcm_rptr,adpcm_wptr;
        UInt16 adpcm_dmaptr;
        UChar adpcm_rate;
        UInt32 adpcm_pptr; /* to know where to begin playing adpcm (in nibbles) */
        UInt32 adpcm_psize; /* to know how many 4-bit samples to play */

} IO;

/**
  * Exported functions to access hardware
  **/

void    IO_write(UInt16 A,UChar V);
UChar   IO_read(UInt16 A);

/**
  * Exported variables
  **/

extern UChar RAM[0x8000];
// mem where variables are stocked (well, RAM... )
// in reality, only 0x2000 bytes are used in a coregraphx and 0x8000 only
// in a supergraphx

extern UChar *WRAM;
// extra backup memory
// This memory lies in Interface Unit or eventually in RGB adaptator

extern UChar *VRAM;
// Video mem
// 0x10000 bytes on coregraphx, the double on supergraphx I think
// contain information about the sprites position/status, information
// about the pattern and palette to use for each tile, and patterns
// for use in sprite/tile rendering

extern UInt16 SPRAM[64*4];
// SPRAM = sprite RAM
// The pc engine got a function to transfert a piece VRAM toward the inner
// gfx cpu sprite memory from where data will be grabbed to render sprites

extern UChar Pal[512];
// PCE->PC Palette convetion array
// Each of the 512 available PCE colors (333 RGB -> 512 colors)
// got a correspondancy in the 256 fixed colors palette

extern UInt32 *VRAM2,*VRAMS;
// These are array to keep in memory the result of the linearisation of
// PCE sprites and tiles

extern UChar *vchange,*vchanges;
// These array are boolean array to know if we must update the
// corresponding linear sprite representation in VRAM2 and VRAMS or not
// if (vchanges[5] != 0) 6th pattern in VRAM2 must be updated

extern UChar *PCM;
// The ADPCM array (0x10000 bytes)

extern UChar cd_port_1800;
extern UChar cd_port_1801;
extern UChar cd_port_1802;
extern UChar cd_port_1804;
// Some meaningfull variables to emulate cd ports

extern UChar *cd_sector_buffer;
// A pointer to know where we're currently reading data in the cd buffer

extern UChar *cd_read_buffer;
// The real buffer into which data are written from the cd and in which we
// takes data to gives it back throught the cd ports

extern UChar *cd_extra_mem;
// extra ram provided by the system CD card

extern UChar *cd_extra_super_mem;
// extra ram provided by the super system CD card

extern UInt32 pce_cd_read_datacnt;
// remaining useful data in cd_read_buffer

extern UChar cd_sectorcnt;
// number of sectors we must still read on cd

extern UChar pce_cd_curcmd;
// number of the current command of the cd interface

extern UChar* zp_base;
// pointer to the beginning of the Zero Page area

extern UChar* sp_base;
// pointer to the beginning of the Stack Area

extern UChar mmr[8];
// Value of each of the MMR registers

extern UChar irequest;
// boolean to know whether some interrupts are waiting for interrupt unmasking

UChar aftercli;
// boolean to know whether we've just encoutered a cli

extern UInt32 cyclecount;
// Number of elapsed cycles

UInt32 cyclecountold;
// Previous number of elapsed cycles

UInt32 ibackup;
// Backup value for elapsed cycle when executing interrupts

extern const TimerPeriod;
// Base period for the timer

// registers:

extern UInt16 reg_pc;
extern UChar  reg_a;
extern UChar  reg_x;
extern UChar  reg_y;
extern UChar  reg_p;
extern UChar  reg_s;
// These are the main h6280 register, reg_p is the flag register

extern UChar  halt_flag;
extern long   cycles;
extern long   frames;
// TODO : comment and/or remove those variables

/**
  * Definitions to ease writing
  **/

#define	VRR	2
enum _VDC_REG {
	MAWR,MARR,VWR,vdc3,vdc4,CR,RCR,BXR,
	BYR,MWR,HSR,HDR,VPR,VDW,VCR,DCR,
	SOUR,DISTR,LENR,SATB};

#define	NODATA	   0xff
#define	ENABLE	   1
#define	DISABLE	   0


#endif

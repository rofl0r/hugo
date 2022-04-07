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

#include <stdio.h>

#include "config.h"
#include "cleantyp.h"

#if defined(SHARED_MEMORY)

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#endif

#define PSG_VOICE_REG           0 /* voice index */

#define PSG_VOLUME_REG          1 /* master volume */

#define PSG_FREQ_LSB_REG        2 /* lower 8 bits of 12 bit frequency */

#define PSG_FREQ_MSB_REG        3 /* actually most significant nibble */

#define PSG_DDA_REG             4
#define PSG_DDA_ENABLE          0x80 /* bit 7 */
#define PSG_DDA_DIRECT_ACCESS   0x40 /* bit 6 */
#define PSG_DDA_VOICE_VOLUME    0x1F /* bits 0-4 */

#define PSG_BALANCE_REG         5
#define PSG_BALANCE_LEFT        0xF0 /* bits 4-7 */
#define PSG_BALANCE_RIGHT       0x0F /* bits 0-3 */

#define PSG_DATA_INDEX_REG      6

#define PSG_NOISE_REG           7
#define PSG_NOISE_ENABLE        0x80 /* bit 7 */

#define PSG_DIRECT_ACCESS_BUFSIZE 1024

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
	UChar PSG[6][8], wave[6][32];
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
        // 6 : wave ringbuffer index
        // 7 : noise data for channels 5 and 6

	UChar psg_ch,psg_volume,psg_lfo_freq,psg_lfo_ctrl;

    UChar psg_da_data[6][PSG_DIRECT_ACCESS_BUFSIZE];
    UInt16 psg_da_index[6], psg_da_count[6];
    boolean psg_channel_disabled[6];

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

        /* Arcade Card variables */
        UInt32 ac_base[4];     /* base address for AC ram accessing */
        UInt16 ac_offset[4];   /* offset address for AC ram accessing */
        UInt16 ac_incr[4];     /* incrment value after read or write accordingly to the control bit */

        UChar  ac_control[4];  /* bit 7: unused */

                               /* bit 6: only $1AX6 hits will add offset to base*/
                               /* bit 5 + bit 6: either hit to $1AX6 or $1AXA will add offset to base */

                               /* bit 4: auto increment offset if 0, and auto */
                               /* increment base if 1 */
                               /* bit 3: unknown */
                               /* bit 2: unknown */
                               /* bit 1: use offset address in the effective address */
                               /*   computation */

                               /* bit 0: apply autoincrement if set */

        UInt32 ac_shift;
        UChar  ac_shiftbits;   /* number of bits to shift by */

/*        UChar  ac_unknown3; */
        UChar  ac_unknown4;

} IO;

/**
  * Exported functions to access hardware
  **/

void    IO_write (UInt16 A,UChar V);
UChar   IO_read  (UInt16 A);
void    bank_set (UChar P, UChar V);

/**
  * Global structure for all hardware variables
  **/

#include "shared_memory.h"

/**
  * Exported variables
  **/

extern struct_hard_pce* hard_pce;
// The global structure for all hardware variables

extern IO io;
// the global I/O status

extern UChar *RAM;
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

extern UInt16 *SPRAM;
// SPRAM = sprite RAM
// The pc engine got a function to transfert a piece VRAM toward the inner
// gfx cpu sprite memory from where data will be grabbed to render sprites

extern UChar *Pal;
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

#define scanline (*p_scanline)

extern UInt32 *p_scanline;
// The current rendered line on screen

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

extern UChar *ac_extra_mem;
// extra ram provided by the Arcade card

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

extern UChar* mmr;
// Value of each of the MMR registers

extern UChar *Page[8];
// physical address on emulator machine less 0x2000 by index (trick to access "faster" the mem)

extern UChar *IOAREA;
// physical address on emulator machine of the IO area (fake address as it has to be handled specially)

extern UChar *ROMMap[256];
// physical address on emulator machine of each of the 256 banks

#define irequest (*p_irequest)

extern UChar *p_irequest;
// bit field storing awaiting interruptions

#define aftercli (*p_aftercli)

extern UChar *p_aftercli;
// boolean to know whether we've just encoutered a cli

#define cyclecount (*p_cyclecount)

extern UInt32 *p_cyclecount;
// Number of elapsed cycles

#define cyclecountold (*p_cyclecountold)

extern UInt32 *p_cyclecountold;
// Previous number of elapsed cycles

#define ibackup (*p_ibackup)

extern UInt32 *p_ibackup;
// Backup value for elapsed cycle when executing interrupts

#define external_control_cpu (*p_external_control_cpu)

extern SInt32 *p_external_control_cpu;

extern const UInt32 TimerPeriod;
// Base period for the timer

// registers:

#if defined(SHARED_MEMORY)

#define reg_pc	(*p_reg_pc)
#define reg_a   (*p_reg_a)
#define reg_x   (*p_reg_x)
#define reg_y   (*p_reg_y)
#define reg_p   (*p_reg_p)
#define reg_s   (*p_reg_s)

extern UInt16 *p_reg_pc;
extern UChar  *p_reg_a;
extern UChar  *p_reg_x;
extern UChar  *p_reg_y;
extern UChar  *p_reg_p;
extern UChar  *p_reg_s;

#else
extern UInt16 reg_pc;
extern UChar  reg_a;
extern UChar  reg_x;
extern UChar  reg_y;
extern UChar  reg_p;
extern UChar  reg_s;
#endif

// These are the main h6280 register, reg_p is the flag register

#define cycles (*p_cycles)

extern UInt32 *p_cycles;
// Number of pc engine cycles elapsed since the resetting of the emulated console

#define frames (*p_frames)

extern UInt32 *p_frames;
// Number of frames the console would have rendered (i.e. when setting frameskip to 1,
// This is the double of frame rendered on pc)

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

#define AC_ENABLE_OFFSET_BASE_6 0x40
#define AC_ENABLE_OFFSET_BASE_A 0x20
#define AC_INCREMENT_BASE 0x10
#define AC_USE_OFFSET 0x02
#define AC_ENABLE_INC 0x01

#endif

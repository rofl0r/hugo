#ifndef INCLUDE_SHARED_MEMORY
#define INCLUDE_SHARED_MEMORY

#include "cleantyp.h"

#define	VRAMSIZE	   0x10000

typedef struct {
	UChar RAM[0x8000];
	UChar PCM[0x10000];
	UChar WRAM[0x2000];
	UChar VRAM[VRAMSIZE];
	UChar VRAM2[VRAMSIZE];
	UChar VRAMS[VRAMSIZE];
	UChar vchange[VRAMSIZE / 32];
	UChar vchanges[VRAMSIZE / 128];
	UInt32 s_scanline;
	
	UInt16 SPRAM[64 * 4];
	UChar  Pal[512];
	
	UInt16 s_reg_pc;
	UChar  s_reg_a;
	UChar  s_reg_x;
	UChar  s_reg_y;
	UChar  s_reg_p;
	UChar  s_reg_s;
	
	UChar  s_irequest;
	UChar  s_aftercli;
	UInt32 s_cyclecount;
	UInt32 s_cyclecountold;
	UInt32 s_ibackup;
	
	UInt32 s_cycles;
	UInt32 s_frames;
	
	SInt32 s_external_control_cpu;
	
	UChar mmr[8];
		
} struct_hard_pce;

#endif

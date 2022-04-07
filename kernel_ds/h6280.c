//  h6280.c - Execute CPU instructions
//

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#include "interupt.h"
#include "dis.h"
#include "pce.h"

#define DEBUG_KERNEL_DS1

extern IO io;

extern UChar* Page[];

extern UChar *ROMMap[];

extern UChar* IOAREA;

UChar* zp_base;
UChar* sp_base;
UChar mmr[8];

UChar irequest;
UChar aftercli;
UInt32 cyclecount;
UInt32 cyclecountold;
UInt32 ibackup;

const TimerPeriod = 1097;

// registers:

UInt16 reg_pc;
UChar  reg_a;
UChar  reg_x;
UChar  reg_y;
UChar  reg_p;
UChar  reg_s;

UChar  halt_flag=0;
long   cycles=0;
long   frames=0;

// flag-value table (for speed)

UChar flnz_list[256] = {
FL_Z,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // 00-0F
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,       // 40-4F
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,       // 70-7F
FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,  // 80-87
FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,
FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,  // 90-97
FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,
FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,  // A0-A7
FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,
FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,  // B0-B7
FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,
FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,  // C0-C7
FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,
FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,  // D0-D7
FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,
FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,  // E0-E7
FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,
FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,  // F0-F7
FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N,FL_N
};

// Elementary operations:
// - defined here for clarity, brevity,
//   and reduced typographical errors

//Addressing modes:

#define abs_operand(x)	   get_8bit_addr(get_16bit_addr(x))
#define absx_operand(x)    get_8bit_addr(get_16bit_addr(x)+reg_x)
#define absy_operand(x)    get_8bit_addr(get_16bit_addr(x)+reg_y)
#define zp_operand(x)	   get_8bit_zp(imm_operand(x))
#define zpx_operand(x)	   get_8bit_zp(imm_operand(x)+reg_x)
#define zpy_operand(x)	   get_8bit_zp(imm_operand(x)+reg_y)
#define zpind_operand(x)   get_8bit_addr(get_16bit_zp(imm_operand(x)))
#define zpindx_operand(x)  get_8bit_addr(get_16bit_zp(imm_operand(x)+reg_x))
#define zpindy_operand(x)  get_8bit_addr(get_16bit_zp(imm_operand(x))+reg_y)

// Elementary flag check (flags 'N' and 'Z'):

#define chk_flnz_8bit(x) \
  reg_p = ((reg_p & (~(FL_N|FL_T|FL_Z))) | flnz_list[x]);

// This code ignores hardware-segment accesses; it should only be used
// to access immediate data (hardware segment does not run code):
//
//  as a function:

inline UChar imm_operand(UInt16 addr) {
  register unsigned short int memreg = addr>>13;
  return( (UChar) (Page[memreg][addr]));
}

//
// as a define:  (inline doesn't always inline it)
//#define imm_operand(x)     (UChar) *(mmr_base[(x)>>13] + ((x) & 0x1FFF))

// This is the more generalized access routine:
inline UChar get_8bit_addr(UInt16 addr) {
  register unsigned short int memreg = addr>>13;
  if (Page[memreg] == IOAREA)
    return(IO_read(addr));
  else
    return((UChar) (Page[memreg][addr]));
}

inline int put_8bit_addr(UInt16 addr, UChar byte) {
  register unsigned int memreg = addr>>13;
  if (Page[memreg] == IOAREA) {
    IO_write(addr, byte);
  } else {
    Page[memreg][addr] = byte;
  }
  return(0);
}

inline UInt16 get_16bit_addr(UInt16 addr) {
  register unsigned int memreg = addr>>13;
  UInt16 ret_16bit = (UChar) Page[memreg][addr];
  memreg = (++addr)>>13;
  ret_16bit += (UInt16) ((UChar) Page[memreg][addr] << 8);

  return(ret_16bit);
}

inline UChar get_8bit_zp(UChar zp_addr) {
  return((UChar) *(zp_base + zp_addr) );
}

inline UInt16 get_16bit_zp(UChar zp_addr) {
  UInt16 n = *(zp_base + zp_addr);
  n += (*(zp_base + (UChar)(zp_addr+1)) << 8);
  return(n);
}

inline void put_8bit_zp(UChar zp_addr, UChar byte) {
  *(zp_base + zp_addr) = byte;
}

inline void push_8bit(UChar byte) {
  *(sp_base + reg_s--) = byte;
}

inline UChar pull_8bit(void) {
  return((UChar) *(sp_base + ++reg_s) );
}

inline void push_16bit(UInt16 addr) {
  *(sp_base + reg_s--) = (UChar)(addr>>8);
  *(sp_base + reg_s--) = (UChar)(addr&0xFF);
  return;
}

inline UInt16 pull_16bit(void) {
  UInt16 n = (UChar) *(sp_base + ++reg_s);
  n += (UInt16)(((UChar) *(sp_base + ++reg_s)) << 8);
  return(n);
}

//
// Implementation of actual opcodes:
//

UChar adc(UChar acc, UChar val) {
  Int16  sig  = (Char)acc;
  UInt16 usig = (UChar)acc;
  UInt16 temp, temp1;

  if (!(reg_p & FL_D)) {		/* binary mode */
    if (reg_p & FL_C) {
      usig++;
      sig++;
    }
    sig  += (Char)val;
    usig += (UChar)val;
    acc   = usig & 0xFF;

    reg_p = (reg_p & ~(FL_N|FL_V|FL_T|FL_Z|FL_C))
	    | (((sig > 127) || (sig < -128)) ? FL_V:0)
	    | ((usig > 255) ? FL_C:0)
	    | flnz_list[acc];

  } else {				/* decimal mode */

// treatment of out-of-range accumulator
// and operand values (non-BCD) is not
// adequately defined.	Nor is overflow
// flag treatment.

// Zeo : rewrote using bcdbin and binbcd arrays to boost code speed and fix
// residual bugs

    temp  = bcdbin[usig] + bcdbin[val];

    if (reg_p & FL_C) { temp++; }

    acc    = binbcd[temp];

    reg_p  = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	     | ((temp > 99) ? FL_C:0)
	     | flnz_list[acc];

    cycles++;	/* decimal mode takes an extra cycle */

  }
  return(acc);
}

void sbc(UChar val) {
  Int16  sig  = (Char)reg_a;
  UInt16 usig = (UChar)reg_a;
  Int16  temp, temp1;

  if (!(reg_p & FL_D)) {		/* binary mode */
    if (!(reg_p & FL_C)) {
      usig--;
      sig--;
    }
    sig   -= (Char)val;
    usig  -= (UChar)val;
    reg_a  = (usig & 0xFF);
    reg_p  = (reg_p & ~(FL_N|FL_V|FL_T|FL_Z|FL_C))
	     | (((sig > 127) || (sig < -128)) ? FL_V:0)
	     | ((usig > 255) ? 0:FL_C)
	     | flnz_list[reg_a];      /* FL_N, FL_Z */

  } else {				/* decimal mode */

// treatment of out-of-range accumulator
// and operand values (non-bcd) is not
// adequately defined.	Nor is overflow
// flag treatment.

    temp  = bcdbin[usig] - bcdbin[val];

    if (!(reg_p & FL_C)) { temp--; }

    reg_p  = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	     | ((temp < 0) ? 0:FL_C);

    while (temp < 0) {
      temp += 100;
    }

    chk_flnz_8bit(reg_a = binbcd[temp]);

    cycles++;	/* decimal mode takes an extra cycle */

  }
}

int adc_abs(void) {
// if flag 'T' is set, use zero-page address specified by register 'X'
// as the accumulator...

  if (reg_p & FL_T) {
    put_8bit_zp(reg_x, adc(get_8bit_zp(reg_x), abs_operand(reg_pc+1)));
    cycles+=8;
  } else {
    reg_a = adc(reg_a, abs_operand(reg_pc+1));
    cycles+=5;
  }
  reg_pc+=3;
  return 0;
}

int adc_absx(void) {
  if (reg_p & FL_T) {
    put_8bit_zp(reg_x, adc(get_8bit_zp(reg_x), absx_operand(reg_pc+1)));
    cycles+=8;
  } else {
    reg_a = adc(reg_a, absx_operand(reg_pc+1));
    cycles+=5;
  }
  reg_pc+=3;
  return 0;
}

int adc_absy(void) {
  if (reg_p & FL_T) {
    put_8bit_zp(reg_x, adc(get_8bit_zp(reg_x), absy_operand(reg_pc+1)));
    cycles+=8;
  } else {
    reg_a = adc(reg_a, absy_operand(reg_pc+1));
    cycles+=5;
  }
  reg_pc+=3;
  return 0;
}

int adc_imm(void) {
  if (reg_p & FL_T) {
    put_8bit_zp(reg_x, adc(get_8bit_zp(reg_x), imm_operand(reg_pc+1)));
    cycles+=5;
  } else {
    reg_a = adc(reg_a, imm_operand(reg_pc+1));
    cycles+=2;
  }
  reg_pc+=2;
  return 0;
}

int adc_zp(void) {
  if (reg_p & FL_T) {
    put_8bit_zp(reg_x, adc(get_8bit_zp(reg_x), zp_operand(reg_pc+1)));
    cycles+=7;
  } else {
    reg_a = adc(reg_a, zp_operand(reg_pc+1));
    cycles+=4;
  }
  reg_pc+=2;
  return 0;
}

int adc_zpx(void) {
  if (reg_p & FL_T) {
    put_8bit_zp(reg_x, adc(get_8bit_zp(reg_x), zpx_operand(reg_pc+1)));
    cycles+=7;
  } else {
    reg_a = adc(reg_a, zpx_operand(reg_pc+1));
    cycles+=4;
  }
  reg_pc+=2;
  return 0;
}

int adc_zpind(void) {
  if (reg_p & FL_T) {
    put_8bit_zp(reg_x, adc(get_8bit_zp(reg_x), zpind_operand(reg_pc+1)));
    cycles+=10;
  } else {
    reg_a = adc(reg_a, zpind_operand(reg_pc+1));
    cycles+=7;
  }
  reg_pc+=2;
  return 0;
}

int adc_zpindx(void) {
  if (reg_p & FL_T) {
    put_8bit_zp(reg_x, adc(get_8bit_zp(reg_x), zpindx_operand(reg_pc+1)));
    cycles+=10;
  } else {
    reg_a = adc(reg_a, zpindx_operand(reg_pc+1));
    cycles+=7;
  }
  reg_pc+=2;
  return 0;
}

int adc_zpindy(void) {
  if (reg_p & FL_T) {
    put_8bit_zp(reg_x, adc(get_8bit_zp(reg_x), zpindy_operand(reg_pc+1)));
    cycles+=10;
  } else {
    reg_a = adc(reg_a, zpindy_operand(reg_pc+1));
    cycles+=7;
  }
  reg_pc+=2;
  return 0;
}

int and_abs(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp &= abs_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=8;

  } else {
    chk_flnz_8bit(reg_a &= abs_operand(reg_pc+1));
    cycles+=5;
  }
  reg_pc+=3;
  return 0;
}

int and_absx(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp &= absx_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=8;

  } else {
    chk_flnz_8bit(reg_a &= absx_operand(reg_pc+1));
    cycles+=5;
  }
  reg_pc+=3;
  return 0;
}

int and_absy(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp &= absy_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=8;

  } else {
    chk_flnz_8bit(reg_a &= absy_operand(reg_pc+1));
    cycles+=5;
  }
  reg_pc+=3;
  return 0;
}

int and_imm(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp &= imm_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=5;

  } else {
    chk_flnz_8bit(reg_a &= imm_operand(reg_pc+1));
    cycles+=2;
  }
  reg_pc+=2;
  return 0;
}

int and_zp(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp &= zp_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=7;

  } else {
    chk_flnz_8bit(reg_a &= zp_operand(reg_pc+1));
    cycles+=4;
  }
  reg_pc+=2;
  return 0;
}

int and_zpx(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp &= zpx_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=7;

  } else {
    chk_flnz_8bit(reg_a &= zpx_operand(reg_pc+1));
    cycles+=4;
  }
  reg_pc+=2;
  return 0;
}

int and_zpind(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp &= zpind_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=10;

  } else {
    chk_flnz_8bit(reg_a &= zpind_operand(reg_pc+1));
    cycles+=7;
  }
  reg_pc+=2;
  return 0;
}

int and_zpindx(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp &= zpindx_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=10;

  } else {
    chk_flnz_8bit(reg_a &= zpindx_operand(reg_pc+1));
    cycles+=7;
  }
  reg_pc+=2;
  return 0;
}

int and_zpindy(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp &= zpindy_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=10;

  } else {
    chk_flnz_8bit(reg_a &= zpindy_operand(reg_pc+1));
    cycles+=7;
  }
  reg_pc+=2;
  return 0;
}

int asl_a(void) {
  UChar temp1 = reg_a;
  reg_a<<=1;
  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((temp1 & 0x80) ? FL_C:0)
	  | flnz_list[reg_a];
  cycles+=2;
  reg_pc++;
  return 0;
}

int asl_abs(void) {
  UInt16 temp_addr = get_16bit_addr(reg_pc+1);
  UChar  temp1	   = get_8bit_addr(temp_addr);
  UChar  temp	   = temp1<<1;

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((temp1 & 0x80) ? FL_C:0)
	  | flnz_list[temp];
  cycles+=7;

  if (put_8bit_addr(temp_addr,temp)) {	// if error writing to address,
    return 1;				// stop before advancing PC
  }
  reg_pc+=3;
  return 0;
}

int asl_absx(void) {
  UInt16 temp_addr = get_16bit_addr(reg_pc+1)+reg_x;
  UChar  temp1	   = get_8bit_addr(temp_addr);
  UChar  temp	   = temp1<<1;

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((temp1 & 0x80) ? FL_C:0)
	  | flnz_list[temp];
  cycles+=7;

  if (put_8bit_addr(temp_addr,temp)) {	// if error writing to address,
    return 1;				// stop before advancing PC
  }
  reg_pc+=3;
  return 0;
}

int asl_zp(void) {
  UChar zp_addr = imm_operand(reg_pc+1);
  UChar temp1	= get_8bit_zp(zp_addr);
  UChar temp	= temp1<<1;

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((temp1 & 0x80) ? FL_C:0)
	  | flnz_list[temp];
  cycles+=6;
  put_8bit_zp(zp_addr, temp);
  reg_pc+=2;
  return 0;
}

int asl_zpx(void) {
  UChar  zp_addr = imm_operand(reg_pc+1)+reg_x;
  UChar  temp1	 = get_8bit_zp(zp_addr);
  UChar  temp	 = temp1<<1;

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((temp1 & 0x80) ? FL_C:0)
	  | flnz_list[temp];
  cycles+=6;
  put_8bit_zp(zp_addr, temp);
  reg_pc+=2;
  return 0;
}

int bbr0(void) {
  reg_p &= ~FL_T;
  if (zp_operand(reg_pc+1)&0x01) {
    reg_pc+=3;
    cycles+=6;
  } else {
    reg_pc+=(Char)imm_operand(reg_pc+2)+3;
    cycles+=8;
  }
  return 0;
}

int bbr1(void) {
  reg_p &= ~FL_T;
  if (zp_operand(reg_pc+1)&0x02) {
    reg_pc+=3;
    cycles+=6;
  } else {
    reg_pc+=(Char)imm_operand(reg_pc+2)+3;
    cycles+=8;
  }
  return 0;
}

int bbr2(void) {
  reg_p &= ~FL_T;
  if (zp_operand(reg_pc+1)&0x04) {
    reg_pc+=3;
    cycles+=6;
  } else {
    reg_pc+=(Char)imm_operand(reg_pc+2)+3;
    cycles+=8;
  }
  return 0;
}

int bbr3(void) {
  reg_p &= ~FL_T;
  if (zp_operand(reg_pc+1)&0x08) {
    reg_pc+=3;
    cycles+=6;
  } else {
    reg_pc+=(Char)imm_operand(reg_pc+2)+3;
    cycles+=8;
  }
  return 0;
}

int bbr4(void) {
  reg_p &= ~FL_T;
  if (zp_operand(reg_pc+1)&0x10) {
    reg_pc+=3;
    cycles+=6;
  } else {
    reg_pc+=(Char)imm_operand(reg_pc+2)+3;
    cycles+=8;
  }
  return 0;
}

int bbr5(void) {
  reg_p &= ~FL_T;
  if (zp_operand(reg_pc+1)&0x20) {
    reg_pc+=3;
    cycles+=6;
  } else {
    reg_pc+=(Char)imm_operand(reg_pc+2)+3;
    cycles+=8;
  }
  return 0;
}

int bbr6(void) {
  reg_p &= ~FL_T;
  if (zp_operand(reg_pc+1)&0x40) {
    reg_pc+=3;
    cycles+=6;
  } else {
    reg_pc+=(Char)imm_operand(reg_pc+2)+3;
    cycles+=8;
  }
  return 0;
}

int bbr7(void) {
  reg_p &= ~FL_T;
  if (zp_operand(reg_pc+1)&0x80) {
    reg_pc+=3;
    cycles+=6;
  } else {
    reg_pc+=(Char)imm_operand(reg_pc+2)+3;
    cycles+=8;
  }
  return 0;
}

int bbs0(void) {
  reg_p &= ~FL_T;
  if (zp_operand(reg_pc+1)&0x01) {
    reg_pc+=(Char)imm_operand(reg_pc+2)+3;
    cycles+=8;
  } else {
    reg_pc+=3;
    cycles+=6;
  }
  return 0;
}

int bbs1(void) {
  reg_p &= ~FL_T;
  if (zp_operand(reg_pc+1)&0x02) {
    reg_pc+=(Char)imm_operand(reg_pc+2)+3;
    cycles+=8;
  } else {
    reg_pc+=3;
    cycles+=6;
  }
  return 0;
}

int bbs2(void) {
  reg_p &= ~FL_T;
  if (zp_operand(reg_pc+1)&0x04) {
    reg_pc+=(Char)imm_operand(reg_pc+2)+3;
    cycles+=8;
  } else {
    reg_pc+=3;
    cycles+=6;
  }
  return 0;
}

int bbs3(void) {
  reg_p &= ~FL_T;
  if (zp_operand(reg_pc+1)&0x08) {
    reg_pc+=(Char)imm_operand(reg_pc+2)+3;
    cycles+=8;
  } else {
    reg_pc+=3;
    cycles+=6;
  }
  return 0;
}

int bbs4(void) {
  reg_p &= ~FL_T;
  if (zp_operand(reg_pc+1)&0x10) {
    reg_pc+=(Char)imm_operand(reg_pc+2)+3;
    cycles+=8;
  } else {
    reg_pc+=3;
    cycles+=6;
  }
  return 0;
}

int bbs5(void) {
  reg_p &= ~FL_T;
  if (zp_operand(reg_pc+1)&0x20) {
    reg_pc+=(Char)imm_operand(reg_pc+2)+3;
    cycles+=8;
  } else {
    reg_pc+=3;
    cycles+=6;
  }
  return 0;
}

int bbs6(void) {
  reg_p &= ~FL_T;
  if (zp_operand(reg_pc+1)&0x40) {
    reg_pc+=(Char)imm_operand(reg_pc+2)+3;
    cycles+=8;
  } else {
    reg_pc+=3;
    cycles+=6;
  }
  return 0;
}

int bbs7(void) {
  reg_p &= ~FL_T;
  if (zp_operand(reg_pc+1)&0x80) {
    reg_pc+=(Char)imm_operand(reg_pc+2)+3;
    cycles+=8;
  } else {
    reg_pc+=3;
    cycles+=6;
  }
  return 0;
}

int bcc(void) {
  reg_p &= ~FL_T;
  if (reg_p & FL_C) {
    reg_pc+=2;
    cycles+=2;
  } else {
    reg_pc+=(Char)imm_operand(reg_pc+1)+2;
    cycles+=4;
  }
  return 0;
}

int bcs(void) {
  reg_p &= ~FL_T;
  if (reg_p & FL_C) {
    reg_pc+=(Char)imm_operand(reg_pc+1)+2;
    cycles+=4;
  } else {
    reg_pc+=2;
    cycles+=2;
  }
  return 0;
}

int beq(void) {
  reg_p &= ~FL_T;
  if (reg_p & FL_Z) {
    reg_pc+=(Char)imm_operand(reg_pc+1)+2;
    cycles+=4;
  } else {
    reg_pc+=2;
    cycles+=2;
  }
  return 0;
}

int bit_abs(void) {
  UChar temp = abs_operand(reg_pc+1);
  reg_p = (reg_p & ~(FL_N|FL_V|FL_T|FL_Z))
	  | ((temp&0x80)  ? FL_N:0)
	  | ((temp&0x40)  ? FL_V:0)
	  | ((reg_a&temp) ? 0:FL_Z);
  reg_pc+=3;
  cycles+=5;
  return 0;
}

int bit_absx(void) {
  UChar temp = absx_operand(reg_pc+1);
  reg_p = (reg_p & ~(FL_N|FL_V|FL_T|FL_Z))
	  | ((temp&0x80)  ? FL_N:0)
	  | ((temp&0x40)  ? FL_V:0)
	  | ((reg_a&temp) ? 0:FL_Z);
  reg_pc+=3;
  cycles+=5;
  return 0;
}

int bit_imm(void) {
// orig code (Eyes/Lichty said immediate mode did not affect
//            'N' and 'V' flags):
//reg_p = (reg_p & ~(FL_T|FL_Z))
//	  | ((reg_a & imm_operand(reg_pc+1)) ? 0:FL_Z);

  UChar temp = imm_operand(reg_pc+1);
  reg_p = (reg_p & ~(FL_N|FL_V|FL_T|FL_Z))
	  | ((temp&0x80)  ? FL_N:0)
	  | ((temp&0x40)  ? FL_V:0)
	  | ((reg_a&temp) ? 0:FL_Z);
  reg_pc+=2;
  cycles+=2;
  return 0;
}

int bit_zp(void) {
  UChar temp = zp_operand(reg_pc+1);
  reg_p = (reg_p & ~(FL_N|FL_V|FL_T|FL_Z))
	  | ((temp&0x80)  ? FL_N:0)
	  | ((temp&0x40)  ? FL_V:0)
	  | ((reg_a&temp) ? 0:FL_Z);
  reg_pc+=2;
  cycles+=4;
  return 0;
}

int bit_zpx(void) {
  UChar temp = zpx_operand(reg_pc+1);
  reg_p = (reg_p & ~(FL_N|FL_V|FL_T|FL_Z))
	  | ((temp&0x80)  ? FL_N:0)
	  | ((temp&0x40)  ? FL_V:0)
	  | ((reg_a&temp) ? 0:FL_Z);
  reg_pc+=2;
  cycles+=4;
  return 0;
}

int bmi(void) {
  reg_p &= ~FL_T;
  if (reg_p & FL_N) {
    reg_pc+=(Char)imm_operand(reg_pc+1)+2;
    cycles+=4;
  } else {
    reg_pc+=2;
    cycles+=2;
  }
  return 0;
}

int bne(void) {
  reg_p &= ~FL_T;
  if (reg_p & FL_Z) {
    reg_pc+=2;
    cycles+=2;
  } else {
    reg_pc+=(Char)imm_operand(reg_pc+1)+2;
    cycles+=4;
  }
  return 0;
}

int bpl(void) {
  reg_p &= ~FL_T;
  if (reg_p & FL_N) {
    reg_pc+=2;
    cycles+=2;
  } else {
    reg_pc+=(Char)imm_operand(reg_pc+1)+2;
    cycles+=4;
  }
  return 0;
}

int bra(void) {
  reg_p &= ~FL_T;
  reg_pc+=(Char)imm_operand(reg_pc+1)+2;
  cycles+=4;
  return 0;
}

int brek(void) {
  push_16bit(reg_pc+2);
  reg_p &= ~FL_T;
  push_8bit(reg_p|FL_B);
  reg_p =(reg_p & ~FL_D) | FL_I;
  reg_pc=get_16bit_addr(0xFFF6);
  cycles+=8;
  return 0;
}

int bsr(void) {
  reg_p &= ~FL_T;
  push_16bit(reg_pc+1);
  reg_pc+=(Char)imm_operand(reg_pc+1)+2;
  cycles+=8;
  return 0;
}

int bvc(void) {
  reg_p &= ~FL_T;
  if (reg_p & FL_V) {
    reg_pc+=2;
    cycles+=2;
  } else {
    reg_pc+=(Char)imm_operand(reg_pc+1)+2;
    cycles+=4;
  }
  return 0;
}

int bvs(void) {
  reg_p &= ~FL_T;
  if (reg_p & FL_V) {
    reg_pc+=(Char)imm_operand(reg_pc+1)+2;
    cycles+=4;
  } else {
    reg_pc+=2;
    cycles+=2;
  }
  return 0;
}

int cla(void) {
  reg_p &= ~FL_T;
  reg_a = 0;
  reg_pc++;
  cycles+=2;
  return 0;
}

int clc(void) {
  reg_p &= ~(FL_T|FL_C);
  reg_pc++;
  cycles+=2;
  return 0;
}

int cld(void) {
  reg_p &= ~(FL_T|FL_D);
  reg_pc++;
  cycles+=2;
  return 0;
}

int cli(void) {
  reg_p &= ~(FL_T|FL_I);
  reg_pc++;
  cycles+=2;
  return 0;
}

int clv(void) {
  reg_p &= ~(FL_V|FL_T);
  reg_pc++;
  cycles+=2;
  return 0;
}

int clx(void) {
  reg_p &= ~FL_T;
  reg_x = 0;
  reg_pc++;
  cycles+=2;
  return 0;
}

int cly(void) {
  reg_p &= ~FL_T;
  reg_y = 0;
  reg_pc++;
  cycles+=2;
  return 0;
}

int cmp_abs(void) {
  UChar temp = abs_operand(reg_pc+1);

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((reg_a < temp) ? 0:FL_C)
	  | flnz_list[(UChar)(reg_a-temp)];
  reg_pc+=3;
  cycles+=5;
  return 0;
}

int cmp_absx(void) {
  UChar temp = absx_operand(reg_pc+1);

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((reg_a < temp) ? 0:FL_C)
	  | flnz_list[(UChar)(reg_a-temp)];
  reg_pc+=3;
  cycles+=5;
  return 0;
}

int cmp_absy(void) {
  UChar temp = absy_operand(reg_pc+1);

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((reg_a < temp) ? 0:FL_C)
	  | flnz_list[(UChar)(reg_a-temp)];
  reg_pc+=3;
  cycles+=5;
  return 0;
}

int cmp_imm(void) {
  UChar temp = imm_operand(reg_pc+1);

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((reg_a < temp) ? 0:FL_C)
	  | flnz_list[(UChar)(reg_a-temp)];
  reg_pc+=2;
  cycles+=2;
  return 0;
}

int cmp_zp(void) {
  UChar temp = zp_operand(reg_pc+1);

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((reg_a < temp) ? 0:FL_C)
	  | flnz_list[(UChar)(reg_a-temp)];
  reg_pc+=2;
  cycles+=4;
  return 0;
}

int cmp_zpx(void) {
  UChar temp = zpx_operand(reg_pc+1);

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((reg_a < temp) ? 0:FL_C)
	  | flnz_list[(UChar)(reg_a-temp)];
  reg_pc+=2;
  cycles+=4;
  return 0;
}

int cmp_zpind(void) {
  UChar temp = zpind_operand(reg_pc+1);

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((reg_a < temp) ? 0:FL_C)
	  | flnz_list[(UChar)(reg_a-temp)];
  reg_pc+=2;
  cycles+=7;
  return 0;
}

int cmp_zpindx(void) {
  UChar temp = zpindx_operand(reg_pc+1);

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((reg_a < temp) ? 0:FL_C)
	  | flnz_list[(UChar)(reg_a-temp)];
  reg_pc+=2;
  cycles+=7;
  return 0;
}

int cmp_zpindy(void) {
  UChar temp = zpindy_operand(reg_pc+1);

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((reg_a < temp) ? 0:FL_C)
	  | flnz_list[(UChar)(reg_a-temp)];
  reg_pc+=2;
  cycles+=7;
  return 0;
}

int cpx_abs(void) {
  UChar temp = abs_operand(reg_pc+1);

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((reg_x < temp) ? 0:FL_C)
	  | flnz_list[(UChar)(reg_x-temp)];
  reg_pc+=3;
  cycles+=5;
  return 0;
}

int cpx_imm(void) {
  UChar temp = imm_operand(reg_pc+1);

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((reg_x < temp) ? 0:FL_C)
	  | flnz_list[(UChar)(reg_x-temp)];
  reg_pc+=2;
  cycles+=2;
  return 0;
}

int cpx_zp(void) {
  UChar temp = zp_operand(reg_pc+1);

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((reg_x < temp) ? 0:FL_C)
	  | flnz_list[(UChar)(reg_x-temp)];
  reg_pc+=2;
  cycles+=4;
  return 0;
}

int cpy_abs(void) {
  UChar temp = abs_operand(reg_pc+1);

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((reg_y < temp) ? 0:FL_C)
	  | flnz_list[(UChar)(reg_y-temp)];
  reg_pc+=3;
  cycles+=5;
  return 0;
}

int cpy_imm(void) {
  UChar temp = imm_operand(reg_pc+1);

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((reg_y < temp) ? 0:FL_C)
	  | flnz_list[(UChar)(reg_y-temp)];
  reg_pc+=2;
  cycles+=2;
  return 0;
}

int cpy_zp(void) {
  UChar temp = zp_operand(reg_pc+1);

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((reg_y < temp) ? 0:FL_C)
	  | flnz_list[(UChar)(reg_y-temp)];
  reg_pc+=2;
  cycles+=4;
  return 0;
}

int dec_a(void) {
  chk_flnz_8bit(--reg_a);
  reg_pc++;
  cycles+=2;
  return 0;
}

int dec_abs(void) {
  UChar  temp;
  UInt16 temp_addr = get_16bit_addr(reg_pc+1);
  chk_flnz_8bit(temp = get_8bit_addr(temp_addr)-1);
  cycles+=7;
  if (put_8bit_addr(temp_addr, temp)) {
     return 1;
  }
  reg_pc+=3;
  return 0;
}

int dec_absx(void) {
  UChar  temp;
  UInt16 temp_addr = get_16bit_addr(reg_pc+1)+reg_x;
  chk_flnz_8bit(temp = get_8bit_addr(temp_addr)-1);
  cycles+=7;
  if (put_8bit_addr(temp_addr, temp)) {
    return 1;
  }
  reg_pc+=3;
  return 0;
}

int dec_zp(void) {
  UChar  temp;
  UChar  zp_addr = imm_operand(reg_pc+1);
  chk_flnz_8bit(temp = get_8bit_zp(zp_addr)-1);
  put_8bit_zp(zp_addr, temp);
  reg_pc+=2;
  cycles+=6;
  return 0;
}

int dec_zpx(void) {
  UChar  temp;
  UChar  zp_addr = imm_operand(reg_pc+1)+reg_x;
  chk_flnz_8bit(temp = get_8bit_zp(zp_addr)-1);
  put_8bit_zp(zp_addr, temp);
  reg_pc+=2;
  cycles+=6;
  return 0;
}

int dex(void) {
  chk_flnz_8bit(--reg_x);
  reg_pc++;
  cycles+=2;
  return 0;
}

int dey(void) {
  chk_flnz_8bit(--reg_y);
  reg_pc++;
  cycles+=2;
  return 0;
}

int eor_abs(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp ^= abs_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=8;

  } else {
    chk_flnz_8bit(reg_a ^= abs_operand(reg_pc+1));
    cycles+=5;
  }
  reg_pc+=3;
  return 0;
}

int eor_absx(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp ^= absx_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=8;

  } else {
    chk_flnz_8bit(reg_a ^= absx_operand(reg_pc+1));
    cycles+=5;
  }
  reg_pc+=3;
  return 0;
}

int eor_absy(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp ^= absy_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=8;

  } else {
    chk_flnz_8bit(reg_a ^= absy_operand(reg_pc+1));
    cycles+=5;
  }
  reg_pc+=3;
  return 0;
}

int eor_imm(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp ^= imm_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=5;

  } else {
    chk_flnz_8bit(reg_a ^= imm_operand(reg_pc+1));
    cycles+=2;
  }
  reg_pc+=2;
  return 0;
}

int eor_zp(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp ^= zp_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=7;

  } else {
    chk_flnz_8bit(reg_a ^= zp_operand(reg_pc+1));
    cycles+=4;
  }
  reg_pc+=2;
  return 0;
}

int eor_zpx(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp ^= zpx_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=7;

  } else {
    chk_flnz_8bit(reg_a ^= zpx_operand(reg_pc+1));
    cycles+=4;
  }
  reg_pc+=2;
  return 0;
}

int eor_zpind(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp ^= zpind_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=10;

  } else {
    chk_flnz_8bit(reg_a ^= zpind_operand(reg_pc+1));
    cycles+=7;
  }
  reg_pc+=2;
  return 0;
}

int eor_zpindx(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp ^= zpindx_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=10;

  } else {
    chk_flnz_8bit(reg_a ^= zpindx_operand(reg_pc+1));
    cycles+=7;
  }
  reg_pc+=2;
  return 0;
}

int eor_zpindy(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp ^= zpindy_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=10;

  } else {
    chk_flnz_8bit(reg_a ^= zpindy_operand(reg_pc+1));
    cycles+=7;
  }
  reg_pc+=2;
  return 0;
}

int halt(void) {
  return(1);
}

int inc_a(void) {
  chk_flnz_8bit(++reg_a);
  reg_pc++;
  cycles+=2;
  return 0;
}

int inc_abs(void) {
  UChar  temp;
  UInt16 temp_addr = get_16bit_addr(reg_pc+1);
  chk_flnz_8bit(temp = get_8bit_addr(temp_addr)+1);
  cycles+=7;
  if (put_8bit_addr(temp_addr, temp)) {
    return 1;
  }
  reg_pc+=3;
  return 0;
}

int inc_absx(void) {
  UChar  temp;
  UInt16 temp_addr = get_16bit_addr(reg_pc+1)+reg_x;
  chk_flnz_8bit(temp = get_8bit_addr(temp_addr)+1);
  cycles+=7;
  if (put_8bit_addr(temp_addr, temp)) {
    return 1;
  }
  reg_pc+=3;
  return 0;
}

int inc_zp(void) {
  UChar temp;
  UChar zp_addr = imm_operand(reg_pc+1);
  chk_flnz_8bit(temp = get_8bit_zp(zp_addr)+1);
  put_8bit_zp(zp_addr, temp);
  reg_pc+=2;
  cycles+=6;
  return 0;
}

int inc_zpx(void) {
  UChar temp;
  UChar zp_addr = imm_operand(reg_pc+1)+reg_x;
  chk_flnz_8bit(temp = get_8bit_zp(zp_addr)+1);
  put_8bit_zp(zp_addr, temp);
  reg_pc+=2;
  cycles+=6;
  return 0;
}

int inx(void) {
  chk_flnz_8bit(++reg_x);
  reg_pc++;
  cycles+=2;
  return 0;
}

int iny(void) {
  chk_flnz_8bit(++reg_y);
  reg_pc++;
  cycles+=2;
  return 0;
}

int jmp(void) {
  reg_p &= ~FL_T;
  reg_pc = get_16bit_addr(reg_pc+1);
  cycles+=4;
  return 0;
}

int jmp_absind(void) {
  reg_p &= ~FL_T;
  reg_pc = get_16bit_addr(get_16bit_addr(reg_pc+1));
  cycles+=7;
  return 0;
}

int jmp_absindx(void) {
  reg_p &= ~FL_T;
  reg_pc = get_16bit_addr(get_16bit_addr(reg_pc+1)+reg_x);
  cycles+=7;
  return 0;
}

int jsr(void) {
  reg_p &= ~FL_T;
  push_16bit(reg_pc+2);
  reg_pc = get_16bit_addr(reg_pc+1);
  cycles+=7;
  return 0;
}

int lda_abs(void) {
  chk_flnz_8bit(reg_a = abs_operand(reg_pc+1));
  reg_pc+=3;
  cycles+=5;
  return 0;
}

int lda_absx(void) {
  chk_flnz_8bit(reg_a = absx_operand(reg_pc+1));
  reg_pc+=3;
  cycles+=5;
  return 0;
}

int lda_absy(void) {
  chk_flnz_8bit(reg_a = absy_operand(reg_pc+1));
  reg_pc+=3;
  cycles+=5;
  return 0;
}

int lda_imm(void) {
  chk_flnz_8bit(reg_a = imm_operand(reg_pc+1));
  reg_pc+=2;
  cycles+=2;
  return 0;
}

int lda_zp(void) {
  chk_flnz_8bit(reg_a = zp_operand(reg_pc+1));
  reg_pc+=2;
  cycles+=4;
  return 0;
}

int lda_zpx(void) {
  chk_flnz_8bit(reg_a = zpx_operand(reg_pc+1));
  reg_pc+=2;
  cycles+=4;
  return 0;
}

int lda_zpind(void) {
  chk_flnz_8bit(reg_a = zpind_operand(reg_pc+1));
  reg_pc+=2;
  cycles+=7;
  return 0;
}

int lda_zpindx(void) {
  chk_flnz_8bit(reg_a = zpindx_operand(reg_pc+1));
  reg_pc+=2;
  cycles+=7;
  return 0;
}

int lda_zpindy(void) {
  chk_flnz_8bit(reg_a = zpindy_operand(reg_pc+1));
  reg_pc+=2;
  cycles+=7;
  return 0;
}

int ldx_abs(void) {
  chk_flnz_8bit(reg_x = abs_operand(reg_pc+1));
  reg_pc+=3;
  cycles+=5;
  return 0;
}

int ldx_absy(void) {
  chk_flnz_8bit(reg_x = absy_operand(reg_pc+1));
  reg_pc+=3;
  cycles+=5;
  return 0;
}

int ldx_imm(void) {
  chk_flnz_8bit(reg_x = imm_operand(reg_pc+1));
  reg_pc+=2;
  cycles+=2;
  return 0;
}

int ldx_zp(void) {
  chk_flnz_8bit(reg_x = zp_operand(reg_pc+1));
  reg_pc+=2;
  cycles+=4;
  return 0;
}

int ldx_zpy(void) {
  chk_flnz_8bit(reg_x = zpy_operand(reg_pc+1));
  reg_pc+=2;
  cycles+=4;
  return 0;
}

int ldy_abs(void) {
  chk_flnz_8bit(reg_y = abs_operand(reg_pc+1));
  reg_pc+=3;
  cycles+=5;
  return 0;
}

int ldy_absx(void) {
  chk_flnz_8bit(reg_y = absx_operand(reg_pc+1));
  reg_pc+=3;
  cycles+=5;
  return 0;
}

int ldy_imm(void) {
  chk_flnz_8bit(reg_y = imm_operand(reg_pc+1));
  reg_pc+=2;
  cycles+=2;
  return 0;
}

int ldy_zp(void) {
  chk_flnz_8bit(reg_y = zp_operand(reg_pc+1));
  reg_pc+=2;
  cycles+=4;
  return 0;
}

int ldy_zpx(void) {
  chk_flnz_8bit(reg_y = zpx_operand(reg_pc+1));
  reg_pc+=2;
  cycles+=4;
  return 0;
}

int lsr_a(void) {
  UChar temp = reg_a;
  reg_a/=2;
  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((temp&1) ? FL_C:0)
	  | flnz_list[reg_a];
  reg_pc++;
  cycles+=2;
  return 0;
}

int lsr_abs(void) {
  UInt16 temp_addr = get_16bit_addr(reg_pc+1);
  UChar  temp1	   = get_8bit_addr(temp_addr);
  UChar  temp	   = temp1/2;

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((temp1&1) ? FL_C:0)
	  | flnz_list[temp];
  cycles+=7;
  if (put_8bit_addr(temp_addr, temp)) {
    return 1;
  }
  reg_pc+=3;
  return 0;
}

int lsr_absx(void) {
  UInt16 temp_addr = get_16bit_addr(reg_pc+1)+reg_x;
  UChar  temp1	   = get_8bit_addr(temp_addr);
  UChar  temp	   = temp1/2;

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((temp1&1) ? FL_C:0)
	  | flnz_list[temp];
  cycles+=7;
  if (put_8bit_addr(temp_addr, temp)) {
    return 1;
  }
  reg_pc+=3;
  return 0;
}

int lsr_zp(void) {
  UChar  zp_addr = imm_operand(reg_pc+1);
  UChar  temp1	 = get_8bit_zp(zp_addr);
  UChar  temp	 = temp1/2;

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((temp1&1) ? FL_C:0)
	  | flnz_list[temp];
  put_8bit_zp(zp_addr, temp);
  reg_pc+=2;
  cycles+=6;
  return 0;
}

int lsr_zpx(void) {
  UChar  zp_addr = imm_operand(reg_pc+1)+reg_x;
  UChar  temp1	 = get_8bit_zp(zp_addr);
  UChar  temp	 = temp1/2;

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((temp1&1) ? FL_C:0)
	  | flnz_list[temp];
  put_8bit_zp(zp_addr, temp);
  reg_pc+=2;
  cycles+=6;
  return 0;
}

int nop(void)  {
  reg_p &= ~FL_T;
  reg_pc++;
  cycles+=2;
  return 0;
}

int ora_abs(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp |= abs_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=8;

  } else {
    chk_flnz_8bit(reg_a |= abs_operand(reg_pc+1));
    cycles+=5;
  }
  reg_pc+=3;
  return 0;
}

int ora_absx(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp |= absx_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=8;

  } else {
    chk_flnz_8bit(reg_a |= absx_operand(reg_pc+1));
    cycles+=5;
  }
  reg_pc+=3;
  return 0;
}

int ora_absy(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp |= absy_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=8;

  } else {
    chk_flnz_8bit(reg_a |= absy_operand(reg_pc+1));
    cycles+=5;
  }
  reg_pc+=3;
  return 0;
}

int ora_imm(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp |= imm_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=5;

  } else {
    chk_flnz_8bit(reg_a |= imm_operand(reg_pc+1));
    cycles+=2;
  }
  reg_pc+=2;
  return 0;
}

int ora_zp(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp |= zp_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=7;

  } else {
    chk_flnz_8bit(reg_a |= zp_operand(reg_pc+1));
    cycles+=4;
  }
  reg_pc+=2;
  return 0;
}

int ora_zpx(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp |= zpx_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=7;

  } else {
    chk_flnz_8bit(reg_a |= zpx_operand(reg_pc+1));
    cycles+=4;
  }
  reg_pc+=2;
  return 0;
}

int ora_zpind(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp |= zpind_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=10;

  } else {
    chk_flnz_8bit(reg_a |= zpind_operand(reg_pc+1));
    cycles+=7;
  }
  reg_pc+=2;
  return 0;
}

int ora_zpindx(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp |= zpindx_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=10;

  } else {
    chk_flnz_8bit(reg_a |= zpindx_operand(reg_pc+1));
    cycles+=7;
  }
  reg_pc+=2;
  return 0;
}

int ora_zpindy(void) {
  if (reg_p & FL_T) {
    UChar temp = get_8bit_zp(reg_x);
    chk_flnz_8bit(temp |= zpindy_operand(reg_pc+1));
    put_8bit_zp(reg_x, temp);
    cycles+=10;

  } else {
    chk_flnz_8bit(reg_a |= zpindy_operand(reg_pc+1));
    cycles+=7;
  }
  reg_pc+=2;
  return 0;
}

int pha(void) {
  reg_p &= ~FL_T;
  push_8bit(reg_a);
  reg_pc++;
  cycles+=3;
  return 0;
}

int php(void) {
  reg_p &= ~FL_T;
  push_8bit(reg_p);
  reg_pc++;
  cycles+=3;
  return 0;
}

int phx(void) {
  reg_p &= ~FL_T;
  push_8bit(reg_x);
  reg_pc++;
  cycles+=3;
  return 0;
}

int phy(void) {
  reg_p &= ~FL_T;
  push_8bit(reg_y);
  reg_pc++;
  cycles+=3;
  return 0;
}

int pla(void) {
  chk_flnz_8bit(reg_a = pull_8bit());
  reg_pc++;
  cycles+=4;
  return 0;
}

int plp(void) {
  reg_p = pull_8bit();
  reg_pc++;
  cycles+=4;
  return 0;
}

int plx(void) {
  chk_flnz_8bit(reg_x = pull_8bit());
  reg_pc++;
  cycles+=4;
  return 0;
}

int ply(void) {
  chk_flnz_8bit(reg_y = pull_8bit());
  reg_pc++;
  cycles+=4;
  return 0;
}

int rmb0(void) {
  UChar temp = imm_operand(reg_pc+1);
  reg_p &= ~FL_T;
  put_8bit_zp(temp, get_8bit_zp(temp) & (~0x01));
  reg_pc+=2;
  cycles+=7;
  return 0;
}

int rmb1(void) {
  UChar temp = imm_operand(reg_pc+1);
  reg_p &= ~FL_T;
  put_8bit_zp(temp, get_8bit_zp(temp) & (~0x02));
  reg_pc+=2;
  cycles+=7;
  return 0;
}

int rmb2(void) {
  UChar temp = imm_operand(reg_pc+1);
  reg_p &= ~FL_T;
  put_8bit_zp(temp, get_8bit_zp(temp) & (~0x04));
  reg_pc+=2;
  cycles+=7;
  return 0;
}

int rmb3(void) {
  UChar temp = imm_operand(reg_pc+1);
  reg_p &= ~FL_T;
  put_8bit_zp(temp, get_8bit_zp(temp) & (~0x08));
  reg_pc+=2;
  cycles+=7;
  return 0;
}

int rmb4(void) {
  UChar temp = imm_operand(reg_pc+1);
  reg_p &= ~FL_T;
  put_8bit_zp(temp, get_8bit_zp(temp) & (~0x10));
  reg_pc+=2;
  cycles+=7;
  return 0;
}

int rmb5(void) {
  UChar temp = imm_operand(reg_pc+1);
  reg_p &= ~FL_T;
  put_8bit_zp(temp, get_8bit_zp(temp) & (~0x20));
  reg_pc+=2;
  cycles+=7;
  return 0;
}

int rmb6(void) {
  UChar temp = imm_operand(reg_pc+1);
  reg_p &= ~FL_T;
  put_8bit_zp(temp, get_8bit_zp(temp) & (~0x40));
  reg_pc+=2;
  cycles+=7;
  return 0;
}

int rmb7(void) {
  UChar temp = imm_operand(reg_pc+1);
  reg_p &= ~FL_T;
  put_8bit_zp(temp, get_8bit_zp(temp) & (~0x80));
  reg_pc+=2;
  cycles+=7;
  return 0;
}

int rol_a(void) {
  UChar flg_tmp = (reg_p & FL_C) ? 1:0;
  UChar temp = reg_a;

  reg_a = (reg_a<<1)+flg_tmp;
  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((temp & 0x80) ? FL_C:0)
	  | flnz_list[reg_a];
  reg_pc++;
  cycles+=2;
  return 0;
}

int rol_abs(void) {
  UChar  flg_tmp   = (reg_p & FL_C) ? 1:0;
  UInt16 temp_addr = get_16bit_addr(reg_pc+1);
  UChar  temp1	   = get_8bit_addr(temp_addr);
  UChar  temp	   = (temp1<<1)+flg_tmp;

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((temp1 & 0x80) ? FL_C:0)
	  | flnz_list[temp];
  cycles+=7;
  if (put_8bit_addr(temp_addr, temp)) {
    return 1;
  }
  reg_pc+=3;
  return 0;
}

int rol_absx(void) {
  UChar  flg_tmp   = (reg_p & FL_C) ? 1:0;
  UInt16 temp_addr = get_16bit_addr(reg_pc+1)+reg_x;
  UChar  temp1	   = get_8bit_addr(temp_addr);
  UChar  temp	   = (temp1<<1)+flg_tmp;

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((temp1 & 0x80) ? FL_C:0)
	  | flnz_list[temp];
  cycles+=7;
  if (put_8bit_addr(temp_addr, temp)) {
    return 1;
  }
  reg_pc+=3;
  return 0;
}

int rol_zp(void) {
  UChar  flg_tmp = (reg_p & FL_C) ? 1:0;
  UChar  zp_addr = imm_operand(reg_pc+1);
  UChar  temp1	 = get_8bit_zp(zp_addr);
  UChar  temp	 = (temp1<<1)+flg_tmp;

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((temp1 & 0x80) ? FL_C:0)
	  | flnz_list[temp];
  put_8bit_zp(zp_addr, temp);
  reg_pc+=2;
  cycles+=6;
  return 0;
}

int rol_zpx(void) {
  UChar  flg_tmp = (reg_p & FL_C) ? 1:0;
  UChar  zp_addr = imm_operand(reg_pc+1)+reg_x;
  UChar  temp1	 = get_8bit_zp(zp_addr);
  UChar  temp	 = (temp1<<1)+flg_tmp;

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((temp1 & 0x80) ? FL_C:0)
	  | flnz_list[temp];
  put_8bit_zp(zp_addr, temp);
  reg_pc+=2;
  cycles+=6;
  return 0;
}

int ror_a(void) {
  UChar flg_tmp = (reg_p & FL_C) ? 0x80:0;
  UChar temp	= reg_a;

  reg_a = (reg_a/2)+flg_tmp;
  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((temp & 0x01) ? FL_C:0)
	  | flnz_list[reg_a];
  reg_pc++;
  cycles+=2;
  return 0;
}

int ror_abs(void) {
  UChar  flg_tmp   = (reg_p & FL_C) ? 0x80:0;
  UInt16 temp_addr = get_16bit_addr(reg_pc+1);
  UChar  temp1	   = get_8bit_addr(temp_addr);
  UChar  temp	   = (temp1/2)+flg_tmp;

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((temp1 & 0x01) ? FL_C:0)
	  | flnz_list[temp];
  cycles+=7;
  if (put_8bit_addr(temp_addr, temp)) {
    return 1;
  }
  reg_pc+=3;
  return 0;
}

int ror_absx(void) {
  UChar  flg_tmp   = (reg_p & FL_C) ? 0x80:0;
  UInt16 temp_addr = get_16bit_addr(reg_pc+1)+reg_x;
  UChar  temp1	   = get_8bit_addr(temp_addr);
  UChar  temp	   = (temp1/2)+flg_tmp;

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((temp1 & 0x01) ? FL_C:0)
	  | flnz_list[temp];
  cycles+=7;
  if (put_8bit_addr(temp_addr, temp)) {
    return 1;
  }
  reg_pc+=3;
  return 0;
}

int ror_zp(void) {
  UChar  flg_tmp = (reg_p & FL_C) ? 0x80:0;
  UChar  zp_addr = imm_operand(reg_pc+1);
  UChar  temp1	 = get_8bit_zp(zp_addr);
  UChar  temp	 = (temp1/2)+flg_tmp;

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((temp1 & 0x01) ? FL_C:0)
	  | flnz_list[temp];
  put_8bit_zp(zp_addr, temp);
  reg_pc+=2;
  cycles+=6;
  return 0;
}

int ror_zpx(void) {
  UChar  flg_tmp = (reg_p & FL_C) ? 0x80:0;
  UChar  zp_addr = imm_operand(reg_pc+1)+reg_x;
  UChar  temp1	 = get_8bit_zp(zp_addr);
  UChar  temp	 = (temp1/2)+flg_tmp;

  reg_p = (reg_p & ~(FL_N|FL_T|FL_Z|FL_C))
	  | ((temp1 & 0x01) ? FL_C:0)
	  | flnz_list[temp];
  put_8bit_zp(zp_addr, temp);
  reg_pc+=2;
  cycles+=6;
  return 0;
}

int rti(void) {
  /* FL_B reset in RTI */
  reg_p = pull_8bit() & ~FL_B;
  reg_pc = pull_16bit();
  cycles+=7;
  return 0;
}

int rts(void) {
  reg_p &= ~FL_T;
  reg_pc = pull_16bit()+1;
  cycles+=7;
  return 0;
}

int sax(void) {
  UChar temp = reg_x;
  reg_p &= ~FL_T;
  reg_x = reg_a;
  reg_a = temp;
  reg_pc++;
  cycles+=3;
  return 0;
}

int say(void) {
  UChar temp = reg_y;
  reg_p &= ~FL_T;
  reg_y = reg_a;
  reg_a = temp;
  reg_pc++;
  cycles+=3;
  return 0;
}

int sbc_abs(void) {
  sbc(abs_operand(reg_pc+1));
  reg_pc+=3;
  cycles+=5;
  return 0;
}

int sbc_absx(void) {
  sbc(absx_operand(reg_pc+1));
  reg_pc+=3;
  cycles+=5;
  return 0;
}

int sbc_absy(void) {
  sbc(absy_operand(reg_pc+1));
  reg_pc+=3;
  cycles+=5;
  return 0;
}

int sbc_imm(void) {
  sbc(imm_operand(reg_pc+1));
  reg_pc+=2;
  cycles+=2;
  return 0;
}

int sbc_zp(void) {
  sbc(zp_operand(reg_pc+1));
  reg_pc+=2;
  cycles+=4;
  return 0;
}

int sbc_zpx(void) {
  sbc(zpx_operand(reg_pc+1));
  reg_pc+=2;
  cycles+=4;
  return 0;
}

int sbc_zpind(void) {
  sbc(zpind_operand(reg_pc+1));
  reg_pc+=2;
  cycles+=7;
  return 0;
}

int sbc_zpindx(void) {
  sbc(zpindx_operand(reg_pc+1));
  reg_pc+=2;
  cycles+=7;
  return 0;
}

int sbc_zpindy(void) {
  sbc(zpindy_operand(reg_pc+1));
  reg_pc+=2;
  cycles+=7;
  return 0;
}

int sec(void) {
  reg_p = (reg_p|FL_C) & ~FL_T;
  reg_pc++;
  cycles+=2;
  return 0;
}

int sed(void) {
  reg_p = (reg_p|FL_D) & ~FL_T;
  reg_pc++;
  cycles+=2;
  return 0;
}

int sei(void) {
  reg_p = (reg_p|FL_I) & ~FL_T;
  reg_pc++;
  cycles+=2;
  return 0;
}

int set(void) {
  reg_p |= FL_T;
  reg_pc++;
  cycles+=2;
  return 0;
}

int smb0(void) {
  UChar temp = imm_operand(reg_pc+1);
  reg_p &= ~FL_T;
  put_8bit_zp(temp, get_8bit_zp(temp) | 0x01);
  reg_pc+=2;
  cycles+=7;
  return 0;
}

int smb1(void) {
  UChar temp = imm_operand(reg_pc+1);
  reg_p &= ~FL_T;
  put_8bit_zp(temp, get_8bit_zp(temp) | 0x02);
  reg_pc+=2;
  cycles+=7;
  return 0;
}

int smb2(void) {
  UChar temp = imm_operand(reg_pc+1);
  reg_p &= ~FL_T;
  put_8bit_zp(temp, get_8bit_zp(temp) | 0x04);
  reg_pc+=2;
  cycles+=7;
  return 0;
}

int smb3(void) {
  UChar temp = imm_operand(reg_pc+1);
  reg_p &= ~FL_T;
  put_8bit_zp(temp, get_8bit_zp(temp) | 0x08);
  reg_pc+=2;
  cycles+=7;
  return 0;
}

int smb4(void) {
  UChar temp = imm_operand(reg_pc+1);
  reg_p &= ~FL_T;
  put_8bit_zp(temp, get_8bit_zp(temp) | 0x10);
  reg_pc+=2;
  cycles+=7;
  return 0;
}

int smb5(void) {
  UChar temp = imm_operand(reg_pc+1);
  reg_p &= ~FL_T;
  put_8bit_zp(temp, get_8bit_zp(temp) | 0x20);
  reg_pc+=2;
  cycles+=7;
  return 0;
}

int smb6(void) {
  UChar temp = imm_operand(reg_pc+1);
  reg_p &= ~FL_T;
  put_8bit_zp(temp, get_8bit_zp(temp) | 0x40);
  reg_pc+=2;
  cycles+=7;
  return 0;
}

int smb7(void) {
  UChar temp = imm_operand(reg_pc+1);
  reg_p &= ~FL_T;
  put_8bit_zp(temp, get_8bit_zp(temp) | 0x80);
  reg_pc+=2;
  cycles+=7;
  return 0;
}

int st0(void) {
  reg_p &= ~FL_T;
  IO_write(0,imm_operand(reg_pc+1));
  reg_pc+=2;
  cycles+=4;
  return 0;
}

int st1(void) {
  reg_p &= ~FL_T;
  IO_write(2,imm_operand(reg_pc+1));
  reg_pc+=2;
  cycles+=4;
  return 0;
}

int st2(void) {
  reg_p &= ~FL_T;
  IO_write(3,imm_operand(reg_pc+1));
  reg_pc+=2;
  cycles+=4;
  return 0;
}

int sta_abs(void) {
  reg_p &= ~FL_T;
  cycles+=5;
  if (put_8bit_addr(get_16bit_addr(reg_pc+1), reg_a)) {
    return 1;
  }
  reg_pc+=3;
  return 0;
}

int sta_absx(void) {
  reg_p &= ~FL_T;
  cycles+=5;
  if (put_8bit_addr(get_16bit_addr(reg_pc+1)+reg_x, reg_a)) {
    return 1;
  }
  reg_pc+=3;
  return 0;
}

int sta_absy(void) {
  reg_p &= ~FL_T;
  cycles+=5;
  if (put_8bit_addr(get_16bit_addr(reg_pc+1)+reg_y, reg_a)) {
    return 1;
  }
  reg_pc+=3;
  return 0;
}

int sta_zp(void) {
  reg_p &= ~FL_T;
  put_8bit_zp(imm_operand(reg_pc+1), reg_a);
  reg_pc+=2;
  cycles+=4;
  return 0;
}

int sta_zpx(void) {
  reg_p &= ~FL_T;
  put_8bit_zp(imm_operand(reg_pc+1)+reg_x, reg_a);
  reg_pc+=2;
  cycles+=4;
  return 0;
}

int sta_zpind(void) {
  reg_p &= ~FL_T;
  cycles+=7;
  if (put_8bit_addr(get_16bit_zp(imm_operand(reg_pc+1)), reg_a)) {
    return 1;
  }
  reg_pc+=2;
  return 0;
}

int sta_zpindx(void) {
  reg_p &= ~FL_T;
  cycles+=7;
  if (put_8bit_addr(get_16bit_zp(imm_operand(reg_pc+1)+reg_x), reg_a)) {
    return 1;
  }
  reg_pc+=2;
  return 0;
}

int sta_zpindy(void) {
  reg_p &= ~FL_T;
  cycles+=7;
  if (put_8bit_addr(get_16bit_zp(imm_operand(reg_pc+1))+reg_y, reg_a)) {
    return 1;
  }
  reg_pc+=2;
  return 0;
}

int stx_abs(void) {
  reg_p &= ~FL_T;
  cycles+=5;
  if (put_8bit_addr(get_16bit_addr(reg_pc+1), reg_x)) {
    return 1;
  }
  reg_pc+=3;
  return 0;
}

int stx_zp(void) {
  reg_p &= ~FL_T;
  put_8bit_zp(imm_operand(reg_pc+1), reg_x);
  reg_pc+=2;
  cycles+=4;
  return 0;
}

int stx_zpy(void) {
  reg_p &= ~FL_T;
  put_8bit_zp(imm_operand(reg_pc+1)+reg_y, reg_x);
  reg_pc+=2;
  cycles+=4;
  return 0;
}

int sty_abs(void) {
  reg_p &= ~FL_T;
  cycles+=5;
  if (put_8bit_addr(get_16bit_addr(reg_pc+1), reg_y)) {
    return 1;
  }
  reg_pc+=3;
  return 0;
}

int sty_zp(void) {
  reg_p &= ~FL_T;
  put_8bit_zp(imm_operand(reg_pc+1), reg_y);
  reg_pc+=2;
  cycles+=4;
  return 0;
}

int sty_zpx(void) {
  reg_p &= ~FL_T;
  put_8bit_zp(imm_operand(reg_pc+1)+reg_x, reg_y);
  reg_pc+=2;
  cycles+=4;
  return 0;
}

int stz_abs(void) {
  reg_p &= ~FL_T;
  cycles+=5;
  if (put_8bit_addr(get_16bit_addr(reg_pc+1), 0)) {
    return 1;
  }
  reg_pc+=3;
  return 0;
}

int stz_absx(void) {
  reg_p &= ~FL_T;
  cycles+=5;
  if (put_8bit_addr((get_16bit_addr(reg_pc+1)+reg_x), 0)) {
    return 1;
  }
  reg_pc+=3;
  return 0;
}

int stz_zp(void) {
  reg_p &= ~FL_T;
  put_8bit_zp(imm_operand(reg_pc+1), 0);
  reg_pc+=2;
  cycles+=4;
  return 0;
}

int stz_zpx(void) {
  reg_p &= ~FL_T;
  put_8bit_zp(imm_operand(reg_pc+1)+reg_x, 0);
  reg_pc+=2;
  cycles+=4;
  return 0;
}

int sxy(void) {
  UChar temp = reg_y;
  reg_p &= ~FL_T;
  reg_y = reg_x;
  reg_x = temp;
  reg_pc++;
  cycles+=3;
  return 0;
}

int tai(void) {
  UInt16 from, to, len, alternate;
  int err = 0;

  reg_p &= ~FL_T;
  from = get_16bit_addr(reg_pc+1);
  to   = get_16bit_addr(reg_pc+3);
  len  = get_16bit_addr(reg_pc+5);
  alternate = 0;

  cycles+=(6 * len) + 17;
  while ((len-- != 0) && (err == 0)) {
    err = put_8bit_addr(to++, get_8bit_addr(from+alternate));
    alternate ^= 1;
  }
  if (!err) reg_pc+=7;
  return(err);
}

int tam(void) {
  UInt16 i;
  UChar bitfld = imm_operand(reg_pc+1);

  for (i = 0; i < 8; i++) {
    if (bitfld & (1 << i)) {
      mmr[i] = reg_a;
      bank_set(i, reg_a);
      /* Page[i] = ROMMap[reg_a]; */
    }
  }
  reg_p &= ~FL_T;
  reg_pc+=2;
  cycles+=5;
  return 0;
}

int tax(void) {
  chk_flnz_8bit(reg_x = reg_a);
  reg_pc++;
  cycles+=2;
  return 0;
}

int tay(void) {
  chk_flnz_8bit(reg_y = reg_a);
  reg_pc++;
  cycles+=2;
  return 0;
}

int tdd(void) {
  UInt16 from, to, len;
  int err = 0;

  reg_p &= ~FL_T;
  from = get_16bit_addr(reg_pc+1);
  to   = get_16bit_addr(reg_pc+3);
  len  = get_16bit_addr(reg_pc+5);

  cycles+=(6 * len) + 17;
  while ((len-- != 0) && (err == 0)) {
    err = put_8bit_addr(to--, get_8bit_addr(from--));
  }
  if (!err) reg_pc+=7;
  return(err);
}

int tia(void) {
  UInt16 from, to, len, alternate;
  int err = 0;

  reg_p &= ~FL_T;
  from = get_16bit_addr(reg_pc+1);
  to   = get_16bit_addr(reg_pc+3);
  len  = get_16bit_addr(reg_pc+5);
  alternate = 0;

  cycles+=(6 * len) + 17;
  while ((len-- != 0) && (err == 0)) {
    err = put_8bit_addr(to+alternate, get_8bit_addr(from++));
    alternate ^= 1;
  }
  if (!err) reg_pc+=7;
  return(err);
}

int tii(void) {
  UInt16 from, to, len;
  int err = 0;

  reg_p &= ~FL_T;
  from = get_16bit_addr(reg_pc+1);
  to   = get_16bit_addr(reg_pc+3);
  len  = get_16bit_addr(reg_pc+5);

  cycles+=(6 * len) + 17;
  while ((len-- != 0) && (err == 0)) {
    err = put_8bit_addr(to++, get_8bit_addr(from++));
  }
  if (!err) reg_pc+=7;
  return(err);
}

int tin(void) {
  UInt16 from, to, len;
  int err = 0;

  reg_p &= ~FL_T;
  from = get_16bit_addr(reg_pc+1);
  to   = get_16bit_addr(reg_pc+3);
  len  = get_16bit_addr(reg_pc+5);

  cycles+=(6 * len) + 17;
  while ((len-- != 0) && (err == 0)) {
    err = put_8bit_addr(to, get_8bit_addr(from++));
  }
  if (!err) reg_pc+=7;
  return(err);
}

int tma(void) {
  int i;
  UChar bitfld = imm_operand(reg_pc+1);

  for (i = 0; i < 8; i++) {
    if (bitfld & (1 << i)) {
      reg_a = mmr[i];
    }
  }
  reg_p &= ~FL_T;
  reg_pc+=2;
  cycles+=4;
  return 0;
}

int trb_abs(void) {
  UInt16 abs_addr = get_16bit_addr(reg_pc+1);
  UChar  temp	  = get_8bit_addr(abs_addr);
  UChar  temp1	  = (~reg_a) & temp;

  reg_p = (reg_p & ~(FL_N|FL_V|FL_T|FL_Z))
	  | ((temp1&0x80) ? FL_N:0)
	  | ((temp1&0x40) ? FL_V:0)
	  | ((temp & reg_a) ? 0:FL_Z);
  cycles+=7;
  if (put_8bit_addr(abs_addr, temp1)) {
    return 1;
  }
  reg_pc+=3;
  return 0;
}

int trb_zp(void) {
  UChar zp_addr  = imm_operand(reg_pc+1);
  UChar temp	 = get_8bit_zp(zp_addr);
  UChar temp1	 = (~reg_a) & temp;

  reg_p = (reg_p & ~(FL_N|FL_V|FL_T|FL_Z))
	  | ((temp1&0x80) ? FL_N:0)
	  | ((temp1&0x40) ? FL_V:0)
	  | ((temp & reg_a) ? 0:FL_Z);
  put_8bit_zp(zp_addr, temp1);
  reg_pc+=2;
  cycles+=6;
  return 0;
}

int tsb_abs(void) {
  UInt16 abs_addr = get_16bit_addr(reg_pc+1);
  UChar  temp	  = get_8bit_addr(abs_addr);
  UChar  temp1	  = reg_a | temp;

  reg_p = (reg_p & ~(FL_N|FL_V|FL_T|FL_Z))
	  | ((temp1&0x80) ? FL_N:0)
	  | ((temp1&0x40) ? FL_V:0)
	  | ((temp & reg_a) ? 0:FL_Z);
  cycles+=7;
  if (put_8bit_addr(abs_addr, temp1)) {
    return 1;
  }
  reg_pc+=3;
  return 0;
}

int tsb_zp(void) {
  UChar zp_addr  = imm_operand(reg_pc+1);
  UChar temp	 = get_8bit_zp(zp_addr);
  UChar temp1	 = reg_a | temp;

  reg_p = (reg_p & ~(FL_N|FL_V|FL_T|FL_Z))
	  | ((temp1&0x80) ? FL_N:0)
	  | ((temp1&0x40) ? FL_V:0)
	  | ((temp & reg_a) ? 0:FL_Z);
  put_8bit_zp(zp_addr, temp1);
  reg_pc+=2;
  cycles+=6;
  return 0;
}

int tstins_abs(void) {
  UChar  imm	  = imm_operand(reg_pc+1);
  UChar  temp	  = abs_operand(reg_pc+2);

  reg_p = (reg_p & ~(FL_N|FL_V|FL_T|FL_Z))
	  | ((temp&0x80) ? FL_N:0)
	  | ((temp&0x40) ? FL_V:0)
	  | ((temp&imm)  ? 0:FL_Z);
  cycles+=8;
  reg_pc+=4;
  return 0;
}

int tstins_absx(void) {
  UChar  imm	  = imm_operand(reg_pc+1);
  UChar  temp	  = absx_operand(reg_pc+2);

  reg_p = (reg_p & ~(FL_N|FL_V|FL_T|FL_Z))
	  | ((temp&0x80) ? FL_N:0)
	  | ((temp&0x40) ? FL_V:0)
	  | ((temp&imm)  ? 0:FL_Z);
  cycles+=8;
  reg_pc+=4;
  return 0;
}

int tstins_zp(void) {
  UChar imm	= imm_operand(reg_pc+1);
  UChar temp	= zp_operand(reg_pc+2);

  reg_p = (reg_p & ~(FL_N|FL_V|FL_T|FL_Z))
	  | ((temp&0x80) ? FL_N:0)
	  | ((temp&0x40) ? FL_V:0)
	  | ((temp&imm)  ? 0:FL_Z);
  cycles+=7;
  reg_pc+=3;
  return 0;
}

int tstins_zpx(void) {
  UChar imm	= imm_operand(reg_pc+1);
  UChar temp	= zpx_operand(reg_pc+2);

  reg_p = (reg_p & ~(FL_N|FL_V|FL_T|FL_Z))
	  | ((temp&0x80) ? FL_N:0)
	  | ((temp&0x40) ? FL_V:0)
	  | ((temp&imm)  ? 0:FL_Z);
  cycles+=7;
  reg_pc+=3;
  return 0;
}


int tsx(void) {
  chk_flnz_8bit(reg_x = reg_s);
  reg_pc++;
  cycles+=2;
  return 0;
}

int txa(void) {
  chk_flnz_8bit(reg_a = reg_x);
  reg_pc++;
  cycles+=2;
  return 0;
}

int txs(void) {
  reg_p &= ~FL_T;
  reg_s = reg_x;
  reg_pc++;
  cycles+=2;
  return 0;
}

int tya(void) {
  chk_flnz_8bit(reg_a = reg_y);
  reg_pc++;
  cycles+=2;
  return 0;
}

// perform machine operations for IRQ2:

int int_irq2 (void) {
  if ((io.irq_mask & FL_IRQ2) != 0) {   // interrupt disabled
     return 0;
  }
//if ((irq_register & FL_IRQ2) == 0) {	 // interrupt disabled ?
//   return 0;
//}
  cycles+=7;
  push_16bit(reg_pc);
  push_8bit(reg_p);
  reg_p = (reg_p & ~FL_D) | FL_I;
  reg_pc = get_16bit_addr(0xFFF6);
  return 0;
}

// perform machine operations for IRQ1 (video interrupt):

int int_irq1 (void) {
  if ((io.irq_mask & FL_IRQ1) != 0) {   // interrupt disabled
     return 0;
  }
//if ((irq_register & FL_IRQ1) == 0) {	 // interrupt disabled ?
//   return 0;
//}
  cycles+=7;
  push_16bit(reg_pc);
  push_8bit(reg_p);
  reg_p = (reg_p & ~FL_D) | FL_I;
  reg_pc = get_16bit_addr(0xFFF8);
  return 0;
}

// perform machine operations for timer interrupt:

int int_tiq(void) {
  if ((io.irq_mask & FL_TIQ) != 0) {    // interrupt disabled
     return 0;
  }
//if ((irq_register & FL_TIQ) == 0) {	// interrupt disabled ?
//   return 0;
//}
  cycles+=7;
  push_16bit(reg_pc);
  push_8bit(reg_p);
  reg_p = (reg_p & ~FL_D) | FL_I;
  reg_pc = get_16bit_addr(0xFFFA);
  return 0;
}

// Execute a single instruction :

void exe_instruct(void) {
  (*optable[Page[reg_pc>>13][reg_pc]].func_exe)();
}

void
Int6502 (UChar Type)
{
  UInt16 J;

  if ((Type == INT_NMI) || (!(reg_p & FL_I)))
    {
      cycles += 7;
      push_16bit(reg_pc);
      push_8bit(reg_p);
      reg_p = (reg_p & ~FL_D);
/*
      WrRAM (SP + R->S,
	     ((R->P & ~(B_FLAG | T_FLAG)) & ~(N_FLAG | V_FLAG | Z_FLAG)) |
	     (R->NF & N_FLAG) | (R->VF & V_FLAG) | (R->ZF ? 0 : Z_FLAG));
*/

      if (Type == INT_NMI)
	{
	  J = VEC_NMI;
	}
      else
	{
          reg_p |=  FL_I;

	  switch (Type)
	    {

	    case INT_IRQ:
	      J = VEC_IRQ;
	      break;

	    case INT_IRQ2:
	      J = VEC_IRQ2;
	      break;

	    case INT_TIMER:
	      J = VEC_TIMER;
	      break;


	    }

	}

      reg_pc = get_16bit_addr(J);
    }
  else
    {
      irequest |= Type;
    }

}


// Execute instructions as a machine would, including all
// important (known) interrupts, hardware functions, and
// actual video display on the hardware
//
// Until the following happens:
// (1) An unknown instruction is to be executed
// (2) An unknown hardware access is performed
// (3) <ESC> key is hit

void exe_go(void) {
  int err = 0;
  UChar I;

//  if (gr_mode != 0) set_gr_palette();

  // err is set on a 'trap':
  while (!err) {

//    Log("Pc = %04x : %s\n", reg_pc, optable[Page[reg_pc>>13][reg_pc]].opname);

#ifdef DEBUG_KERNEL_DS
/*
      if (reg_pc<0xE000)
      {
      Log("PC = %04X (%02X), A = %02X, X = %02X, Y = %02X, P = %02X\n",
          reg_pc,
          Page[reg_pc>>13][reg_pc],
          reg_a,
          reg_x,
          reg_y,
          reg_p);
/*
      {
        int i;
        for (i=0xF0; i<256; i++)
          Log("%02X",get_8bit_zp(i));
        Log("\n");
      }

      }
*/
#endif


    err = (*optable[Page[reg_pc>>13][reg_pc]].func_exe)();
    // err = (*optable[Page[reg_pc>>13][reg_pc]].func_exe)();

    // TIMER stuff:
//  if (tiq_flag) {
//     tiq_flag = 0;
//     if (!(reg_p & FL_I)) int_tiq();
//  }

    // HSYNC stuff - count cycles:
    if (cycles > 455) {

/*
      Log("Horizontal sync, cycles = %d, cycleNew = %d\n",
          cycles,
          CycleNew);
*/
      CycleNew += cycles;
      cycles -= 455;
      // scanline++;

      /* inspired from Marat's code ... */

	  /* If we have come after CLI, get INT_? from IRequest */
	  /* Otherwise, get it from the loop handler            */
	  if (aftercli)
	    {

	      if (irequest & INT_TIMER)
		{

		  irequest &= ~INT_TIMER;

		  I = INT_TIMER;

		}
	      else if (irequest & INT_IRQ)
		{

		  irequest &= ~INT_IRQ;

		  I = INT_IRQ;

		}

	      else if (irequest & INT_IRQ2)
		{

		  irequest &= ~INT_IRQ2;

		  I = INT_IRQ2;

		}

	      cycles = 455;

	      if (irequest == 0)

		{
		  cycles = ibackup;	/* Restore the ICount        */
		  aftercli = 0;	/* Done with AfterCLI state  */

		}
	    }
	  else
	    {

              // Log("Calling periodic handler\n");

	      I = Loop6502 ();	/* Call the periodic handler */
	      // _ICount += _IPeriod;	/* Reset the cycle counter   */
              cycles = 0;
	    }

          // Log("Requested interrupt is %d\n",I);

	  if (I == INT_QUIT)
	    return;	/* Exit if INT_QUIT     */
	  if (I)
	    Int6502 (I);	/* Interrupt if needed  */


	  if ((unsigned int) (CycleNew - cyclecountold) >
	      (unsigned int) TimerPeriod * 2)

	    cyclecountold = CycleNew;

	}
      else
	{

	  if (CycleNew - cyclecountold >= TimerPeriod)
	    {

	      cyclecountold += TimerPeriod;

	      I = TimerInt ();

	      if (I)
		Int6502 (I);

	    }

	}


/* Dave's code ....
      if (scanline < 241) {			    // body of screen
//	 if ((frames & 3) == 1) put_CG(lines-1);    // calculate scanline
         // put_CG(scanline-1);
	 io.vdc_status = 0x04;			    // HSYNC
//	 if (HSync_Int) {
         if (RasHitON) {
	    if (!(reg_p & FL_I)) int_irq1();
	 }

      } else if ((scanline > 240) && (scanline < 252)) {  // vsync pulse
	 io.vdc_status = 0x04;			    // HSYNC
//	 if (HSync_Int) {
         if (RasHitON) {
	    if (!(reg_p & FL_I)) int_irq1();
	 }

      } else if ((scanline > 251) && (scanline < 255)) {  // vsync pulse
//	 stat_6270 = 0x24;			    // VSYNC *and* HSYNC
//	 if (lines == 252)  {
	    io.vdc_status = 0x20;
	    if (!(reg_p & FL_I)) int_irq1();
//	 } else if (HSync_Int) {
//	    if (!(reg_p & FL_I)) int_irq1();
//	 }

      } else if (scanline < 257) { 		    // vblank interval
	 io.vdc_status = 0x04;			    // VSYNC
//	 if (HSync_Int) {
         if (RasHitON) {
	    if (!(reg_p & FL_I)) int_irq1();
	 }

      } else {					    // (line 260) restart
	 scanline = 0;
	 frames++;
         if (osd_keyboard())
            return;
//	 if ( ((frames & 1) == 1) && (gr_mode != 0) ) {
	 if ((frames & 1) == 1) {
//	 if (gr_mode != 0) {
//	    if (pal_flag == 1) set_gr_palette();
//	    put_graphics();
//            put_sprites();
            RefreshScreen ();
//	    blit_to_screen();
	 }
//	 if (key_array[1] != 0) err = 1;
//	 if (kbhit()) err = 1;
	 if (halt_flag) err = 1;
	 io.vdc_status = 0x04;
	 if (RasHitON) {
	    if (!(reg_p & FL_I)) int_irq1();
	 }
      }
    }

    */
}
//exit_pctimerint();     // release PC's timer interrupt
//exit_pckeyint();       // release PC's keyboard-read interrupt
}



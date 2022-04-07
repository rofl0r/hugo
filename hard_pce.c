/***************************************************************************/
/*                                                                         */
/*                         HARDware PCEngine                               */
/*                                                                         */
/* This source file implements all functions relatives to pc engine inner  */
/* hardware (memory access e.g.)                                           */
/*                                                                         */
/***************************************************************************/

#include "hard_pce.h"

/**
  * Variables declaration
  * cf explanations in the header file
  **/

UChar RAM[0x8000];
UInt16 SPRAM[64 * 4];
UInt32 *VRAM2;
UInt32 *VRAMS;
UChar Pal[512];
UChar *vchange;
UChar *vchanges;
UChar *WRAM;
UChar *VRAM;

IO io;

UChar cd_port_1800 = 0; /**/
UChar cd_port_1801 = 0; /**/
UChar cd_port_1802 = 0; /**/
UChar cd_port_1804 = 0; /**/

UChar *cd_read_buffer;
UChar *cd_sector_buffer;
UChar *cd_extra_mem;
UChar *cd_extra_super_mem;
UChar *ac_extra_mem;

UInt32 pce_cd_read_datacnt; /**/
UChar cd_sectorcnt;
UChar pce_cd_curcmd; /**/

UChar* zp_base;
UChar* sp_base;
UChar mmr[8];

UChar irequest;
UChar aftercli;
UInt32 cyclecount;
UInt32 cyclecountold;
UInt32 ibackup;

const UInt32 TimerPeriod = 1097;

// registers

UInt16 reg_pc;
UChar  reg_a;
UChar  reg_x;
UChar  reg_y;
UChar  reg_p;
UChar  reg_s;

UChar  halt_flag=0;
long   cycles=0;
long   frames=0;

/**
  * Functions to access PCE hardware
  **/

/* read */
UChar IO_read (UInt16 A)
{
  UChar ret;

/*  Replaced below...
#ifndef FINAL_RELEASE
  if (((A & 0x1FFF) > 0x1A00) && ((A & 0x1FFF) < 0x1B00))
    Log("AC Read at %04x\n", A);
#endif
*/

#ifndef FINAL_RELEASE
  if ((A & 0x1F00) == 0x1A00)
    Log("AC Read at %04x\n", A);
#endif


  /* BEWARE */
  // switch(A&0x1C00){ // safe way but not for specials ports
  /* BE CAREFUL, 0x1C00 is safe */

  switch (A & 0x1FC0)
    {				// MAY BE UNSAFE
    case 0x0000:		/* VDC */
      switch (A & 3)
	{
	case 0:
	  ret = io.vdc_status;
	  io.vdc_status = 0;	//&=VDC_InVBlank;//&=~VDC_BSY;
	  return ret;
	case 1:
	  return 0;
	case 2:
	  if (io.vdc_reg == VRR)
	    return VRAM[io.VDC[MARR].W * 2];
	  else
	    return io.VDC[io.vdc_reg].B.l;
	case 3:
	  if (io.vdc_reg == VRR)
	    {
	      ret = VRAM[io.VDC[MARR].W * 2 + 1];
	      io.VDC[MARR].W += io.vdc_inc;
	      return ret;
	    }
	  else
	    return io.VDC[io.vdc_reg].B.h;
	}
      break;

    case 0x0400:		/* VCE */
      switch (A & 7)
	{
	case 4:
	  return io.VCE[io.vce_reg.W].B.l;
	case 5:
	  return io.VCE[io.vce_reg.W++].B.h;
	}
      break;
    case 0x0800:		/* PSG */
      switch (A & 15)
	{
	case 0:
	  return io.psg_ch;
	case 1:
	  return io.psg_volume;
	case 2:
	  return io.PSG[io.psg_ch][2];
	case 3:
	  return io.PSG[io.psg_ch][3];
	case 4:
	  return io.PSG[io.psg_ch][4];
	case 5:
	  return io.PSG[io.psg_ch][5];
	case 6:
	  {
	    int ofs = io.wavofs[io.psg_ch];
	    io.wavofs[io.psg_ch] = (io.wavofs[io.psg_ch] + 1) & 31;
	    return io.wave[io.psg_ch][ofs];

#ifdef USE_MORE_SOUND_VAR
	    int ofs = fixtoi (snd_pos[io.psg_ch]);
	    snd_pos[io.psg_ch] =
	      itofix ((fixtoi (snd_pos[io.psg_ch]) + 1) & 31);
	    return io.wave[io.psg_ch][ofs];
#endif

	  }
	case 7:
	  return io.PSG[io.psg_ch][7];
	case 8:
	  return io.psg_lfo_freq;
	case 9:
	  return io.psg_lfo_ctrl;
	default:
	  return NODATA;
	}
      break;
    case 0x0c00:		/* timer */
      return io.timer_counter;

    case 0x1000:		/* joypad */
      ret = io.JOY[io.joy_counter] ^ 0xff;
      if (io.joy_select & 1)
	ret >>= 4;
      else
	{
	  ret &= 15;
	  io.joy_counter = (io.joy_counter + 1) % 5;
	}

      /* return ret | Country; */	/* country 0:JPN 1<<6=US */
      return ret;

    case 0x1400:		/* IRQ */
      switch (A & 15)
	{
	case 2:
	  return io.irq_mask;
	case 3:
	  ret = io.irq_status;
	  io.irq_status = 0;
	  return ret;
	}
      break;


    case 0x18C0:		// Memory management ?
      switch (A & 15)
	{
	case 5:
	case 1:
	  return 0xAA;
	case 2:
	case 6:
	  return 0x55;
	case 3:
	case 7:
	  return 0x03;
	}
      break;
      
    case 0x1AC0:
     switch (A & 15)
     {
            case 0:
                 return (UChar)(io.ac_shift);
            case 1:
                 return (UChar)(io.ac_shift >> 8);
            case 2:
                 return (UChar)(io.ac_shift >> 16);
            case 3:
                 return (UChar)(io.ac_shift >> 24);
            case 4:
                 return io.ac_shiftbits;
            case 5:
                 return io.ac_unknown4;
            case 14:
              return 0x10;
            case 15:
              return 0x51;
            default:
              Log("Unknown Arcade card port access : 0x%04X\n", A);
     }
     break;
      
    case 0x1A00:
    {
     UChar ac_port = (A >> 4) & 3;
     switch (A & 15)
     {
      case 0:
      case 1:
	/*
           switch (io.ac_control[ac_port] & (AC_USE_OFFSET | AC_USE_BASE))
             {
              case 0:
                   return ac_extra_mem[0];
              case AC_USE_OFFSET:
                   ret = ac_extra_mem[io.ac_offset[ac_port]];
                   if (!(io.ac_control[ac_port] & AC_INCREMENT_BASE))
                     io.ac_offset[ac_port]+=io.ac_incr[ac_port];
                   return ret;
              case AC_USE_BASE:
                   ret = ac_extra_mem[io.ac_base[ac_port]];
                   if (io.ac_control[ac_port] & AC_INCREMENT_BASE)
                     io.ac_base[ac_port]+=io.ac_incr[ac_port];
                   return ret;
              default:
                   ret = ac_extra_mem[io.ac_base[ac_port] + io.ac_offset[ac_port]];
                   if (io.ac_control[ac_port] & AC_INCREMENT_BASE)
                     io.ac_base[ac_port]+=io.ac_incr[ac_port];
                   else
                     io.ac_offset[ac_port]+=io.ac_incr[ac_port];
                   return ret;
              }
           return 0;
*/
           if (io.ac_control[ac_port] & AC_USE_OFFSET)
              ret = ac_extra_mem[((io.ac_base[ac_port] + io.ac_offset[ac_port]) & 0x1fffff)];
           else
              ret = ac_extra_mem[((io.ac_base[ac_port]) & 0x1fffff)];

           if (io.ac_control[ac_port] & AC_ENABLE_INC)
           {
             if (io.ac_control[ac_port] & AC_INCREMENT_BASE)
               io.ac_base[ac_port] = (io.ac_base[ac_port] + io.ac_incr[ac_port]) & 0xffffff;
             else
               io.ac_offset[ac_port] = (io.ac_offset[ac_port] + io.ac_incr[ac_port]) & 0xffff;
           }

           return ret;


      case 2:
           return (UChar)(io.ac_base[ac_port]);
      case 3:
           return (UChar)(io.ac_base[ac_port] >> 8);
      case 4:
           return (UChar)(io.ac_base[ac_port] >> 16);
      case 5:
           return (UChar)(io.ac_offset[ac_port]);
      case 6:
           return (UChar)(io.ac_offset[ac_port] >> 8);
      case 7:
           return (UChar)(io.ac_incr[ac_port]);
      case 8:
           return (UChar)(io.ac_incr[ac_port] >> 8);
      case 9:
           return io.ac_control[ac_port];
      default:
           Log("Unknown Arcade card port access : 0x%04X\n", A);
     }
     break;
    }
    case 0x1800:		// CD-ROM extention
      switch (A & 15)
	{
	case 0:
	  return cd_port_1800;
	case 1:
	  {
	    UChar retval;

	    if (cd_read_buffer)
	      {
		retval = *cd_read_buffer++;
		if (pce_cd_read_datacnt == 2048)
		  {
		    pce_cd_read_datacnt--;

#ifndef FINAL_RELEASE
		    fprintf (stderr, "Data count fudge\n");
#endif

		  }
		if (!pce_cd_read_datacnt)
		  cd_read_buffer = 0;
	      }
	    else
	      retval = 0;
	    return retval;
	  }

	case 2:
	  return cd_port_1802;

	case 3:

	  {

	    static UChar tmp_res = 0x02;

	    tmp_res = 0x02 - tmp_res;

	    io.backup = DISABLE;

/* TEST */// return 0x20;

	    return tmp_res | 0x20;

	  }

/* TEST */
	case 4:
	  return cd_port_1804;

/* TEST */
	case 5:
	  return 0x50;

/* TEST */
	case 6:
	  return 0x05;

	case 0x0A:
#ifndef FINAL_RELEASE
	  Log ("HARD : Read %x from ADPCM[%04X] to VRAM : 0X%04X\n",
	       PCM[io.adpcm_rptr], io.adpcm_rptr, io.VDC[MAWR].W * 2);
#endif

	  if (!io.adpcm_firstread)
	    return PCM[io.adpcm_rptr++];
	  else
	    {
	      io.adpcm_firstread--;
	      return NODATA;
	    }

	case 0x0B:		/* TEST */
	  return 0x00;
	case 0x0C:		/* TEST */
	  return 0x01;		// 0x89
	case 0x0D:		/* TEST */
	  return 0x00;

	case 8:
	  if (pce_cd_read_datacnt)
	    {
	      UChar retval;

	      if (cd_read_buffer)
                retval = *cd_read_buffer++;
	      else
		retval = 0;

	      if (!--pce_cd_read_datacnt)
		{
		  cd_read_buffer = 0;
		  if (!--cd_sectorcnt)
		    {
#ifndef FINAL_RELEASE
		      fprintf (stderr, "Sector data count over.\n");
#endif
		      cd_port_1800 |= 0x10;
		      pce_cd_curcmd = 0;
		    }
		  else
		    {
#ifndef FINAL_RELEASE
		      fprintf (stderr, "Sector data count %d.\n",
			       cd_sectorcnt);
#endif
		      pce_cd_read_sector ();
		    }
		}
	      return retval;
	    }
	  break;
	}
    }
#ifndef FINAL_RELEASE
  fprintf (stderr, "ignore I/O read %04X\nat PC = %04X\n", A, M.PC.W);
#endif
  return NODATA;
}


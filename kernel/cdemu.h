/**************************************************************************/
/*                                                                        */
/*                        CD Emulation source file                        */
/*                                                                        */
/*    This file is intended to be included in the case which handle the   */
/* execution of the opcodes, when occur a 0xFC opcode, we must either     */
/* restoring the jump thanks to the data situated just after the invalid  */
/* opcode or make the function by ourself, returning with a 'RTS' with a  */
/* value in A, 0 means success and other value for an error               */
/*                                                                        */
/*     Code source by Zeograd, tell me if you make any use of this        */
/*              Help really wanted on all of this                         */
/*                                                                        */
/**************************************************************************/

/* CD related functions */

#define CD_BOOT   0x00
#define CD_RESET  0x01
#define CD_BASE   0x02
#define CD_READ   0x03
#define CD_SEEK   0x04
#define CD_EXEC   0x05
#define CD_PLAY   0x06
#define CD_SEARCH 0x07
#define CD_PAUSE  0x08
#define CD_STAT   0x09
#define CD_SUBA   0x0A
#define CD_DINFO  0x0B
#define CD_CONTENTS 0x0C
#define CD_SUBRQ  0x0D
#define CD_PCMRD  0x0E
#define CD_FADE   0x0F

/* ADPCM related functions */

#define AD_RESET  0x10
#define AD_TRANS  0x11
#define AD_READ   0x12
#define AD_WRITE  0x13
#define AD_PLAY   0x14
#define AD_CPLAY  0x15
#define AD_STOP   0x16
#define AD_STAT   0x17

/* BACKUP MEM related functions */

#define BM_FORMAT 0x18
#define BM_FREE   0x19
#define BM_READ   0x1A
#define BM_WRITE  0x1B
#define BM_DELETE 0x1C
#define BM_FILES  0x1D

/* Miscelanous functions */

#define EX_GETVER 0x1E
#define EX_SETVEC 0x1F
#define EX_GETFNT 0x20
#define EX_JOYSNS 0x21
#define EX_JOYREP 0x22
#define EX_SCRSIZ 0x23
#define EX_DOTMOD 0x24
#define EX_SCRMOD 0x25
#define EX_IMODE  0x26
#define EX_VMOD   0x27
#define EX_HMOD   0x28
#define EX_VSYNC  0x29
#define EX_RCRON  0x2A
#define EX_RCOFF  0x2B
#define EX_IRQON  0x2C
#define EX_IRQOFF 0x2D
#define EX_BGON   0x2E
#define EX_BGOFF  0x2F
#define EX_SPRON  0x30
#define EX_SPROFF 0x31
#define EX_DSPON  0x32
#define EX_DPSOFF 0x33
#define EX_DMAMOD 0x34
#define EX_SPRDMA 0x35
#define EX_SATCLR 0x36
#define EX_SPRPUT 0x37
#define EX_SETRCR 0x38
#define EX_SETRED 0x39
#define EX_SETWRT 0x3A
#define EX_SETDMA 0x3B
#define EX_BINBCD 0x3C
#define EX_BCDBIN 0x3D
#define EX_RND    0x3E

/* Math related functions */

#define MA_MUL8U  0x3F
#define MA_MUL8S  0x40
#define MA_MU16U  0x41
#define MA_DIV16S 0x42
#define MA_DIV16U 0x43
#define MA_SQRT   0x44
#define MA_SIN    0x45
#define MA_COS    0x46
#define MA_ATNI   0x47

/* PSG BIOS functions */

#define PSG_BIOS  0x48
#define GRP_BIOS  0x49
#define KEY_BIOS  0x4A
#define PSG_DRIVE 0x4B
#define EX_COLORC 0x4C

/* 'Register' defines */

#define _al 0x20F8
#define _ah 0x20F9
#define _bl 0x20FA
#define _bh 0x20FB
#define _cl 0x20FC
#define _ch 0x20FD
#define _dl 0x20FE
#define _dh 0x20FF


#define RTS M_POP(_PC.B.l);M_POP(_PC.B.h);_PC_++
// all user defined function must end by a RTS to go back to
// the PC of the calling function

case 0xFC:
     // Invalid opcode meaning a CD BIOS function have been called
     {
      UInt16 number=Op6502(_PC_--);

#ifndef FINAL_RELEASE
      switch (number)
        {
    case CD_BOOT:
         fprintf(stderr,"BOOT FUNCTION trapped\n");
         break;
    case CD_RESET:
         fprintf(stderr,"RESET FUNCTION trapped\n");
         break;
    case CD_BASE:
         fprintf(stderr,"BASE FUNCTION trapped\n");
         break;
    case CD_READ:
         fprintf(stderr,"READ FUNCTION trapped\n");
         break;
    case CD_SEEK:
         fprintf(stderr,"SEEK FUNCTION trapped\n");
         break;
    case CD_EXEC:
         fprintf(stderr,"EXEC FUNCTION trapped\n");
         break;
    case CD_PLAY:
         fprintf(stderr,"PLAY FUNCTION trapped\n");
         break;
    case CD_SEARCH:
         fprintf(stderr,"SEARCH FUNCTION trapped\n");
         break;
    case CD_PAUSE:
         fprintf(stderr,"PAUSE FUNCTION trapped\n");
         break;
    case CD_STAT:
         fprintf(stderr,"STAT FUNCTION trapped\n");
         break;
    case CD_SUBA:
         fprintf(stderr,"SUBA FUNCTION trapped\n");
         break;
    case CD_DINFO:
         fprintf(stderr,"DINFO FUNCTION trapped\n");
         break;
    case CD_CONTENTS:
         fprintf(stderr,"CONTENT FUNCTION trapped\n");
         break;
    case CD_SUBRQ:
         fprintf(stderr,"SUBRQ FUNCTION trapped\n");
         break;
    case CD_PCMRD:
         fprintf(stderr,"PCMRD FUNCTION trapped\n");
         break;
    case CD_FADE:
         fprintf(stderr,"FADE FUNCTION trapped\n");
         break;
    default:
         fprintf(stderr,"FUNCTION #%02X trapped\n",number);
        }
    {
     UInt16 position = (RAM[SP+_S+2] << 8) +
                       (RAM[SP+_S+1]);
     position -= 3;
    fprintf(stderr,"at IP = %X\nFrom 0X%04X\n",_PC_,position);
    }
#endif



      switch (number)
        {

        case CD_RESET:
             // set ISO values or initialise CD rom
             // don't return any value

             switch (CD_emulation)
               {
                case 1:
                    if (osd_cd_init(ISO_filename) != 0)
                      {
                       Log("CD rom drive init error!\n");
                       exit(4);
                       }
                     break;
                case 2:
                case 3:
                case 4:
                     fill_cd_info();
                     break;
/*
                case 5:
                     fill_HCD_info(ISO_);
                     break;
*/
               }

             _Wr6502(0x222D,1);
             // This byte is set to 1 if a disc if present

             RTS;
             break;




          case CD_READ:
             {
              UChar mode = Rd6502(_dh);
              UInt32 nb_to_read = Rd6502(_al);
              UInt16 offset = Rd6502(_bl) +
                             (Rd6502(_bh) << 8);

              pce_cd_sectoraddy =
                           (Rd6502(_cl) << 16) +
                           (Rd6502(_ch) << 8)  +
                           (Rd6502(_dl));

              pce_cd_sectoraddy +=
                                (Rd6502(0x2274+3*Rd6502(0x2273)) << 16 )+
                                (Rd6502(0x2275+3*Rd6502(0x2273)) << 8 )+
                                (Rd6502(0x2276+3*Rd6502(0x2273)));

#ifndef FINAL_RELEASE
              fprintf(stderr,"reading hooking, cl = %x, ch = %x, dl = %x\n\n" ,
                      Rd6502(_cl),Rd6502(_ch),Rd6502(_dl));
               fprintf(stderr,"\n_AX=0x%04x\n_BX=0x%04x\n_CX=0x%04x\n_DX=0x%04x\n",
                       Rd6502(_al) + 256 * Rd6502(_ah),
                       Rd6502(_bl) + 256 * Rd6502(_bh),
                       Rd6502(_cl) + 256 * Rd6502(_ch),
                       Rd6502(_dl) + 256 * Rd6502(_dh)
                       );

#endif

              switch (mode)
                {

                 case 0: // local, size in byte

                      nb_to_read = Rd6502(_al) + (Rd6502(_ah) << 8);

                      while (nb_to_read>=2048)
                        {
                         int index;
                         pce_cd_read_sector();
                         for (index=0;index<2048;index++)
                             _Wr6502(offset++,cd_read_buffer[index]);
                         nb_to_read-=2048;
                         }

                      if (nb_to_read)
                      {
                       int index;
                       pce_cd_read_sector();
                       for (index=0;index<nb_to_read;index++)
                          _Wr6502(offset++,cd_read_buffer[index]);

                       }

                      _ZF = _A = 0;

                      // TEST
                      // cd_port_1800 |= 0xD0;
                      // TEST

                      cd_sectorcnt = 0;
                      cd_read_buffer = NULL;
                      pce_cd_read_datacnt = 0;


                      RTS;
                      break;


                 case 1: // local, size in sector
                      while (nb_to_read)
                        {
                         int index;
                         pce_cd_read_sector();
                         for (index=0;index<2048;index++)
                             _Wr6502(offset++,cd_read_buffer[index]);
                         nb_to_read--;
                         }
                      _A = 0;

                      // TEST
                      cd_port_1800 |= 0xD0;
                      // TEST

                      cd_sectorcnt = 0;
                      cd_read_buffer = NULL;
                      pce_cd_read_datacnt = 0;


                      // TEST
                      // io.vdc_status=0;
                      // TEST


                      RTS;
                      break;


                 case 2:
                 case 3:
                 case 4:
                 case 5:
                 case 6:
                      {
                       UChar nb_bank_to_fill_completely = nb_to_read >> 2;
                       UChar remaining_block_to_write = nb_to_read & 3;
                       UChar bank_where_to_write = Rd6502(_bl);
                       UInt16 offset_in_bank = 0;
#ifndef FINAL_RELEASE
                       fprintf(stderr,"Writing to bank 0x%x\nNumber of block : 0x%x\n",bank_where_to_write,nb_to_read);
                       fprintf(stderr,"Number of bank to fill completely : 0x%x\nremains %d quarter of bank\n\n",
                              nb_bank_to_fill_completely, remaining_block_to_write );
#endif

                       while (nb_bank_to_fill_completely--)
                         {
                          pce_cd_read_sector();
                          memcpy(ROMMap[bank_where_to_write],cd_read_buffer,2048);

                          pce_cd_read_sector();
                          memcpy(ROMMap[bank_where_to_write]+2048,cd_read_buffer,2048);

                          pce_cd_read_sector();
                          memcpy(ROMMap[bank_where_to_write]+2048 * 2,cd_read_buffer,2048);

                          pce_cd_read_sector();
                          memcpy(ROMMap[bank_where_to_write]+2048 * 3,cd_read_buffer,2048);

                          bank_where_to_write++;
                          }

                       offset_in_bank = 0;
                       while (remaining_block_to_write--)
                         {
                          pce_cd_read_sector();
#ifndef FINAL_RELEASE
                          fprintf(stderr,"Writing quarter to ROMMap[0x%x] + 0x%x\n\n",
                                  bank_where_to_write,offset_in_bank);
#endif
                          memcpy(ROMMap[bank_where_to_write]+offset_in_bank,cd_read_buffer,2048);
                          offset_in_bank += 2048;
                          }

                       }


                      cd_sectorcnt = 0;
                      cd_read_buffer = NULL;
                      pce_cd_read_datacnt = 0;

                      _ZF = _A = 0;
                      RTS;
                      break;



                   case 0xFE:

                      IO_write(0,0);
                      IO_write(2,offset & 0xFF);
                      IO_write(3,offset >> 8);

                      IO_write(0,2);

                      {
                       UInt32 nb_sector;
                       nb_to_read = Rd6502(_al) + (Rd6502(_ah) << 8);
                       nb_sector = (nb_to_read >> 11) + ((nb_to_read & 2047) != 0);
#ifndef FINAL_RELEASE
                       fprintf(stderr,"0xFE mode reading\n0x%04x bytes to read\n%d sectors to read\noffset in VRAM : 0x%04x\n",
                        nb_to_read,nb_sector,offset );
#endif
                       while (nb_sector)
                         {
                          int x,index = min(2048, nb_to_read);
                          pce_cd_read_sector();

                          //memcpy(&VRAM[offset],cd_read_buffer,index);
                          for (x=0;x<index;x+=2)
                             {
                            IO_write(2,cd_read_buffer[x]);
                            IO_write(3,cd_read_buffer[x+1]);


                              }

                          //offset+=index;
                          nb_to_read-=index;
                          nb_sector--;
//                          io.VDC[MAWR].W+=io.vdc_inc*index/2;
                          }

                       cd_sectorcnt = 0;
                       cd_read_buffer = NULL;
                       pce_cd_read_datacnt = 0;

                       _A = 0;
                       RTS;
                       break;
                       }


                 case 0xFF:
#ifndef FINAL_RELEASE
                     fprintf(stderr,"0xFF mode reading\n0x%04x blocks to read\noffset in VRAM : 0x%04x\n\n",
                             nb_to_read,offset);
#endif
                     if (!nb_to_read)
                       _A = 0x22;
                     else
                     {
                      IO_write(0,0);

                      IO_write(2,offset & 0xFF);
                      IO_write(3,offset >> 8);
                      IO_write(0,2);

                      while (nb_to_read)
                        {
                         int index;
                         pce_cd_read_sector();
                         for( index=0;index<2048;index+=2)
                          {
                            IO_write(2,cd_read_buffer[index]);
                            IO_write(3,cd_read_buffer[index+1]);
                          }

                         nb_to_read--;

                         }

                         cd_sectorcnt = 0;
                         cd_read_buffer = NULL;
                         pce_cd_read_datacnt = 0;

                       _A = 0;
                      }
                      RTS;
                      break;

                 default :
               _Wr6502(0x2273,0);
               _PC_+=2;
#ifndef FINAL_RELEASE
               fprintf(stderr,"Reading mode not supported : %d\n_AX=0x%04x\n_BX=0x%04x\n_CX=0x%04x\n_DX=0x%04x\n",
                       mode,
                       Rd6502(_al) + 256 * Rd6502(_ah),
                       Rd6502(_bl) + 256 * Rd6502(_bh),
                       Rd6502(_cl) + 256 * Rd6502(_ch),
                       Rd6502(_dl) + 256 * Rd6502(_dh)
                       );
#endif
               can_write_debug = 1;
                 }

              }
             break;


        case CD_PAUSE:
             switch(CD_emulation)
               {
                case 1:
                     osd_cd_stop_audio();
                     break;
                case 2:
                case 3:
                case 4:
                     break;
                case 5:
                     HCD_pause_playing();
                     break;
                }
             _A = 0;
             RTS;
             break;

        case CD_STAT:
             /* TEST */
             /*
             if (_A)
               {
                 _A = 0;
                 _ZF = 1;
               }
             else
               {
                 _A = cd_port_1800 & 0x80;
                 _ZF = (_A == 0);
                }
              */
             /* Toggle the */
             _ZF = _A = (timer_60&64?0x80:0x00);
             RTS;
             break;

        case CD_SUBA:
             {
              UInt16 offset = Rd6502(_bl) + (Rd6502(_bh) << 8);
              static UChar result = 3;
              result = 3 - result;

              Wr6502(offset,result); // TEST, for golden axe (3) and solid force (0)
              }
             RTS;
             break;

        case CD_PCMRD:
             // do almost nothing
             // fake the audio player, maybe not other piece of code
             _ZF = _A = RAM[0x41];
             //_A = 1;
             RTS;
             break;



        case AD_RESET:
             // do nothing
             // don't return any value
             _ZF = _A = 0;
             RTS;
             break;


        case AD_TRANS:
             {

              UInt32 nb_to_read = Rd6502(_al);
              UInt16 ADPCM_offset = Rd6502(_bl) + (Rd6502(_bh) << 8);

#ifndef FINAL_RELEASE
             fprintf(stderr,"AD_TRANS : Nb of sectors %d\nmust set buffer ? : %d\nADPCM buffer address: %x\nCD sector address : %x\n\n",
	     Rd6502(_al),Rd6502(_dh),Rd6502(_bl) + 256 * Rd6502(_bh),
             Rd6502(_dl) + 256 * (Rd6502(_cl) + 256 * Rd6502(_ch)) );
#endif


              pce_cd_sectoraddy =
                           (Rd6502(_cl) << 16) +
                           (Rd6502(_ch) << 8)  +
                           (Rd6502(_dl));

              pce_cd_sectoraddy += (Rd6502(0x2274+3*Rd6502(0x2273)) << 16 )+
                                  (Rd6502(0x2275+3*Rd6502(0x2273)) << 8 )+
                                  (Rd6502(0x2276+3*Rd6502(0x2273)));

              if (!Rd6502(_dh))
                io.adpcm_dmaptr = ADPCM_offset;
              else
                ADPCM_offset = io.adpcm_dmaptr;

                      while (nb_to_read)
                        {
                         int index;
                         pce_cd_read_sector();
                         for (index=0;index<2048;index++)
                             PCM[ADPCM_offset++]=cd_read_buffer[index];
                         nb_to_read--;
                         }

                io.adpcm_dmaptr = ADPCM_offset;

             _ZF = _A = 0;
             RTS;

             }
             break;



        case AD_READ:
             {

              UInt16 ADPCM_buffer = Rd6502(_cl) + ( Rd6502(_ch) << 8);
              UChar type = Rd6502(_dh);
              UInt16 address = Rd6502(_bl) + ( Rd6502(_bh) << 8);
              UInt16 size = Rd6502(_al) + ( Rd6502(_ah) << 8 );

#ifndef FINAL_RELEASE
            fprintf(stderr,"AD_READ trapped\narguments : buffer address 0x%0x\nlength : 0x%x\ndest type : %x\ndest addr : 0x%x\n\n",
                    Rd6502(_ch) * 256 + Rd6502(_cl),
                    Rd6502(_ah) * 256 + Rd6502(_al),
                    Rd6502(_dh),
                    Rd6502(_bh) * 256 + Rd6502(_bl));
#endif

             switch (type)
               {
                case 0: // memory write
                     io.adpcm_rptr = ADPCM_buffer;
                     while (size)
                       {
                        _Wr6502(address++,PCM[io.adpcm_rptr++]);
                        size--;
                        }
                     break;
                case 0xFF: // VRAM write

                      io.adpcm_rptr = ADPCM_buffer;

                      IO_write(0,0);
                      IO_write(2,address & 0xFF);
                      IO_write(3,address >> 8);

                      IO_write(0,2);

                       while (size)
                         {

#ifndef FINAL_RELEASE
                     Log("BIOS : read %02X from ADPCM[%04X] to VRAM (2) in 0X%04X\n",PCM[io.adpcm_rptr],io.adpcm_rptr,
                         io.VDC[MAWR].W*2);
#endif
                            IO_write(2,PCM[io.adpcm_rptr++]);
                            size--;

                            if (size)
                              {

#ifndef FINAL_RELEASE
                     Log("BIOS : read %02X from ADPCM[%04X] to VRAM (3) in 0X%04X\n",PCM[io.adpcm_rptr],io.adpcm_rptr,
                         io.VDC[MAWR].W*2+1);
#endif


                               IO_write(3,PCM[io.adpcm_rptr++]);
                               size--;
                              }

                          }

                     break;
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                     {
                      UChar bank_to_fill = Rd6502(_bl);
                      UInt32 i;

                      while (size>=2048)
                        {
                         for (i=0;i<2048;i++)
                             ROMMap[bank_to_fill][i] = PCM[io.adpcm_rptr++];

                         bank_to_fill ++;

                         size -= 2048;
                         }

                      for (i=0;i<size;i++)
                         ROMMap[bank_to_fill][i] = PCM[io.adpcm_rptr++];

                      }
                     break;
                default:
                   Log("Type reading not supported in AD_READ : %x\n",type);
                   exit(-2);
               }


             _ZF = _A = 0;
             RTS;
             }
             break;


/*

        case AD_WRITE:
             // do nothing

#ifndef FINAL_RELEASE
            fprintf(stderr,"AD_WRITE trapped\narguments : buffer address 0x%0x\n length : 0x%x\ndest type : %x\ndest addr : 0x%x\n\n",
                    Rd6502(_ch) * 256 + Rd6502(_cl),
                    Rd6502(_ah) * 256 + Rd6502(_al),
                    Rd6502(_dh),
                    Rd6502(_bh) * 256 + Rd6502(_bl));
            Log("AD_WRITE bios hooked\n");
#endif


             _ZF = _A = 0;
             RTS;
             break;

*/

        case AD_PLAY:


         io.adpcm_pptr = (Rd6502(_bl) + (Rd6502(_bh) << 8)) << 1;

         io.adpcm_psize = (Rd6502(_al) + (Rd6502(_ah) << 8)) << 1;

         io.adpcm_rate = 32 / (16 - (Rd6502(_dh) & 15));

         new_adpcm_play = 1;

#ifndef FINAL_RELEASE
             Log("AD_PLAY hooked\nArgs are :\nAX=0x%04X (length)\nBX=0x%04X (ADPCM address)\nDH=0x%02X (Rate)\nDL=0x%02X (Misc.)\n\n",
                 Rd6502(_al) + 256 * Rd6502(_ah),
                 Rd6502(_bl) + 256 * Rd6502(_bh),
                 Rd6502(_dh),
                 Rd6502(_dl));

             {
              FILE *F=fopen("adpcm.pcm","wb");

              fwrite(PCM + (Rd6502(_bl) + 256 * Rd6502(_bh)),
                     Rd6502(_al) + 256 * Rd6502(_ah),
                     1,
                     F);

              fclose(F);

              }

            set_message("ADPLAY started");

            message_delay = 180;


#endif

             _ZF = _A = 0;
             RTS;
             break;
        case AD_CPLAY:
             // do nothing
             Log("AD_CPLAY hooked\nArgs are :\nAX=0x%04X (length)\nBX=0x%04X (ADPCM address)\nDH=0x%02X (Rate)\nDL=0x%02X (Misc.)\n\n",
                 Rd6502(_al) + 256 * Rd6502(_ah),
                 Rd6502(_bl) + 256 * Rd6502(_bh),
                 Rd6502(_dh),
                 Rd6502(_dl));

             _ZF = _A = 0;
             RTS;
             break;
        case AD_STOP:
             // do nothing
             _ZF = _A = 0;
             RTS;
             break;

        case AD_STAT:
             {
              static ret_val = 0;

              ret_val =! ret_val;

              _ZF = _A = ret_val;
             }
             RTS;
             break;


        case CD_DINFO:
	     Log("CD_DINFO hooked\n");
             if (Op6502(_al)==0x02)
               {
                UInt16 buf_offset = Op6502(_bl)+ (Op6502(_bh) << 8);
                   // usually 0x2256 in system 3.0

#ifndef FINAL_RELEASE
               fprintf(stderr,"Emulating data type info of DINFO\n");
               fprintf(stderr,"Info of track %x asked, buffer at 0x%04X\nbl=%x   bh=%x\n",
                          Rd6502(_ah),buf_offset,Rd6502(_bl),Rd6502(_bh));
#endif
                // _ah contain the number of the track


                switch (CD_emulation)
                  {
                   case 2:
                   case 3:
                   case 4:
                   case 5:

                        Wr6502(buf_offset  ,CD_track[bcdbin[Rd6502(_ah)]].beg_min);
                        Wr6502(buf_offset+1,CD_track[bcdbin[Rd6502(_ah)]].beg_sec);
                        Wr6502(buf_offset+2,CD_track[bcdbin[Rd6502(_ah)]].beg_fra);
                        Wr6502(buf_offset+3,CD_track[bcdbin[Rd6502(_ah)]].type);

                        Log("Type of track %d is %d\nIt begins at lsn,%d\n",
                            bcdbin[Rd6502(_ah)],
                            CD_track[bcdbin[Rd6502(_ah)]].type,
                            Time2Frame(CD_track[bcdbin[Rd6502(_ah)]].beg_min,
                                       CD_track[bcdbin[Rd6502(_ah)]].beg_sec,
                                       CD_track[bcdbin[Rd6502(_ah)]].beg_fra) - 150
                                       );

                        break;
                   case 1:
		      {
			int Min, Sec, Fra, Ctrl;
			UChar *buffer = (UChar*) alloca(7);
			
			Log("Type of track asked\n");
			
			osd_cd_track_info(bcdbin[Rd6502(_ah)],
				    &Min,
				    &Sec,
				    &Fra,
				    &Ctrl);


			Wr6502(buf_offset  , binbcd[Min]);
			Wr6502(buf_offset+1, binbcd[Sec]);
			Wr6502(buf_offset+2, binbcd[Fra]);
			Wr6502(buf_offset+3, Ctrl);
			
		      }
		    break;
		  }

		 _A = 0; // All fine

                }
             else if (Op6502(_al)==0x00)
               {
                UInt16 buf_offset = Op6502(_bl)+(Op6502(_bh) << 8);

#ifndef FINAL_RELEASE
               fprintf(stderr,"Emulating number of track info of DINFO\n");
#endif

                switch (CD_emulation)
                  {
                   case 2:
                   case 3:
                   case 4:
                        Wr6502(buf_offset  ,binbcd[01]); // Number of first track  (BCD)
                        Wr6502(buf_offset+1,binbcd[nb_max_track]); // Number of last track (BCD)
                        break;
                   case 1:
		      {
			int first_track, last_track;
			osd_cd_nb_tracks(&first_track,
					 &last_track);
                        Log("Min track = %d\nMax track = %d\n",
                            first_track,
                            last_track);
			Wr6502(buf_offset  ,binbcd[first_track]);
			Wr6502(buf_offset+1,binbcd[last_track]);

		      }
		    
		      break;
                   case 5:
                        Log("HCD: first track %d, last track %d\n",HCD_first_track,HCD_last_track);
                        Wr6502(buf_offset  ,binbcd[HCD_first_track]);
                        Wr6502(buf_offset+1,binbcd[HCD_last_track]);
                        break;
                  }

                _ZF = _A = 0; // All fine
                }
              else
                {

#ifndef FINAL_RELEASE
               fprintf(stderr,"ERROR : function 0X%02X in DINFO not emulated at 0x%04X\n",Op6502(_al),_PC_);
#endif

                _ZF = _A = 1; // problem
                }

             RTS;
             break;

         case CD_PLAY:
              Log("CD_PLAY: AX=%02X%02X, BX=%02X%02X, CX=%02X%02X, DX=%02X%02X\n",
                  Rd6502(_ah),
                  Rd6502(_al),
                  Rd6502(_bh),
                  Rd6502(_bl),
                  Rd6502(_ch),
                  Rd6502(_cl),
                  Rd6502(_dh),
                  Rd6502(_dl));

              if (Rd6502(_bh)==0x80)
                {
                 /* TEST */
/* #ifndef FINAL_RELEASE */
                  Log("Audio track %x asked to play\n\n",Rd6502(_al));
/* #endif */

                  switch (CD_emulation)
                    {
                     case 1:
		      osd_cd_play_audio_track(bcdbin[Rd6502(_al)]);
		      break;
                     case 2:
                     case 3:
                     case 4:
                      break;
                     case 5:

#ifndef FINAL_RELEASE
                          Log("HCD : must play track # %d\n",bcdbin[Rd6502(_al)]);
#endif

                          HCD_play_track(bcdbin[Rd6502(_al)],Rd6502(_dh) & 1);
                          break;
                    }

                }
              else
                {
                 UInt32 begin_sect = (Rd6502(_al) << 16) +
                                     (Rd6502(_ah) << 8) +
                                     (Rd6502(_bl));
                 UInt32 end_sect   = (Rd6502(_cl) << 16) +
                                     (Rd6502(_ch) << 8) +
                                     (Rd6502(_dl));
                 UInt32 sect_len;

                 int min1 = bcdbin[Rd6502(_al)];
                 int sec1 = bcdbin[Rd6502(_ah)];
                 int fra1 = bcdbin[Rd6502(_bl)];

                 int min2 = bcdbin[Rd6502(_cl)];
                 int sec2 = bcdbin[Rd6502(_ch)];
                 int fra2 = bcdbin[Rd6502(_dl)];

                 begin_sect = Time2Frame(min1,sec1,fra1);
                 end_sect = Time2Frame(min2,sec2,fra2);

                 Log("Absolute read : %02d:%02d:%02d - %02d:%02d:%02d\n%d - %d\n",
                     bcdbin[Rd6502(_al)],
                     bcdbin[Rd6502(_ah)],
                     bcdbin[Rd6502(_bl)],
                     bcdbin[Rd6502(_cl)],
                     bcdbin[Rd6502(_ch)],
                     bcdbin[Rd6502(_dl)],
                     begin_sect,
                     end_sect);

                 // Redbook2Time(begin_sect, &min1, &sec1, &fra1);
                 // Redbook2Time(end_sect, &min2, &sec2, &fra2);

                 fra2 -= fra1;
                 if (fra2<0)
                   {
                    fra2 += 75;
                    sec2--;
                    }
                 sec2 -= sec1;
                 if (sec2<0)
                   {
                    sec2 += 60;
                    fra2--;
                    }
                 min2 -= min1;

                 sect_len = Time2Frame(min2,sec2,fra2);

                 switch (CD_emulation)
                   {
                    case 1:
		     osd_cd_stop_audio();
		     osd_cd_play_audio_range(min1,
				       sec1,
				       fra1,
				       min2,
				       sec2,
				       fra2);
		     break;
                    case 5:
#ifndef FINAL_RELEASE
                         Log("HCD : must play sectors %d-%d\n",begin_sect,sect_len);
#endif
                         HCD_play_sectors(begin_sect, sect_len, Rd6502(_dh) & 1);
                         break;
                    }

                 }
              _ZF = _A = 0;
              RTS;
              break;

          case EX_JOYSNS:
               {
                UChar dummy[5],index;

                for (index=0;index<5;index++)
                   {

                    dummy[index]=Rd6502(0x2228+index);

                    _Wr6502(0x2232+index,dummy[index]);

                    _Wr6502(0x2228+index,io.JOY[index]);

                    _Wr6502(0x222D+index,(io.JOY[index]^dummy[index])&io.JOY[index]);
                    }

                }


               RTS;
               break;
/*
	  case PSG_BIOS:
               Log("PSG BIOS hit!!!\n");
               return;
	       break;

          case GRP_BIOS:
               Log("GRP_BIOS hit\n");
               return ;
               break;
/*        case EX_VSYNC:
             // for speedy emu, never wait
             // don't return any value
             RTS;
             break;
*/
	case BM_FREE:
             {
              SInt16 free_mem;

              free_mem = WRAM[4] + (WRAM[5] << 8);
              free_mem-= WRAM[6] + (WRAM[7] << 8);
              free_mem-= 0x12; /* maybe the header */

              if (free_mem < 0)
                free_mem = 0;

              _Wr6502(_cl, free_mem & 0xff);
              _Wr6502(_ch, free_mem >> 8);

              }
             RTS;
             break;
        default:
          _Wr6502(_PC_,CDBIOS_replace[number][0]);
          _Wr6502(_PC_+1,CDBIOS_replace[number][1]);
        }

      }
     break;


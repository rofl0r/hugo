/**************************************************************************/
/*                 'Portable' PC-Engine Emulator Source file              */
/*                                                                        */
/*      1998 by BERO bero@geocities.co.jp                                 */
/*                                                                        */
/*    Modified 1998 by hmmx hmmx@geocities.co.jp                          */
/*    Modified 1999-2002 by Zeograd (Olivier Jolly) zeograd@caramail.com  */
/**************************************************************************/

/* Header section */

#include "pce.h"
#include "iso_ent.h"

#define LOG_NAME "hugo.log"

#define CD_FRAMES 75
#define CD_SECS 60

/* Variable section */

UChar minimum_bios_hooking = 0;

UChar can_write_debug = 0;

UChar *cd_buf = NULL;

UChar *PopRAM;
// Now dynamicaly allocated
// ( size of popRAMsize bytes )
// If someone could explain me why we need it
// the version I have works well without this trick

const UInt32 PopRAMsize = 0x8000;
// I don't really know if it must be 0x8000 or 0x10000

#define ZW      64
//byte ZBuf[ZW*256];
UChar *Page[8], *ROMMap[256];
//BOOL IsROM[8];

UChar *ROM, *PCM, *DMYROM, *IOAREA;
// WRAM = backup RAM (~ the one on CD ROM drive)
// IOAREA = a pointer to the emulated IO zone
// vchange = array of boolean to know whether bg tiles have changed (i.e.
//    vchanges[5]==1 means the 6th tile have changed and VRAM2 should be updated)
//    [to check !]
// vchanges IDEM for sprites
// ROM = the same thing as the ROM file (w/o header)

UChar CDBIOS_replace[0x4d][2];
// Used to know what byte do we have replaced to hook bios functions so that
// we can restore them if needed

int ROM_size;
// obvious, no ?
// actually, the number of block of 0x2000 bytes in the rom

extern SInt32 vmode;
// What is the favorite video mode to use

SInt32 smode;
// what sound card type should we use? (0 means the silent one,
// my favorite : the fastest!!! ; and -1 means AUTODETECT;
// later will avoid autodetection if wanted)

SChar silent = 1;
// a bit different from the previous one, even if asked to
// use a card, we could not be able to make sound...

/*
 * nb_joy no more used
 * unsigned char nb_joy = 1;
 * number of input to poll
 */

int Country;
/* Is this^ initialised anywhere ?
 * You may try to play with if some games don't want to start
 * it could be useful on some cases
 */

int IPeriod;
// Number of cycle between two interruption calls

UInt32 TimerCount;
// int CycleOld;
// int TimerPeriod;
int scanlines_per_frame = 263;
UInt32 scanline;

//int MinLine = 0,MaxLine = 255;
#define MAXDISP 227

char cart_name[PATH_MAX] = "";
// Name of the file containing the ROM

char short_cart_name[PATH_MAX];
// Just the filename without the extension (with a dot)
// you just have to add your own extension...

char short_iso_name[PATH_MAX];
// Just the ISO filename without the extension (with a dot)
// you just have to add your own extension...

char log_filename[PATH_MAX];
// real filename of the logging file
// it thus also includes full path on advanced system

UChar hook_start_cd_system = 0;
// Do we hook CD system to avoid pressing start on main screen

UChar use_eagle = 0;
// eagle use ?

UChar use_scanline = 0;
// use scanline mode ?

char true_file_name[PATH_MAX];
// the name of the file containing the ROM (with path, ext)
// Now needed 'coz of ZIP archiving...

char short_exe_name[PATH_MAX];
// Used to function whatever the launching directory
// Help working under WIN9X without troubles
// Actually, the path of the EXE

char sav_path[PATH_MAX];
// The place where to keep saved games
// currently a subdir of the EXE path named 'SAV'

char video_path[PATH_MAX];
// The place where to keep output pictures

char ISO_filename[PATH_MAX] = "";
// The name of the ISO file

UChar force_header = 1;
// Force the first sector of the code track to be the correct header

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
#if defined(EXTERNAL_DAT) && defined(ALLEGRO)
DATAFILE *datafile;
// A pointer to the datafile where we keep bitmaps...
// Make things looks cleaner.
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

char *bmdefault = NULL;
// Name of the backup memory

UChar cart_reload = 0;
// Once the game ended, do we need to load another rom
// i.e. do we escape game by pressing F12 or do we called the file selector

char effectively_played = 0;
// Well, the name is enough I think...

UChar populus = 0;
// no more hasardous detection
// thanks to the CRC detection
// now, used to know whether the save file
// must contain extra info

UChar US_encoded_card = 0;
// Do we have to swap bit order in the rom

UInt16 NO_ROM;
// Number of the ROM in the database or 0xFFFF if unknown

UChar debug_on_beginning = 0;
// Do we have to set a bp on the reset IP

UChar CD_emulation = 0;
// Do we emulate CD ( == 1)
//            or  ISO file   ( == 2)
//            or  ISQ file   ( == 3)
//            or  plain BIN file ( == 4)
//            or  HCD ( == 5)

UChar builtin_system_used = 0;
// Have we used the .dat included rom or no ?

int scroll = 0;

signed char snd_vol[6][32];
// cooked volume for each channel

Track CD_track[0x100];
// Track
// beg_min -> beginning in minutes since the begin of the CD(BCD)
// beg_sec -> beginning in seconds since the begin of the CD(BCD)
// beg_fr -> beginning in frames   since the begin of the CD(BCD)
// type -> 0 = audio, 4 = data
// beg_lsn -> beginning in number of sector (2048 bytes)
// length -> number of sector

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
#ifndef ALLEGRO
  // #error "It's not compulsory but you should implement fixed type by yourself"
#else
fixed snd_inc[6], snd_pos[6] = { 0, 0, 0, 0, 0, 0 };
// how mush do we have to advance the index of each voice
// limit the use of division in sound mixing and position
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

UInt32 freq_int = 22050;
// frequency of interrupt to be used to make sound

volatile SChar key_delay = 0;
// delay to avoid to many key strokes

volatile char can_blit = 1;
// used to sync screen to 60 image/sec.

volatile UInt32 message_delay = 0;
// if different of zero, we must display the message pointed by pmessage

char exit_message[256] = "";
// What we must display at the end

UChar language;
/* The language of the messages
 * 0 -> English
 * 1 -> French
 * 2 -> Spanish
 * 3 -> Slovenian
 * 4 -> Portuguese
 * 5 -> German
 * 6 -> Dutch
 * 7 -> Polish
 * 8 -> Italian
 */

UChar cd_fade;
// the byte set by the fade function

unsigned char binbcd[0x100] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
  0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
  0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
  0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
  0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99,
};


unsigned char bcdbin[0x100] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0, 0, 0, 0, 0, 0,
  0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0, 0, 0, 0, 0, 0,
  0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0, 0, 0, 0, 0, 0,
  0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0, 0, 0, 0, 0, 0,
  0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0, 0, 0, 0, 0, 0,
  0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0, 0, 0, 0, 0, 0,
  0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0, 0, 0, 0, 0, 0,
  0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0, 0, 0, 0, 0, 0,
  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0, 0, 0, 0, 0, 0,
  0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61, 0x62, 0x63, 0, 0, 0, 0, 0, 0,
};


UChar cd_port_180b = 0;

UChar pce_cd_adpcm_trans_done = 0;

UChar pce_cd_cmdcnt;

FILE *iso_FILE = NULL;

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
PACKFILE *packed_iso_FILE = NULL;
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

UInt32 packed_iso_filesize = 0;

UInt32 ISQ_position = 0;

UInt32 pce_cd_sectoraddy;

UChar pce_cd_sectoraddress[3];

UChar pce_cd_temp_dirinfo[4];

UChar pce_cd_temp_play[4];

UChar pce_cd_temp_stop[4];

UChar pce_cd_dirinfo[4];



// struct cdrom_tocentry pce_cd_tocentry;

UChar nb_max_track = 24;	//(NO MORE BCD!!!!!)

//char cdsystem_path[256];

//extern char   *pCartName;

//extern char snd_bSound;

UInt32 timer_60 = 0;
// how many times do the interrupt have been called

unsigned char joy_mapping[5][16];

int UPeriod = 0;
// Number of frame to skip

static int BaseClock;

UChar video_driver = 0;
/* 0 => Normal driver, normal display
 * 1 => Eagle graphism
 * 2 => Scanline graphism
 */

// Pre declaration of reading function routines
void read_sector_dummy (unsigned char *, UInt32);
void read_sector_CD (unsigned char *, UInt32);
void read_sector_ISO (unsigned char *, UInt32);
void read_sector_ISQ (unsigned char *, UInt32);
void read_sector_BIN (unsigned char *, UInt32);
void read_sector_HCD (unsigned char *, UInt32);

void (*read_sector_method[6]) (unsigned char *, UInt32) =
{
read_sector_dummy,
    read_sector_CD,
    read_sector_ISO, read_sector_ISQ, read_sector_BIN, read_sector_HCD};

char *
check_char (char *s, char c)
{
  while ((*s) && (*s != c))
    s++;

  return *s == c ? s : NULL;

}

char*
strupr(char *s)
{
	char* t = s;
	while (*s)
	{
		*s = toupper(*s);
		s++;
	}
	return t;
}

char *
strcasestr (char *s1, char *s2)
{

  char *tmps1 = (char *) strupr (strdup (s1));
  char *tmps2 = (char *) strupr (strdup (s2));

  char *result = strstr (tmps1, tmps2);

  free (tmps1);
  free (tmps2);

  return result;

}

#if !defined(WIN32)

int
stricmp (char *s1, char *s2)
{

  char *tmps1 = (char *) strupr (strdup (s1));
  char *tmps2 = (char *) strupr (strdup (s2));

  int result = strcmp (tmps1, tmps2);

  free (tmps1);
  free (tmps2);

  return result;

}
#endif

#if defined(ALLEGRO)
void
interrupt_60hz ()
#else
  #if defined(SDL)
  UInt32 interrupt_60hz (UInt32 interval, void *param)
  #endif
#endif
{

  /* Refresh freezed values in RAM */
  for (can_blit = 0; can_blit < current_freezed_values; can_blit++)
    RAM[list_to_freeze[can_blit].position] = list_to_freeze[can_blit].value;

  /* Make the system understand it can blit */
  can_blit = 1;

  /* If we've displayed a message recently, make it less recent */
  if (message_delay)
    message_delay--;

  /* number of call of this function */
  timer_60++;

#if defined(ALLEGRO)  
  
  /* If we've pressed a key recently, make it less recent =) */
  if (key_delay)
    key_delay--;
    
  return;
#else
   #if defined(SDL)
      return interval;
   #endif
 #endif
};

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

/*
 * For allegro, when calling regulary a function trought the timer, we must
 * tell when does the function stops
 */

END_OF_FUNCTION (interrupt_60hz);

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

#if defined(ALLEGRO)

void
delete_file_tmp (char *name, int dummy, int dummy2)
{
  delete_file (name);
  return;
};

#endif

/*****************************************************************************

    Function: Log

    Description: log data into LOG_NAME
    Parameters: like printf
    Return: nothing

*****************************************************************************/
void
Log (char *format, ...)
{
  FILE *log_file;
  char buf[256];

  va_list ap;
  va_start (ap, format);
  vsprintf (buf, format, ap);
  va_end (ap);

  if (!(log_file = fopen (log_filename, "at")))
    return;

  fprintf (log_file, buf);

  fflush (log_file);

  fclose (log_file);

  return;
}

/*****************************************************************************

    Function: init_log_file

    Description: destroy the current log file, and create another
    Parameters: none
    Return: nothin

*****************************************************************************/
void
init_log_file ()
{
#ifdef MSDOS

  struct time Time;
  struct date Date;

#endif

  unlink (log_filename);
  Log ("--[ INITIALISATION ]--------------------------------\n");

#ifdef MSDOS

  getdate (&Date);
  gettime (&Time);
  Log
    ("Creating Dos Hu-Go! log file on %02d:%02d:%02d.%02d, the %d/%d/%d\nVersion 2.09 alpha of %s\n",
     Time.ti_hour, Time.ti_min, Time.ti_sec, Time.ti_hund, Date.da_day,
     Date.da_mon, Date.da_year, __DATE__);

#elif defined(LINUX)
  Log ("Creating Linux log file version 2.09 alpha of %s\n", __DATE__);
#elif defined(WIN32)
  Log ("Creating Win32 log file version 2.09 alpha of %s\n", __DATE__);
#endif

}

extern int op6502_nb;
UInt32 bank_set_nb = 0;

void
bank_set (UChar P, UChar V)
{
#if !(defined(FINAL_RELEASE))
  if ((V >= ROM_size && (V < 0x68)) || ((V > 0x87) && (V < 0xF7)))
//      if (V>=ROM_size && V<0xF7)
    fprintf (stderr, "ignore bank set %x:%02x\n", P, V);
#endif

  // bank_set_nb ++;

#if !defined(FINAL_RELEASE)
#warning REMOVE ME
  if ((V >= 0x40) && (V <= 0x43))
    printf ("AC bank access : %X\n", V);
#endif
  
  if (ROMMap[V] == IOAREA)
    Page[P] = IOAREA;
  else
    Page[P] = ROMMap[V] - P * 0x2000;
}

UChar
_Rd6502 (UInt16 A)
{
  if (Page[A >> 13] != IOAREA)
    return Page[A >> 13][A];
  else
    return IO_read (A);
}

void
_Wr6502 (UInt16 A, UChar V)
{
#ifndef SF2
  /*
     if (A>0xE000)
     {
     Log("Write in rom bios at PC = 0X%04X\n",M.PC.W);
     return;
     }
   */
  if (Page[A >> 13] != IOAREA)
    Page[A >> 13][A] = V;
  else
    IO_write (A, V);

/*
        if (Page[A>>13]!=IOAREA)
         {
          if (A>>13<0x68)
             if ((A & 0x1ffc) == 0x1ff0) {
                int i;
                
                ROMMap[0x40] = ROMMap[0] + 0x80000;
                ROMMap[0x40] += (A & 3) * 0x80000;
                
                for (i = 0x41; i <= 0x7f; i++) {
                    ROMMap[i] = ROMMap[i-1] + 0x2000;
                }
       }
          else
            Page[A>>13][A]=V;
         }
        else IO_write(A,V);
*/

#else
  if (Page[A >> 13] == IOAREA)
    IO_write (A, V);
  else
    /* support for SF2CE silliness */
  if ((A & 0x1ffc) == 0x1ff0)
    {
      int i;

      ROMMap[0x40] = ROMMap[0] + 0x80000;
      ROMMap[0x40] += (A & 3) * 0x80000;

      for (i = 0x41; i <= 0x7f; i++)
	{
	  ROMMap[i] = ROMMap[i - 1] + 0x2000;
	}
    }
  else
    Page[A >> 13][A] = V;

#endif
}

void
fill_cd_info ()
{

  UChar Min, Sec, Fra;
  UChar current_track;

  // Track 1 is almost always a audio avertising track
  // 30 sec. seems usual

  CD_track[1].beg_min = binbcd[00];
  CD_track[1].beg_sec = binbcd[02];
  CD_track[1].beg_fra = binbcd[00];

  CD_track[1].type = 0;
  CD_track[1].beg_lsn = 0;	// Number of sector since the
  // beginning of track 1

  CD_track[1].length = 47 * CD_FRAMES + 65;	// Most common

  // CD_track[0x01].length=53 * CD_FRAMES + 65;

  // CD_track[0x01].length=0 * CD_FRAMES + 16;

  nb_sect2msf (CD_track[1].length, &Min, &Sec, &Fra);

// Fra = CD_track[0x01].length % CD_FRAMES;
// Sec = (CD_track[0x01].length) % (CD_FRAMES * CD_SECS) / CD_SECS;
// Min = (CD_track[0x01].length) (CD_FRAMES * CD_SECS);

// Second track is the main code track

  CD_track[2].beg_min = binbcd[bcdbin[CD_track[1].beg_min] + Min];
  CD_track[2].beg_sec = binbcd[bcdbin[CD_track[1].beg_sec] + Sec];
  CD_track[2].beg_fra = binbcd[bcdbin[CD_track[1].beg_fra] + Fra];

  CD_track[2].type = 4;
  CD_track[2].beg_lsn =
    msf2nb_sect (bcdbin[CD_track[2].beg_min] - bcdbin[CD_track[1].beg_min],
		 bcdbin[CD_track[2].beg_sec] - bcdbin[CD_track[1].beg_sec],
		 bcdbin[CD_track[2].beg_fra] - bcdbin[CD_track[1].beg_fra]);
  switch (CD_emulation)
    {
    case 2:
      CD_track[0x02].length = filesize (iso_FILE) / 2048;
      break;
    case 3:
      CD_track[0x02].length = packed_iso_filesize / 2048;
      break;
    case 4:
      CD_track[0x02].length = 140000;
      break;
    default:
      break;
    }




  // Now most track are audio

  for (current_track = 3; current_track < bcdbin[nb_max_track];
       current_track++)
    {

      Fra = CD_track[current_track - 1].length % CD_FRAMES;
      Sec = (CD_track[current_track - 1].length / CD_FRAMES) % CD_SECS;
      Min = (CD_track[current_track - 1].length / CD_FRAMES) / CD_SECS;

      CD_track[current_track].beg_min =
	binbcd[bcdbin[CD_track[current_track - 1].beg_min] + Min];
      CD_track[current_track].beg_sec =
	binbcd[bcdbin[CD_track[current_track - 1].beg_sec] + Sec];
      CD_track[current_track].beg_fra =
	binbcd[bcdbin[CD_track[current_track - 1].beg_fra] + Fra];

      CD_track[current_track].type = 0;
      CD_track[current_track].beg_lsn =
	msf2nb_sect (bcdbin[CD_track[current_track].beg_min] -
		     bcdbin[CD_track[1].beg_min],
		     bcdbin[CD_track[current_track].beg_sec] -
		     bcdbin[CD_track[1].beg_sec],
		     bcdbin[CD_track[current_track].beg_fra] -
		     bcdbin[CD_track[1].beg_fra]);
      // 1 min for all
      CD_track[current_track].length = 1 * CD_SECS * CD_FRAMES;

    }

  // And the last one is generally also code


  Fra = CD_track[nb_max_track - 1].length % CD_FRAMES;
  Sec = (CD_track[nb_max_track - 1].length / CD_FRAMES) % CD_SECS;
  Min = (CD_track[nb_max_track - 1].length / CD_FRAMES) / CD_SECS;

  CD_track[nb_max_track].beg_min =
    binbcd[bcdbin[CD_track[nb_max_track - 1].beg_min] + Min];
  CD_track[nb_max_track].beg_sec =
    binbcd[bcdbin[CD_track[nb_max_track - 1].beg_sec] + Sec];
  CD_track[nb_max_track].beg_fra =
    binbcd[bcdbin[CD_track[nb_max_track - 1].beg_fra] + Fra];

  CD_track[nb_max_track].type = 4;
  CD_track[nb_max_track].beg_lsn =
    msf2nb_sect (bcdbin[CD_track[nb_max_track].beg_min] -
		 bcdbin[CD_track[1].beg_min],
		 bcdbin[CD_track[nb_max_track].beg_sec] -
		 bcdbin[CD_track[1].beg_sec],
		 bcdbin[CD_track[nb_max_track].beg_fra] -
		 bcdbin[CD_track[1].beg_fra]);

  /* Thank to Nyef for having localised a little bug there */
  switch (CD_emulation)
    {
    case 2:
      CD_track[nb_max_track].length = filesize (iso_FILE) / 2048;
      break;
    case 3:
      CD_track[nb_max_track].length = packed_iso_filesize / 2048;
      break;
    case 4:
      CD_track[nb_max_track].length = 14000;
      break;
    default:
      break;
    }

  return;
}


void
read_sector_BIN (unsigned char *p, UInt32 sector)
{
  static first_read = 1;
  static second_track_sector = 0;
  int result;


  if (first_read)
    {
      UChar found = 0, dummy;
      int index_in_header = 0;
      unsigned long position;

      fseek (iso_FILE, 0, SEEK_SET);

      while ((!found) && (!feof (iso_FILE)))
	{
	  dummy = getc (iso_FILE);
	  if (dummy == ISO_header[0])
	    {
	      position = ftell (iso_FILE);
	      index_in_header = 1;
	      while ((index_in_header < 0x800) &&
		     (getc (iso_FILE) == ISO_header[index_in_header++]));

	      if (index_in_header == 0x800)
		{
		  found = 1;
		  second_track_sector = ftell (iso_FILE) - 0x800;
		}

	      fseek (iso_FILE, position, SEEK_SET);
	    }
	}

      first_read = 0;

    }



  for (result = bcdbin[nb_max_track]; result > 0x01; result--)
    {
      if ((sector >= CD_track[binbcd[result]].beg_lsn) &&
	  (sector <= CD_track[binbcd[result]].beg_lsn +
	   CD_track[binbcd[result]].length))
	break;
    }

  if (result != 0x02)
    {
      Log ("Read on non track 2\nTrack %d asked\nsector : 0x%x\n", result,
	   pce_cd_sectoraddy);
      exit (-10);
    }

#ifndef FINAL_RELEASE
  fprintf (stderr, "Loading sector nø%d.\n", pce_cd_sectoraddy);
#endif

  fseek (iso_FILE,
	 second_track_sector + (sector -
				CD_track[binbcd[result]].beg_lsn) * 2352,
	 SEEK_SET);
  fread (p, 2048, 1, iso_FILE);

}

void
read_sector_ISQ (unsigned char *p, UInt32 sector)
{
  int result;
  UInt32 dummy;

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

  for (result = bcdbin[nb_max_track]; result > 0x01; result--)
    {
      if ((sector >= CD_track[binbcd[result]].beg_lsn) &&
	  (sector <= CD_track[binbcd[result]].beg_lsn +
	   CD_track[binbcd[result]].length))
	break;
    }

  if (result != 0x02)
    {
      Log ("Read on non track 2\nTrack %d asked\nsector : 0x%x\n", result,
	   pce_cd_sectoraddy);
      exit (-10);
    }

#ifndef FINAL_RELEASE
  fprintf (stderr, "Loading sector nø%d.\n", pce_cd_sectoraddy);
#endif

  dummy = (sector - CD_track[binbcd[result]].beg_lsn) * 2048;

  if (ISQ_position > dummy)
    {
      pack_fclose (packed_iso_FILE);
      packed_iso_FILE = pack_fopen (ISO_filename, F_READ_PACKED);
      pack_fseek (packed_iso_FILE, dummy);
      ISQ_position = dummy;
    }
  else if (ISQ_position < dummy)
    {
      pack_fseek (packed_iso_FILE, dummy - ISQ_position);
      ISQ_position = dummy;
    }

  pack_fread (p, 2048, packed_iso_FILE);

  ISQ_position += 2048;

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

}

#define CD_BUF_LENGTH 8
UInt32 first_sector = 0;

void
read_sector_CD (unsigned char *p, UInt32 sector)
{
  int i;
#ifndef FINAL_RELEASE
  Log ("Lecture du secteur : %d\n", sector);
#endif

  if (cd_buf != NULL)
    if ((sector >= first_sector) &&
	(sector <= first_sector + CD_BUF_LENGTH - 1))
      {
	memcpy (p, cd_buf + 2048 * (sector - first_sector), 2048);
	return;
      }
    else
      {
	for (i = 0; i < CD_BUF_LENGTH; i++)
	  osd_cd_read (cd_buf + 2048 * i, sector + i);
	first_sector = sector;
	memcpy (p, cd_buf, 2048);
      }
  else
    {
      cd_buf = (UChar *) malloc (CD_BUF_LENGTH * 2048);
      for (i = 0; i < CD_BUF_LENGTH; i++)
	osd_cd_read (cd_buf + 2048 * i, sector + i);
      first_sector = sector;
      memcpy (p, cd_buf, 2048);
    }

  //  osd_cd_read(p,sector);
}

void
read_sector_ISO (unsigned char *p, UInt32 sector)
{
  int result;

  for (result = nb_max_track; result > 0x01; result--)
    {
      if ((sector >= CD_track[result].beg_lsn) &&
	  (sector <= CD_track[result].beg_lsn + CD_track[result].length))
	break;
    }

#ifndef FINAL_RELEASE
  fprintf (stderr, "Loading sector nø%d.\n", pce_cd_sectoraddy);
  Log
    ("Loading sector nø%d.\nAX=%02x%02x\nBX=%02x%02x\nCX=%02x%02x\nDX=%02x%02x\n\n",
     pce_cd_sectoraddy, RAM[0xf9], RAM[0xf8], RAM[0xfb], RAM[0xfa], RAM[0xfd],
     RAM[0xfc], RAM[0xff], RAM[0xfe]);
  Log("temp+2-5 = %x %x %x\ntemp + 1 = %02x\n",RAM[5], RAM[6], RAM[7], RAM[4]);
  Log ("ISO : seek at %d\n", (sector - CD_track[result].beg_lsn) * 2048);
  Log ("Track nø%d begin at %d\n", result, CD_track[result].beg_lsn);
#endif

  if (result != 0x02)
    {
      int i;

      Log ("Read on non track 2\nTrack %d asked\nsector : 0x%x\n", result,
	   pce_cd_sectoraddy);

      /* exit(-10);

       * Don't quit anymore but fill the reading buffer with garbage
       * easily recognizable
       */

      for (i = 0; i < 2048; i += 4)
	*(UInt32 *) & p[i] = 0xDEADBEEF;

      return;
    }

  if (sector == CD_track[result].beg_lsn)
    {				/* We're reading the first sector, the header */
      if (force_header)
	{
	  memcpy (p, ISO_header, 0x800);
	  return;
	}
    }

  fseek (iso_FILE, (sector - CD_track[result].beg_lsn) * 2048, SEEK_SET);
  fread (p, 2048, 1, iso_FILE);

}

void
read_sector_dummy (unsigned char *p, UInt32 sector)
{
  return;
}

void
pce_cd_read_sector (void)
{
  /* Avoid sound jiggling when accessing some sectors */
  if (sound_driver == 1)
    osd_snd_set_volume (0);

#ifndef FINAL_RELEASE
  Log ("Will read sectors using function #%d\n", CD_emulation);
#endif

  (*read_sector_method[CD_emulation]) (cd_sector_buffer, pce_cd_sectoraddy);

  pce_cd_sectoraddy++;

#if 0
  for (result = 0; result < 2048; result++)
    {
      if ((result & 15) == 0)
	{
	  fprintf (stderr, "%03x: ", result);
	}
      fprintf (stderr, "%02x", cd_sector_buffer[result]);
      if ((result & 15) == 15)
	{
	  fprintf (stderr, "\n");
	}
      else
	{
	  fprintf (stderr, " ");
	}
    }
#endif

#ifndef FINAL_RELEASE
#if 0
  {

    FILE *g = fopen ("read.cd", "at");
    int result;

    fprintf (g, "\nsector #%x\n", pce_cd_sectoraddy - 1);

    for (result = 0; result < 2048; result++)
      {
	if ((result & 15) == 0)
	  {
	    fprintf (g, "%03x: ", result);
	  }
	fprintf (g, "%02x", cd_sector_buffer[result]);
	if ((result & 15) == 15)
	  {
	    fprintf (g, "\n");
	  }
	else
	  {
	    fprintf (g, " ");
	  }
      }

    fclose (g);

  }
#endif
#endif

  pce_cd_read_datacnt = 2048;
  cd_read_buffer = cd_sector_buffer;

  /* restore sound volume */
  if (sound_driver == 1)
    osd_snd_set_volume (0);
}

void
issue_ADPCM_dma (void)
{

#ifndef FINAL_RELEASE
  fprintf (stderr, "Will make DMA transfert\n");
  Log ("ADPCM DMA will begin\n");
#endif

  while (cd_sectorcnt--)
    {
      memcpy (PCM + io.adpcm_dmaptr, cd_read_buffer, pce_cd_read_datacnt);

      cd_read_buffer = NULL;

      io.adpcm_dmaptr += pce_cd_read_datacnt;

      pce_cd_read_datacnt = 0;

      pce_cd_read_sector ();

    }

  pce_cd_read_datacnt = 0;

  pce_cd_adpcm_trans_done = 1;

  cd_read_buffer = NULL;

}


void
pce_cd_set_sector_address (void)
{
  pce_cd_sectoraddy = pce_cd_sectoraddress[0] << 16;
  pce_cd_sectoraddy += pce_cd_sectoraddress[1] << 8;
  pce_cd_sectoraddy += pce_cd_sectoraddress[2];
}

/*
 *  convert logical_block_address to m-s-f_number (3 bytes only)
 *  lifted from the cdrom test program in the Linux kernel docs.
 *  hacked up to convert to BCD.
 */
#ifndef LINUX
#define CD_MSF_OFFSET 150
#endif

void
lba2msf (int lba, unsigned char *msf)
{
  lba += CD_MSF_OFFSET;
  msf[0] = binbcd[lba / (CD_SECS * CD_FRAMES)];
  lba %= CD_SECS * CD_FRAMES;
  msf[1] = binbcd[lba / CD_FRAMES];
  msf[2] = binbcd[lba % CD_FRAMES];
}


UInt32
msf2nb_sect (UChar min, UChar sec, UChar frm)
{
  UInt32 result = frm;
  result += sec * CD_FRAMES;
  result += min * CD_FRAMES * CD_SECS;
  return result;
}

void
nb_sect2msf (UInt32 lsn, UChar * min, UChar * sec, UChar * frm)
{

  (*frm) = lsn % CD_FRAMES;
  lsn /= CD_FRAMES;
  (*sec) = lsn % CD_SECS;
  (*min) = lsn / CD_SECS;

  return;
}

void
pce_cd_handle_command (void)
{

  if (pce_cd_cmdcnt)
    {
#ifndef FINAL_RELEASE
      fprintf (stderr, "Command arg received: 0x%02x.\n", cd_port_1801);
#endif

      if (--pce_cd_cmdcnt)
	cd_port_1800 = 0xd0;
      else
	cd_port_1800 = 0xc8;

      switch (pce_cd_curcmd)
	{
	case 0x08:
	  if (!pce_cd_cmdcnt)
	    {
#ifndef FINAL_RELEASE
	      fprintf (stderr, "Read command: %d sectors.\n", cd_port_1801);
	      fprintf (stderr, "Starting at %02x:%02x:%02x.\n",
		       pce_cd_sectoraddress[0], pce_cd_sectoraddress[1],
		       pce_cd_sectoraddress[2]);
	      fprintf (stderr, "MODE : %x\n", Rd6502 (0x20FF));
#endif

	      cd_sectorcnt = cd_port_1801;
	      pce_cd_set_sector_address ();
	      pce_cd_read_sector ();


	      /* TEST */
	      // cd_port_1800 = 0xD0; // Xanadu 2 doesn't block but still crash
	      /* TEST */

#ifndef FINAL_RELEASE
	      fprintf (stderr, "Result of reading : $1800 = 0X%02X\n\n\n",
		       cd_port_1800);
#endif

/* TEST ZEO
    if (Rd6502(0x20ff)==0xfe)
     cd_port_1800 = 0x98;
    else
     cd_port_1800 = 0xc8;
/* ******** */

	    }
	  else
	    pce_cd_sectoraddress[3 - pce_cd_cmdcnt] = cd_port_1801;

	  break;
	case 0xd8:

	  pce_cd_temp_play[pce_cd_cmdcnt] = cd_port_1801;

	  if (!pce_cd_cmdcnt)
	    {
	      cd_port_1800 = 0xd8;
	    }
	  break;
	case 0xd9:

	  pce_cd_temp_stop[pce_cd_cmdcnt] = cd_port_1801;

	  if (!pce_cd_cmdcnt)
	    {
	      cd_port_1800 = 0xd8;
/*
               if (pce_cd_temp_stop[3] == 1)
                 osd_cd_play_audio_track(bcdbin[pce_cd_temp_play[2]]);
               else
*/
	      if ((pce_cd_temp_play[0] |
		   pce_cd_temp_play[1] |
		   pce_cd_temp_stop[0] | pce_cd_temp_stop[1]) == 0)
		{
		  if (CD_emulation == 5)
		    HCD_play_track (bcdbin[pce_cd_temp_play[2]], 1);
		  else
		    osd_cd_play_audio_track (bcdbin[pce_cd_temp_play[2]]);
		}
	      else
		{
		  if (CD_emulation == 5)
		    HCD_play_sectors (Time2Frame (bcdbin[pce_cd_temp_play[2]],
						  bcdbin[pce_cd_temp_play[1]],
						  bcdbin[pce_cd_temp_play
							 [0]]),
				      Time2Frame (bcdbin[pce_cd_temp_stop[2]],
						  bcdbin[pce_cd_temp_stop[1]],
						  bcdbin[pce_cd_temp_stop
							 [0]]),
				      pce_cd_temp_stop[3] == 1);
		  else
		    osd_cd_play_audio_range (bcdbin[pce_cd_temp_play[2]],
					     bcdbin[pce_cd_temp_play[1]],
					     bcdbin[pce_cd_temp_play[0]],
					     bcdbin[pce_cd_temp_stop[2]],
					     bcdbin[pce_cd_temp_stop[1]],
					     bcdbin[pce_cd_temp_stop[0]]);
		}
	      Log ("play from %d:%d:%d:(%d) to %d:%d:%d:(%d)\nloop = %d\n",
		   bcdbin[pce_cd_temp_play[2]],
		   bcdbin[pce_cd_temp_play[1]],
		   bcdbin[pce_cd_temp_play[0]],
		   pce_cd_temp_play[3],
		   bcdbin[pce_cd_temp_stop[2]],
		   bcdbin[pce_cd_temp_stop[1]],
		   bcdbin[pce_cd_temp_stop[0]],
		   pce_cd_temp_stop[3], pce_cd_temp_stop[3] == 1);

	    }
	  break;
	case 0xde:

#ifndef FINAL_RELEASE
	  Log (" Arg for 0xde command is %X, command count is %d\n",
	       cd_port_1801, pce_cd_cmdcnt);
#endif

	  if (pce_cd_cmdcnt)
	    pce_cd_temp_dirinfo[pce_cd_cmdcnt] = cd_port_1801;
	  else
	    {
	      // We have received two arguments in pce_cd_temp_dirinfo
	      // We can use only one
	      // There's an argument indicating the kind of info we want
	      // and an optional argument for track number

	      pce_cd_temp_dirinfo[0] = cd_port_1801;

#ifndef FINAL_RELEASE
	      Log
		(" I'll answer to 0xde command request\nArguments are %x, %x, %x, %x\n",
		 pce_cd_temp_dirinfo[0], pce_cd_temp_dirinfo[1],
		 pce_cd_temp_dirinfo[2], pce_cd_temp_dirinfo[3]);
#endif

	      switch (pce_cd_temp_dirinfo[1])
		{
		case 0:
		  // We want info on number of first and last track

		  switch (CD_emulation)
		    {
		    case 2:
		    case 3:
		    case 4:
		      pce_cd_dirinfo[0] = binbcd[01];	// Number of first track  (BCD)
		      pce_cd_dirinfo[1] = binbcd[nb_max_track];	// Number of last track (BCD)
		      break;
		    case 1:
		      {
			int first_track, last_track;
			osd_cd_nb_tracks (&first_track, &last_track);
			pce_cd_dirinfo[0] = binbcd[first_track];
			pce_cd_dirinfo[1] = binbcd[last_track];
		      }
		      break;
		    case 5:
		      Log ("HCD: first track %d, last track %d\n",
			   HCD_first_track, HCD_last_track);
		      pce_cd_dirinfo[0] = binbcd[HCD_first_track];
		      pce_cd_dirinfo[1] = binbcd[HCD_last_track];
		      break;
		    }		// switch CD emulation

		  cd_read_buffer = pce_cd_dirinfo;
		  pce_cd_read_datacnt = 2;

#ifndef FINAL_RELEASE
		  Log (" Data resulting of 0xde request is %x and %x\n",
		       cd_read_buffer[0], cd_read_buffer[1]);
#endif
		  break;

		case 2:

		  // We want info on the track whose number is pce_cd_temp_dirinfo[0]

		  switch (CD_emulation)
		    {
		    case 2:
		    case 3:
		    case 4:
		    case 5:

		      pce_cd_dirinfo[0] =
			CD_track[bcdbin[pce_cd_temp_dirinfo[0]]].beg_min;
		      pce_cd_dirinfo[1] =
			CD_track[bcdbin[pce_cd_temp_dirinfo[0]]].beg_sec;
		      pce_cd_dirinfo[2] =
			CD_track[bcdbin[pce_cd_temp_dirinfo[0]]].beg_fra;
		      pce_cd_dirinfo[3] =
			CD_track[bcdbin[pce_cd_temp_dirinfo[0]]].type;

#ifndef FINAL_RELEASE
		      Log ("Type of track %d is %d\n",
			   bcdbin[pce_cd_temp_dirinfo[0]],
			   CD_track[bcdbin[pce_cd_temp_dirinfo[0]]].type);
#endif
		      break;
		    case 1:
		      {
			int Min, Sec, Fra, Ctrl;
			UChar *buffer = (UChar *) alloca (7);
			osd_cd_track_info (bcdbin[pce_cd_temp_dirinfo[0]],
					   &Min, &Sec, &Fra, &Ctrl);

			pce_cd_dirinfo[0] = binbcd[Min];
			pce_cd_dirinfo[1] = binbcd[Sec];
			pce_cd_dirinfo[2] = binbcd[Fra];
			pce_cd_dirinfo[3] = Ctrl;

			Log
			  ("The control byte of the audio track #%d is 0x%02X\n",
			   bcdbin[pce_cd_temp_dirinfo[0]], pce_cd_dirinfo[3]);

			break;

		      }		// case CD emulation = 1

		    }		// switch CD emulation

		  pce_cd_read_datacnt = 3;
		  cd_read_buffer = pce_cd_dirinfo;

		  break;

		case 1:

		  switch (CD_emulation)
		    {
		    case 1:
		      {

			int min, sec, fra;

			osd_cd_length (&min, &sec, &fra);

			pce_cd_dirinfo[0] = binbcd[min];
			pce_cd_dirinfo[1] = binbcd[sec];
			pce_cd_dirinfo[2] = binbcd[fra];

			break;
		      }		// case Cd emulation = 1
		    default:
		      pce_cd_dirinfo[0] = 0x25;
		      pce_cd_dirinfo[1] = 0x06;
		      pce_cd_dirinfo[2] = 0x00;
		    }		// switch CD emulation

		  pce_cd_read_datacnt = 3;
		  cd_read_buffer = pce_cd_dirinfo;

		  break;

		}		// switch command of request 0xde

	    }			// end if of request 0xde (receiving command or executing them)

	}			// switch of request

    }				// end if of command arg or new request
  else
    {

      // it's an command ID we're receiving

#ifndef FINAL_RELEASE
      fprintf (stderr, "Command byte received: 0x%02x.\n", cd_port_1801);
#endif

      switch (cd_port_1801)
	{
	case 0x00:
	  cd_port_1800 = 0xD8;
	  break;
	case 0x08:
	  pce_cd_curcmd = cd_port_1801;
	  pce_cd_cmdcnt = 4;
	  break;
	case 0xD8:
	  pce_cd_curcmd = cd_port_1801;
	  pce_cd_cmdcnt = 4;
	  break;
	case 0xD9:
	  pce_cd_curcmd = cd_port_1801;
	  pce_cd_cmdcnt = 4;
	  break;
	case 0xDA:
	  pce_cd_curcmd = cd_port_1801;
	  pce_cd_cmdcnt = 0;

	  if (CD_emulation == 1)
	    osd_cd_stop_audio ();
	  else if (CD_emulation == 5)
	    HCD_pause_playing ();

	  break;
	case 0xDE:
	  /* Get CD directory info */
	  /* First arg is command? */
	  /* Second arg is track? */
	  cd_port_1800 = 0xd0;
	  pce_cd_cmdcnt = 2;
	  pce_cd_read_datacnt = 3;	/* 4 bytes */
	  pce_cd_curcmd = cd_port_1801;
	  break;
	}

/*
        if (cd_port_1801 == 0x00) {
            cd_port_1800 = 0xd8;
        } else if (cd_port_1801 == 0x08) {
            pce_cd_curcmd = cd_port_1801;
            pce_cd_cmdcnt = 4;
        } else if (cd_port_1801 == 0xd8) {
            pce_cd_cmdcnt = 4;
            pce_cd_curcmd = cd_port_1801;
        } else if (cd_port_1801 == 0xd9) {
            pce_cd_cmdcnt = 4;
            pce_cd_curcmd = cd_port_1801;
        } else if (cd_port_1801 == 0xde) {
            // Get CD directory info
            // First arg is command?
            // Second arg is track?
            cd_port_1800 = 0xd0;
            pce_cd_cmdcnt = 2;
            pce_cd_read_datacnt = 3; // 4 bytes
            pce_cd_curcmd = cd_port_1801;
        }
*/

    }
}

void
IO_write (UInt16 A, UChar V)
{
  //printf("w%04x,%02x ",A&0x3FFF,V);

#ifndef FINAL_RELEASE
  if ((A & 0x1F00) == 0x1A00)
    Log ("\nAC Write %02x at %04x\n", V, A);
#endif

  /* CAREFUL */
  // switch(A&0x1CC0) { /* BE CAREFUL, MAY BE VERY VERY UNSTABLE !!!!!!! */
  /* DANGER */

  switch (A & 0x1F00)
    {
    case 0x0000:		/* VDC */
      /* TEST */
      /* if (M.PC.W<0xe000)
         if ((io.vdc_reg) && (io.vdc_reg != 2))
         Log("Writing %x to port %04x, reg = 0x%x\n",V,A,io.vdc_reg);
       */
      switch (A & 3)
	{
	case 0:
	  io.vdc_reg = V & 31;
	  return;
	case 1:
	  return;
	case 2:
	  //printf("vdc_l%d,%02x ",io.vdc_reg,V);
	  switch (io.vdc_reg)
	    {
	    case VWR:		/* Write to video */
	      io.vdc_ratch = V;
	      return;
	    case HDR:		/* Horizontal Definition */
	      {
		typeof (io.screen_w) old_value = io.screen_w;
		io.screen_w = (V + 1) * 8;

#ifndef FINAL_RELEASE
		Log ("Screen width set to %d\n", io.screen_w);
		fprintf (stderr, "screen width set to %d\n\n", io.screen_w);
#endif
		if (io.screen_w == old_value)
		  break;

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
		/* TODO: checking if needed, this could remove an ALLEGRO 
		 * related piece of code
		 */
#ifdef ALLEGRO
		clear (screen);
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

		// (*init_normal_mode[video_driver]) ();
		(*osd_gfx_driver_list[video_driver].init) ();


		{
		  UInt32 x, y = (WIDTH - io.screen_w) / 2 - 512 * WIDTH;
		  for (x = 0; x < 1024; x++)
		    {
		      spr_init_pos[x] = y;
		      y += WIDTH;
		    }
		}
	      }
	      break;

	    case MWR:		/* size of the virtual background screen */
	      {
		static UChar bgw[] = { 32, 64, 128, 128 };
		io.bg_h = (V & 0x40) ? 64 : 32;
		io.bg_w = bgw[(V >> 4) & 3];
	      }
	      break;

	    case BYR:		/* Vertical screen offset */
	      if (!scroll)
		{
		  oldScrollX = ScrollX;
		  oldScrollY = ScrollY;
		  oldScrollYDiff = ScrollYDiff;
		}
	      io.VDC[BYR].B.l = V;
	      scroll = 1;
	      ScrollYDiff = scanline - 1;
	      return;
	    case BXR:		/* Horizontal screen offset */
	      if (!scroll)
		{
		  oldScrollX = ScrollX;
		  oldScrollY = ScrollY;
		  oldScrollYDiff = ScrollYDiff;
		}
	      io.VDC[BXR].B.l = V;
	      scroll = 1;
	      return;
	    }

	  io.VDC[io.vdc_reg].B.l = V;
	  /* all others reg just need to get the value, without
	     additional stuff */


#ifndef FINAL_RELEASE
	  if (io.vdc_reg > 19)
	    {
	      fprintf (stderr, "ignore write lo vdc%d,%02x\n", io.vdc_reg, V);
	    }
#endif

	  return;
	case 3:
	  switch (io.vdc_reg)
	    {
	    case VWR:		/* Write to mem */
	      /* Writing to hi byte actually perform the action */
	      VRAM[io.VDC[MAWR].W * 2] = io.vdc_ratch;
	      VRAM[io.VDC[MAWR].W * 2 + 1] = V;

	      vchange[io.VDC[MAWR].W / 16] = 1;
	      vchanges[io.VDC[MAWR].W / 64] = 1;

	      io.VDC[MAWR].W += io.vdc_inc;
	      io.vdc_ratch = 0;
	      return;

	    case VDW:		/* screen height */
	      {
		typeof (io.screen_h) temp_h = io.screen_h;
		io.VDC[VDW].B.h = V;

		io.screen_h = (io.VDC[VDW].W & 511) + 1;

		//if (CD_emulation)
		//  io.screen_h += 16;

		/*
		   if (io.screen_h > 224)
		   io.screen_h = 224;
		 */

		MaxLine = io.screen_h - 1;

		if (temp_h == io.screen_h)
		  return;
		/* TODO: check utility here too, cf upper */
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
		clear (screen);
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

		// (*init_normal_mode[video_driver]) ();
		(*osd_gfx_driver_list[video_driver].init) ();

	      }
	      return;

	    case LENR:		/* DMA transfert */
	      io.VDC[LENR].B.h = V;

	      /* VRAM to VRAM DMA */
	      memcpy (VRAM + io.VDC[DISTR].W * 2, VRAM + io.VDC[SOUR].W * 2,
		      (io.VDC[LENR].W + 1) * 2);

	      memset (vchange + io.VDC[DISTR].W / 16, 1,
		      (io.VDC[LENR].W + 1) / 16);
	      memset (vchange + io.VDC[DISTR].W / 64, 1,
		      (io.VDC[LENR].W + 1) / 64);

	      io.VDC[DISTR].W += io.VDC[LENR].W + 1;
	      io.VDC[SOUR].W += io.VDC[LENR].W + 1;

	      io.vdc_status |= VDC_DMAfinish;

	      return;

	    case CR:		/* Auto increment size */
	      {
		static UChar incsize[] = { 1, 32, 64, 128 };
		io.vdc_inc = incsize[(V >> 3) & 3];
	      }
	      break;
	    case HDR:		/* Horizontal display end */
	      /* TODO : well, maybe we should implement it */
	      //io.screen_w = (io.VDC_ratch[HDR]+1)*8;
	      //TRACE0("HDRh\n");
	      break;

	    case BYR:		/* Vertical screen offset */
	      if (!scroll)
		{
		  oldScrollX = ScrollX;
		  oldScrollY = ScrollY;
		  oldScrollYDiff = ScrollYDiff;
		}
	      io.VDC[BYR].B.h = V & 1;
	      scroll = 1;
	      ScrollYDiff = scanline - 1;
	      return;
	    case SATB:		/* DMA from VRAM to SATB */
	      io.VDC[SATB].B.h = V;
	      io.vdc_satb = 1;
	      io.vdc_status &= ~VDC_SATBfinish;
	      return;
	    case BXR:		/* Horizontal screen offset */
	      if (!scroll)
		{
		  oldScrollX = ScrollX;
		  oldScrollY = ScrollY;
		  oldScrollYDiff = ScrollYDiff;
		}
	      io.VDC[BXR].B.h = V & 3;
	      scroll = 1;
	      return;
	    }
	  io.VDC[io.vdc_reg].B.h = V;

#ifndef FINAL_RELEASE
	  if (io.vdc_reg > 19)
	    {
	      fprintf (stderr, "ignore write hi vdc%d,%02x\n", io.vdc_reg, V);
	    }
#endif

	  return;
	}
      break;

    case 0x0400:		/* VCE */
      switch (A & 7)
	{
	case 0:		/*io.vce_reg.W = 0; io.vce_ratch=0; *//*?? */
	  /*TRACE("VCE 0, V=%X\n", V); */ return;

	  /* Choose color index */
	case 2:
	  io.vce_reg.B.l = V;
	  return;
	case 3:
	  io.vce_reg.B.h = V & 1;
	  return;

	  /* Set RGB components for current choosen color */
	case 4:
	  io.VCE[io.vce_reg.W].B.l = V;
	  {
	    UChar c;
	    int i, n;
	    n = io.vce_reg.W;
	    c = io.VCE[n].W >> 1;
	    if (n == 0)
	      {
		for (i = 0; i < 256; i += 16)
		  Pal[i] = c;
	      }
	    else if (n & 15)
	      Pal[n] = c;
	  }


	  return;
	case 5:
	  io.VCE[io.vce_reg.W].B.h = V;
	  {
	    UChar c;
	    int i, n;
	    n = io.vce_reg.W;
	    c = io.VCE[n].W >> 1;
	    if (n == 0)
	      {
		for (i = 0; i < 256; i += 16)
		  Pal[i] = c;
	      }
	    else if (n & 15)
	      Pal[n] = c;
	  }
	  io.vce_reg.W = (io.vce_reg.W + 1) & 0x1FF;
	  return;
	}
      break;


    case 0x0800:		/* PSG */

#define BASE_FREQ 3700000

#ifdef USE_MORE_SOUND_VAR

#define ajust_vol { char j; \
   fixed tmp=itofix((io.PSG[io.psg_ch][4]&15)* \
                   ((io.PSG[io.psg_ch][5]&15)+(io.PSG[io.psg_ch][5]>>4))* \
                   ((io.psg_volume&15)+(io.psg_volume>>4)))/(6*32*32) ; \
   for (tmp2=0,j=0;j<32;j++,tmp2+=tmp) \
      snd_vol[io.psg_ch][j]=fixtoi(tmp2); }
#endif

      switch (A & 15)
	{

	  /* Select PSG channel */
	case 0:
	  io.psg_ch = V & 7;
	  return;

	  /* Select global volume */
	case 1:
	  io.psg_volume = V;
	  return;

	  /* Frequency setting, 8 lower bits */
	case 2:
	  io.PSG[io.psg_ch][2] = V;

#ifdef USE_MORE_SOUND_VAR
	  if ((!V) && (!io.PSG[io.psg_ch][3]))
	    snd_inc[io.psg_ch] = 0;
	  else
	    snd_inc[io.psg_ch] =
	      itofix (BASE_FREQ /
		      (io.PSG[io.psg_ch][2] +
		       io.PSG[io.psg_ch][3] * 256)) / freq_int;
#endif
	  break;
	  /* Frequency setting, 4 upper bits */
	case 3:
	  io.PSG[io.psg_ch][3] = V & 15;

#ifdef USE_MORE_SOUND_VAR
	  if ((!(V & 15)) && (!io.PSG[io.psg_ch][2]))
	    snd_inc[io.psg_ch] = 0;
	  else
	    snd_inc[io.psg_ch] =
	      itofix (BASE_FREQ /
		      (io.PSG[io.psg_ch][2] +
		       io.PSG[io.psg_ch][3] * 256)) / freq_int;
#endif
	  break;

	case 4:
	  io.PSG[io.psg_ch][4] = V;
#ifdef USE_MORE_SOUND_VAR
	  ajust_vol;
#endif
	  break;

	  /* Set channel specific volume */
	case 5:
	  io.PSG[io.psg_ch][5] = V;
#ifdef USE_MORE_SOUND_VAR
	  ajust_vol;
#endif
	  break;

	  /* Put a value into the channel sample or directly into the mixer */
	case 6:
	  if (io.PSG[io.psg_ch][4] & 0x40)
	    {
	      io.wave[io.psg_ch][0] = V & 31;
	    }
	  else
	    {
	      io.wave[io.psg_ch][io.wavofs[io.psg_ch]] = V & 31;
	      io.wavofs[io.psg_ch] = (io.wavofs[io.psg_ch] + 1) & 31;
#ifdef USE_MORE_SOUND_VAR
	      io.wave[io.psg_ch][fixtoi (snd_pos[io.psg_ch])] = V & 31;
	      snd_pos[io.psg_ch] =
		itofix ((fixtoi (snd_pos[io.psg_ch]) + 1) & 31);
#endif
	    }
	  break;
	case 7:		/*if (io.psg_ch!=5) */
	  io.PSG[io.psg_ch][7] = V;
	  /*else io.PSG[5][7] = 135; */
	  break;

	case 8:
	  io.psg_lfo_freq = V;
	  break;
	case 9:
	  io.psg_lfo_ctrl = V;
	  break;

#ifndef FINAL_RELEASE
	default:
	  fprintf (stderr, "ignored PSG write\n");
#endif
	}
      return;

    case 0x0c00:		/* timer */
      //TRACE("Timer Access: A=%X,V=%X\n", A, V);
      switch (A & 1)
	{
	case 0:
	  io.timer_reload = V & 127;
	  return;
	case 1:
	  V &= 1;
	  if (V && !io.timer_start)
	    io.timer_counter = io.timer_reload;
	  io.timer_start = V;
	  return;
	}
      break;

    case 0x1000:		/* joypad */
//        TRACE("V=%02X\n", V);
      io.joy_select = V & 1;
      //io.joy_select = V;
      if (V & 2)
	io.joy_counter = 0;
      return;

    case 0x1400:		/* IRQ */
      switch (A & 15)
	{
	case 2:
	  io.irq_mask = V;	/*TRACE("irq_mask = %02X\n", V); */
	  return;
	case 3:
	  io.irq_status = (io.irq_status & ~TIRQ) | (V & 0xF8);
	  return;
	}
      break;

    case 0x1A00:
      {

	if ((A & 0x1AF0) == 0x1AE0)
	  {
	    switch (A & 15)
	      {
	      case 0:
		io.ac_shift = (io.ac_shift & 0xffffff00) | V;
		break;
	      case 1:
		io.ac_shift = (io.ac_shift & 0xffff00ff) | (V << 8);
		break;
	      case 2:
		io.ac_shift = (io.ac_shift & 0xff00ffff) | (V << 16);
		break;
	      case 3:
		io.ac_shift = (io.ac_shift & 0x00ffffff) | (V << 24);
		break;
	      case 4:
		io.ac_shiftbits = V & 0x0f;
		if (io.ac_shiftbits != 0)
		  {
		    if (io.ac_shiftbits < 8)
		      {
			io.ac_shift <<= io.ac_shiftbits;
		      }
		    else
		      {
			io.ac_shift >>= (16 - io.ac_shiftbits);
		      }
		  }
	      default:
		break;
	      }
	    return;
	  }
	else
	  {
	    UChar ac_port = (A >> 4) & 3;
	    switch (A & 15)
	      {
	      case 0:
	      case 1:

		if (io.ac_control[ac_port] & AC_USE_OFFSET)
		  {
		    // Log("Write %d to 0x%04X (base + offset)\n",V,
		    //           io.ac_offset[ac_port] + io.ac_base[ac_port]);
		    ac_extra_mem[((io.ac_base[ac_port] +
				   io.ac_offset[ac_port]) & 0x1fffff)] = V;
		  }
		else
		  {
		    // Log("Write %d to 0x%04X (base)\n",V, io.ac_base[ac_port]);
		    ac_extra_mem[((io.ac_base[ac_port]) & 0x1fffff)] = V;
		  }

		if (io.ac_control[ac_port] & AC_ENABLE_INC)
		  {
		    if (io.ac_control[ac_port] & AC_INCREMENT_BASE)
		      io.ac_base[ac_port] =
			(io.ac_base[ac_port] +
			 io.ac_incr[ac_port]) & 0xffffff;
		    else
		      io.ac_offset[ac_port] =
			(io.ac_offset[ac_port] +
			 io.ac_incr[ac_port]) & 0xffffff;
		  }

	      case 2:
		io.ac_base[ac_port] = (io.ac_base[ac_port] & 0xffff00) | V;
		return;
	      case 3:
		io.ac_base[ac_port] =
		  (io.ac_base[ac_port] & 0xff00ff) | (V << 8);
		return;
	      case 4:
		io.ac_base[ac_port] =
		  (io.ac_base[ac_port] & 0x00ffff) | (V << 16);
		return;
	      case 5:
		io.ac_offset[ac_port] = (io.ac_offset[ac_port] & 0xff00) | V;
		return;
	      case 6:
		io.ac_offset[ac_port] =
		  (io.ac_offset[ac_port] & 0x00ff) | (V << 8);
		if (io.ac_control[ac_port] & (AC_ENABLE_OFFSET_BASE_6))
		  io.ac_base[ac_port] =
		    (io.ac_base[ac_port] + io.ac_offset[ac_port]) & 0xffffff;
		return;
	      case 7:
		io.ac_incr[ac_port] = (io.ac_incr[ac_port] & 0xff00) | V;
		return;
	      case 8:
		io.ac_incr[ac_port] =
		  (io.ac_incr[ac_port] & 0x00ff) | (V << 8);
		return;
	      case 9:
		io.ac_control[ac_port] = V;
		return;
	      case 0xa:
		if (io.
		    ac_control[ac_port] & (AC_ENABLE_OFFSET_BASE_A |
					   AC_ENABLE_OFFSET_BASE_6) ==
		    (AC_ENABLE_OFFSET_BASE_A | AC_ENABLE_OFFSET_BASE_6))
		  io.ac_base[ac_port] =
		    (io.ac_base[ac_port] + io.ac_offset[ac_port]) & 0xffffff;
		return;
	      default:
		Log ("\nUnknown AC write %d into 0x%04X\n", V, A);
	      }

	  }
      }
      break;

    case 0x1800:		/* CD-ROM extention */
      switch (A & 15)
	{

	case 7:
	  io.backup = ENABLE;
	  return;

	case 0:
	  if (V == 0x81)
	    cd_port_1800 = 0xD0;
	  return;
	case 1:
	  cd_port_1801 = V;
	  if (!pce_cd_cmdcnt)
	    switch (V)
	      {
	      case 0:
/*
#ifndef FINAL_RELEASE
        fprintf(stderr,"RESET? command at 1801\n");
#endif
*/
		return;
	      case 3:
/*
#ifndef FINAL_RELEASE
        fprintf(stderr,"GET SYSTEM STATUS? command at 1801\n");
#endif
*/
		return;
	      case 8:
/*
#ifndef FINAL_RELEASE
        fprintf(stderr,"READ SECTOR command at 1801\n");
#endif*/
		return;
	      case 0x81:
/*
#ifndef FINAL_RELEASE
        fprintf(stderr,"ANOTHER RESET? command at 1801\n");
#endif
*/
		cd_port_1800 = 0x40;
		return;
	      case 0xD8:
	      case 0xD9:
/*
#ifndef FINAL_RELEASE
        fprintf(stderr,"PLAY AUDIO? command at 1801\n");
#endif
*/
		return;

	      case 0xDA:
/*
#ifndef FINAL_RELEASE
        fprintf(stderr,"PAUSE AUDIO PLAYING? command at 1801\n");
#endif
*/
		return;

	      case 0xDD:
/*
#ifndef FINAL_RELEASE
        fprintf(stderr,"READ Q CHANNEL? command at 1801\n");
#endif
*/
		return;
	      case 0xDE:
/*
#ifndef FINAL_RELEASE
        fprintf(stderr,"GET DIRECTORY INFO? command at 1801\n");
#endif
*/

		return;

	      default:
/*
#ifndef FINAL_RELEASE
        if (!pce_cd_cmdcnt)
         fprintf(stderr,"ERROR, unknown command %x at 1801\n",V);
#endif
*/
		return;
	      }

	  return;

	case 2:
#ifndef FINAL_RELEASE
	  // fprintf(stderr,"trying to access port 1802 in write\n");
#endif

	  if ((!(cd_port_1802 & 0x80)) && (V & 0x80))
	    {
	      cd_port_1800 &= ~0x40;
	    }
	  else if ((cd_port_1802 & 0x80) && (!(V & 0x80)))
	    {
	      cd_port_1800 |= 0x40;

#ifndef FINAL_RELEASE
	      Log ("ADPCM trans = %d\n", pce_cd_adpcm_trans_done);
#endif
	      if (pce_cd_adpcm_trans_done)
		{

#ifndef FINAL_RELEASE
		  Log ("ack the DMA transfert of ADPCM data\n");
#endif

		  cd_port_1800 |= 0x10;
		  pce_cd_curcmd = 0x00;
		  pce_cd_adpcm_trans_done = 0;

		}

	      if (cd_port_1800 & 0x08)
		{
/*              deb_printf("pce_cd: data byte acknowledged.\n"); */
		  if (cd_port_1800 & 0x20)
		    {
		      cd_port_1800 &= ~0x80;
		    }
		  else if (!pce_cd_read_datacnt)
		    {
		      if (pce_cd_curcmd == 0x08)
			{
			  if (!--cd_sectorcnt)
			    {
#ifndef FINAL_RELEASE
			      fprintf (stderr, "sector data count over.\n");
#endif
			      cd_port_1800 |= 0x10;	/* wrong */
			      pce_cd_curcmd = 0x00;
			    }
			  else
			    {
#ifndef FINAL_RELEASE
			      fprintf (stderr, "sector data count %d.\n",
				       cd_sectorcnt);
#endif
			      pce_cd_read_sector ();
			    }
			}
		      else
			{
			  if (cd_port_1800 & 0x10)
			    {
			      cd_port_1800 |= 0x20;
			    }
			  else
			    {
			      cd_port_1800 |= 0x10;
			    }
			}
		    }
		  else
		    {
		      pce_cd_read_datacnt--;
		    }
		}
	      else
		{
		  pce_cd_handle_command ();
		}
	    }

	  cd_port_1802 = V;
	  return;
	case 4:
	  if (V & 2)
	    {			// Reset asked
	      // do nothing for now
#ifndef FINAL_RELEASE
	      fprintf (stderr, "Reset mode for CD asked\n");
#endif

	      switch (CD_emulation)
		{
		case 1:
		  if (osd_cd_init (ISO_filename) != 0)
		    {
		      Log ("CD rom drive couldn't be initialised\n");
		      exit (4);
		    }
		  break;
		case 2:
		case 3:
		case 4:
		  fill_cd_info ();
		  break;
/*
                case 5:
                     fill_HCD_info(ISO_);
                     break;
*/
		}

	      _Wr6502 (0x222D, 1);
	      // This byte is set to 1 if a disc if present

	      //cd_port_1800 &= ~0x40;
	      cd_port_1804 = V;
	    }
	  else
	    {			// Normal utilisation
#ifndef FINAL_RELEASE
	      fprintf (stderr, "Normal mode for CD asked\n");
#endif
	      cd_port_1804 = V;
	      // cd_port_1800 |= 0x40; // Maybe the previous reset is enough
	      // cd_port_1800 |= 0xD0;
	      // Indicates that the Hardware is ready after such a reset
	    }
	  return;

	case 8:
	  io.adpcm_ptr.B.l = V;
	  return;
	case 9:
	  io.adpcm_ptr.B.h = V;
	  return;

	case 0x0A:
	  PCM[io.adpcm_wptr++] = V;
#ifndef FINAL_RELEASE
	  fprintf (stderr, "Wrote %02X to ADPCM buffer[%04X]\n", V,
		   io.adpcm_wptr - 1);
	  Log ("wrote to ADPCM, %02X at PCM[0X%04X]\n", V, io.adpcm_wptr - 1);
#endif
	  return;
	case 0x0B:		// DMA enable ?

	  if ((V & 2) && (!(cd_port_180b & 2)))
	    {
	      issue_ADPCM_dma ();
	      cd_port_180b = V;
	      return;
	    }


	  /* TEST */
	  if (!V)
	    {
	      cd_port_1800 &= ~0xF8;
	      cd_port_1800 |= 0xD8;
	    }

	  cd_port_180b = V;

	  return;

	case 0x0C:		/* TEST, not nyef code */
	  // well, do nothing

	  return;
	case 0x0D:

	  if ((V & 0x03) == 0x03)
	    {
	      io.adpcm_dmaptr = io.adpcm_ptr.W;	// set DMA pointer
#ifndef FINAL_RELEASE
	      fprintf (stderr, "Set DMA pointer to %x\n", io.adpcm_dmaptr);
#endif
	    }

	  if (V & 0x04)
	    {
	      io.adpcm_wptr = io.adpcm_ptr.W;	// set write pointer
#ifndef FINAL_RELEASE
	      fprintf (stderr, "Set write pointer to %x\n", io.adpcm_wptr);
#endif
	    }

	  if (V & 0x08)		// set read pointer
	    {
	      io.adpcm_rptr = io.adpcm_ptr.W;
	      io.adpcm_firstread = 2;

#ifndef FINAL_RELEASE
	      fprintf (stderr, "Set read pointer to %x\n", io.adpcm_rptr);
#endif

	    }

/* TEST
                else { io.adpcm_rptr = io.adpcm_ptr.W; io.adpcm_firstread = TRUE; }
*/
	  /* TEST */
	  //if (V&0x08) io.

	  if (V & 0x80)
	    {			// ADPCM reset
#ifndef FINAL_RELEASE
	      fprintf (stderr, "Reset mode for ADPCM\n");
#endif
	    }
	  else
	    {			// Normal ADPCM utilisation
#ifndef FINAL_RELEASE
	      fprintf (stderr, "Normal mode for ADPCM\n");
#endif
	    }



	  return;

	case 0xe:		// Set ADPCM playback rate
	  io.adpcm_rate = 32 / (16 - (V & 15));

#ifndef FINAL_RELEASE
	  fprintf (stderr, "ADPCM rate set to %d kHz\n", io.adpcm_rate);
#endif
	  return;

	case 0xf:		// don't know how to use it
#ifndef FINAL_RELEASE
	  fprintf (stderr, "Fade setting to %d\n", V);
#endif

	  cd_fade = V;
	  return;
	}			// A&15 switch, i.e. CD ports
      break;
    }
#ifndef FINAL_RELEASE
  fprintf (stderr,
	   "ignore I/O write %04x,%02x\tBase adress of port %X\nat PC = %04X\n",
	   A, V, A & 0x1CC0,
#ifdef KERNEL_DS
	   reg_pc);
#else
	   M.PC.W);
#endif

#endif
//      DebugDumpTrace(4, TRUE);
}

static double osd_getTime(void)
{
#ifdef WIN32
  return SDL_GetTicks() * 1e-3;
#elif defined(DJGPP)
  return uclock() * (1.0 / UCLOCKS_PER_SEC);
#else
  struct timeval tp;

  gettimeofday(&tp, NULL);
  // printf("current microsec = %f\n",tp.tv_sec + 1e-6 * tp.tv_usec);
  return tp.tv_sec + 1e-6 * tp.tv_usec;
#endif
}

static void osd_sleep(double s)
{
  // emu_too_fast = 0;
	// printf("Sleeping %d micro seconds\n", s);
	// Log("Sleeping %f micro seconds\n", s);
  if (s > 0)
  {
#ifdef linux
    struct timeval tp;

    tp.tv_sec = 0;
    tp.tv_usec = 1e6 * s;
    select(1,NULL,NULL,NULL,&tp);
#elif defined(WIN32)
    SDL_Delay(s * 1e3);
#elif defined(DJGPP)
    double curtime = osd_time();
    while ((curtime + s) > osd_time());
#else
    usleep(s * 1e6);
#endif
    // emu_too_fast = 1;
  }
}


#ifndef KERNEL_DS

/* write */
M6502 M;


UChar
Loop6502 (M6502 * R)
#else
UChar
Loop6502 ()
#endif
{
  static double lasttime = 0, lastcurtime = 0, frametime = 0.1;	
	
  static int UCount = 0;
  static int prevline;
  int dispmin, dispmax;
  int ret;

  dispmin = 0;
/*
    (MaxLine - MinLine >
     MAXDISP ? MinLine + ((MaxLine - MinLine - MAXDISP + 1) >> 1) : MinLine);
*/
  dispmax = 242;
/*
    (MaxLine - MinLine >
     MAXDISP ? MaxLine - ((MaxLine - MinLine - MAXDISP + 1) >> 1) : MaxLine);
*/

  scanline = (scanline + 1) % scanlines_per_frame;
  //printf("scan %d\n",scanline);
  ret = INT_NONE;
  io.vdc_status &= ~VDC_RasHit;
  if (scanline > MaxLine)
    io.vdc_status |= VDC_InVBlank;
//      if (scanline==MinLine+scanlines_per_frame-1)
//      else 
  if (scanline == MinLine)
    {
      io.vdc_status &= ~VDC_InVBlank;
      prevline = dispmin;
      ScrollYDiff = 0;
      oldScrollYDiff = 0;
//              if (io.vdc_iswrite_h)
//              {
//                      io.vdc_iswrite_h = 0;
//                      ScrollY = io.VDC[BYR].W;
//              }
//              TRACE("\nFirstLine\n");
    }
  else if (scanline == MaxLine)
    {
      if (CheckSprites ())
	io.vdc_status |= VDC_SpHit;
      else
	io.vdc_status &= ~VDC_SpHit;
      if (UCount)
	UCount--;
      else
	{
	  if (SpriteON && SPONSwitch)
	    RefreshSpriteExact (prevline, dispmax, 0);
	  RefreshLine (prevline, dispmax - 1);
	  if (SpriteON && SPONSwitch)
	    RefreshSpriteExact (prevline, dispmax, 1);
	  prevline = dispmax;
	  UCount = UPeriod;
	  RefreshScreen ();
	}
    }
  if (scanline >= MinLine && scanline <= MaxLine)
    {
      if (scanline == (io.VDC[RCR].W & 1023) - 64)
	{
	  if (RasHitON && !UCount && dispmin <= scanline
	      && scanline <= dispmax)
	    {
	      /* TODO : check the '6' there */
	      if (SpriteON && SPONSwitch)
		RefreshSpriteExact (prevline - 0, scanline, 0);
	      RefreshLine (prevline - 0, scanline - 1);
	      if (SpriteON && SPONSwitch)
		RefreshSpriteExact (prevline - 0, scanline, 1);
	      prevline = scanline;
	    }
	  io.vdc_status |= VDC_RasHit;
	  if (RasHitON)
	    {
	      //TRACE("rcr=%d\n", scanline);
	      ret = INT_IRQ;
	    }
	}
      else if (scroll)
	{
	  if (scanline - 1 > prevline && !UCount)
	    {
	      int tmpScrollX, tmpScrollY, tmpScrollYDiff;
	      tmpScrollX = ScrollX;
	      tmpScrollY = ScrollY;
	      tmpScrollYDiff = ScrollYDiff;
	      ScrollX = oldScrollX;
	      ScrollY = oldScrollY;
	      ScrollYDiff = oldScrollYDiff;
	      if (SpriteON && SPONSwitch)
		RefreshSpriteExact (prevline, scanline - 1, 0);
	      RefreshLine (prevline, scanline - 2);
	      if (SpriteON && SPONSwitch)
		RefreshSpriteExact (prevline, scanline - 1, 1);
	      prevline = scanline - 1;
	      ScrollX = tmpScrollX;
	      ScrollY = tmpScrollY;
	      ScrollYDiff = tmpScrollYDiff;
	    }
	}
    }
  else
    {
      int rcr = (io.VDC[RCR].W & 1023) - 64;
      if (scanline == rcr)
	{
//                      ScrollYDiff = scanline;
	  if (RasHitON)
	    {
	      //TRACE("rcr=%d\n", scanline);
	      io.vdc_status |= VDC_RasHit;
	      ret = INT_IRQ;
	    }
	}
    }
  scroll = 0;
  if (scanline == MaxLine + 1)
    {

      if (osd_keyboard ())
	return INT_QUIT;
	 
	  	{
        double curtime;
		const double deltatime = (1.0 / 60.0);

         curtime = osd_getTime();
		
		 // printf("lasttime = %8f, curtime = %8f, aimed = %8f\n", lasttime, curtime, lasttime + deltatime);
		
          osd_sleep(lasttime + deltatime - curtime);
          curtime = osd_getTime();

          /* make average time */
          // frametime = (frametime * 4.0 + curtime - lastcurtime) * 0.2;
          // fps = 1.0 / frametime;
          lastcurtime = curtime;

          lasttime += deltatime;
          if ((lasttime + deltatime) < curtime)
            lasttime = curtime;

	}
	  

      /* VRAM to SATB DMA */
      if (io.vdc_satb == 1 || io.VDC[DCR].W & 0x0010)
	{
	  memcpy (SPRAM, VRAM + io.VDC[SATB].W * 2, 64 * 8);
	  io.vdc_satb = 1;
	  io.vdc_status &= ~VDC_SATBfinish;
	}
      if (ret == INT_IRQ)
	io.vdc_pendvsync = 1;
      else
	{
	  //io.vdc_status|=VDC_InVBlank;
	  if (VBlankON)
	    {
	      ret = INT_IRQ;
	    }
	}
    }
  else if (scanline == min (MaxLine + 5, scanlines_per_frame - 1))
    {
      if (io.vdc_satb)
	{
	  io.vdc_status |= VDC_SATBfinish;
	  io.vdc_satb = 0;
	  if (SATBIntON)
	    {
	      ret = INT_IRQ;
	    }
/*              } else {
                        io.vdc_status&=~VDC_SATBfinish;
                        io.vdc_satb = 0;
*/ }
    }
  else if (io.vdc_pendvsync && ret != INT_IRQ)
    {
      io.vdc_pendvsync = 0;
      //io.vdc_status|=VDC_InVBlank;
      if (VBlankON)
	{
	  //TRACE("vsync=%d\n", scanline);
	  ret = INT_IRQ;
	}
    }
  if (ret == INT_IRQ)
    {
      if (!(io.irq_mask & IRQ1))
	{
	  io.irq_status |= IRQ1;
	  return ret;
	}
    }
  return INT_NONE;
}

UChar
TimerInt ()
{
  if (io.timer_start)
    {
      io.timer_counter--;
      if (io.timer_counter > 128)
	{
	  io.timer_counter = io.timer_reload;

	  if (!(io.irq_mask & TIRQ))
	    {
	      io.irq_status |= TIRQ;
	      return INT_TIMER;
	    }

	}
    }
  return INT_NONE;
}

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

#ifdef EXTERNAL_DAT

#define LOAD_INTEGRATED_SYS_FILE ROM_size = 48; ROM = malloc(48*0x2000 + 512 ); memcpy(ROM,datafile[Built_in_cdsystem].dat,48*0x2000 + 512); ROM += 512; builtin_system_used=1; return 0;

#else

#define LOAD_INTEGRATED_SYS_FILE ROM_size = 48; ROM = malloc(48*0x2000 + 512 ); memcpy(ROM,data[Built_in_cdsystem].dat,48*0x2000 + 512); ROM += 512; builtin_system_used=1; return 0;

#endif

#else

// #define LOAD_INTEGRATED_SYS_FILE ROM_size = 32; ROM = malloc(32 * 0x2000); {FILE* f = fopen("syscard.pce","rb");fread(ROM,32,0x2000,f); fclose(f);}; builtin_system_used = 1;

#define LOAD_INTEGRATED_SYS_FILE return search_syscard();

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

/*****************************************************************************

    Function: search_syscard

    Description: Search for a system card rom
    Parameters: none
    Return: -1 on error else 0
             set true_file_name

*****************************************************************************/
SInt32
search_syscard()
{
	#define POSSIBLE_LOCATION_COUNT 4
	const char* POSSIBLE_LOCATION[POSSIBLE_LOCATION_COUNT] = {
		"./","../","/usr/local/lib/hugo/","/usr/lib/hugo/"
	};
	
	#define POSSIBLE_FILENAME_COUNT  4
	const char* POSSIBLE_FILENAME[POSSIBLE_FILENAME_COUNT] = {
		"syscard.pce","syscard3.pce","syscard30.pce","cd-rom~1.pce"
	};
	
	int location, filename;
	char temp_buffer[PATH_MAX];
	
	for (location = 0; location < POSSIBLE_LOCATION_COUNT; location++)
		for (filename = 0; filename < POSSIBLE_FILENAME_COUNT; filename++)
		{
			FILE* f;
			strcpy(temp_buffer, POSSIBLE_LOCATION[location]);
			strcat(temp_buffer, POSSIBLE_FILENAME[filename]);
			Log("Testing syscard location : %s\n",temp_buffer);
			if ((f = fopen(temp_buffer,"rb")) != NULL)
			{
				int CD_emulation_bak = CD_emulation;
				int return_value;
				
				fclose(f);
				
				return_value = CartLoad(temp_buffer);
				
				CD_emulation = CD_emulation_bak;
				
				return return_value;
			}
		}
		
	return -1;
}

/*****************************************************************************

    Function: CartLoad

    Description: load a card
    Parameters: char* name (the filename to load)
    Return: -1 on error else 0
                 set true_file_name or builtin_system

*****************************************************************************/
SInt32
CartLoad (char *name)
{
  FILE *fp;
  int fsize;
  char tmp_path[80];

  if (CD_emulation == 1)
    {

/*
 *       CD_emulation = 0;
 *
 *       CartLoad("h:/jeu/pce/cd_ge_93.pce");
 *
 *       CD_emulation = 1;
 *
 *       return 0;
 */
      LOAD_INTEGRATED_SYS_FILE;

    }

  if (strcasestr (name, ".HCD"))
    {

      // Enable Hu-Go! Cd Definition
      CD_emulation = 5;

      Log ("HCD emulation enabled\n");

      // Load correct ISO filename
      strcpy (ISO_filename, name);

      if (!fill_HCD_info (name))
	return 1;

      LOAD_INTEGRATED_SYS_FILE;

    }
  else if (strcasestr (name, ".ISO"))
    {

      // Enable ISO support
      CD_emulation = 2;

      // Load correct ISO filename
      strcpy (ISO_filename, name);
		
      LOAD_INTEGRATED_SYS_FILE;
    }
  else if (strcasestr (name, ".ISQ"))
    {

      // Enable ISQ support
      CD_emulation = 3;

      // Load correct ISO filename
      strcpy (ISO_filename, name);

      LOAD_INTEGRATED_SYS_FILE;
    }
  else if (strcasestr (name, ".BIN"))
    {

      // Enable BIN support
      CD_emulation = 4;

      // Load correct ISO filename
      strcpy (ISO_filename, name);

      LOAD_INTEGRATED_SYS_FILE;
    }
  else if (strcasestr (name, ".ZIP"))
    {
#ifdef MSDOS		
      char tmp_char[128], tmp_char2[128], tmp_char3[128];
      char *array_arg[6] =
	{ tmp_char3, tmp_char, "-Cjoqq", tmp_char2, "*.pce", NULL };

      sprintf (tmp_char, "%sHU-GO!.TMP/*.*", short_exe_name);
      for_each_file (tmp_char, 32, delete_file_tmp, 0);

      sprintf (tmp_char, "-d%sHU-GO!.TMP", short_exe_name);
      sprintf (tmp_char2, "%s", name);
      sprintf (tmp_char3, "%sHU-GO!.TMP/REDIR.RDF", short_exe_name);

      DecompressArchive (5, array_arg);


      if (!strcmp (name_to_extract, ""))
	sprintf (tmp_path, "%sHU-GO!.TMP/%sPCE", short_exe_name,
		 short_cart_name);
      else
	sprintf (tmp_path, "%sHU-GO!.TMP/%s", short_exe_name,
		 name_to_extract);

#endif	  
	  
      strcpy (true_file_name, tmp_path);
      fp = fopen (tmp_path, "rb");
    }
  else
    {

      CD_emulation = 0;
      strcpy (true_file_name, name);
      fp = fopen (name, "rb");
    }

  if (fp == NULL)
    {

      if (!check_char (name, '.'))	//if dot omitted, we try with PCE extension
	{
	  strcat (name, ".pce");
	  return CartLoad (name);
	};

      if (strcasestr (name, ".pce"))	//if filename with .PCE doesn't exist, it may be in ZIP
	{
	  strcpy (&name[strlen (name) - 4], ".ZIP");
	  return CartLoad (name);
	};

      return -1;
    }

  strcpy (cart_name, name);

  // find file size
  fseek (fp, 0, SEEK_END);
  fsize = ftell (fp);

  // ajust var if header present
  fseek (fp, fsize & 0x1fff, SEEK_SET);
  fsize &= ~0x1fff;

  // read ROM
  ROM = (UChar *) malloc (fsize);
  ROM_size = fsize / 0x2000;
  fread (ROM, 1, fsize, fp);

  fclose (fp);

#if defined(TEST_ROM_RELOCATED)
	printf("ROM = %x %x %x %x ....\n",ROM[0],ROM[1],ROM[2],ROM[3]);
#endif
	
  return 0;
}


#ifndef KERNEL_DS
int
ResetPCE (M6502 * M)
{
  int i;

  memset (M, 0, sizeof (*M));
  memset (SPRAM, 0, 64 * 8);

  TimerCount = TimerPeriod;
  M->IPeriod = IPeriod;
  M->TrapBadOps = 1;
  memset (&io, 0, sizeof (IO));
  scanline = 0;
  io.vdc_status = 0;
  io.vdc_inc = 1;
  io.minline = 0;
  io.maxline = 255;
  io.irq_mask = 0;
  io.psg_volume = 0;
  io.psg_ch = 0;

/* TEST */
  io.screen_w = 255;
/* TEST */// normally 256

/* TEST */
//   io.screen_h = 214;
/* TEST */

/* TEST */
//      io.screen_h = 240;
/* TEST */

  io.screen_h = 224;

  {
    UInt32 x, y = (WIDTH - io.screen_w) / 2 - 512 * WIDTH;
    for (x = 0; x < 1024; x++)
      {
	spr_init_pos[x] = y;
	y += WIDTH;
      }
    //pos = WIDTH*(HEIGHT-FC_H)/2+(WIDTH-FC_W)/2+WIDTH*y+x;
  }

  for (i = 0; i < 6; i++)
    {
      io.PSG[i][4] = 0x80;
    }
  CycleOld = 0;
  Reset6502 (M);

  if (debug_on_beginning)
    {

      Bp_list[GIVE_HAND_BP].position = M->PC.W;

      Bp_list[GIVE_HAND_BP].original_op = Op6502 (M->PC.W);

      Bp_list[GIVE_HAND_BP].flag = ENABLED;

      _Wr6502 (M->PC.W, 0xB + 0x10 * GIVE_HAND_BP);

    }

  if (((CD_emulation >= 2) && (CD_emulation <= 5))
      && (!strcmp (ISO_filename, "")))
    CD_emulation = 0;		// if no ISO name given, give up the emulation

  if ((CD_emulation == 2) || (CD_emulation == 4))
    {

      if (!(iso_FILE = fopen (ISO_filename, "rb")))
	{
	  sprintf (exit_message, MESSAGE[language][iso_file_not_found],
		   ISO_filename);
	  return 1;
	}

      fill_cd_info ();

    }
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
  else if (CD_emulation == 3)
    {

      if (!(packed_iso_FILE = pack_fopen (ISO_filename, F_READ_PACKED)))
	{
	  sprintf (exit_message, MESSAGE[language][iso_file_not_found],
		   ISO_filename);
	  return 1;
	}

      packed_iso_filesize = 0;
      while (!pack_feof (packed_iso_FILE))
	{
	  pack_getc (packed_iso_FILE);
	  packed_iso_filesize++;
	}

      Log ("packed filesize is %d\n", packed_iso_filesize);

      pack_fclose (packed_iso_FILE);
      packed_iso_FILE = pack_fopen (ISO_filename, F_READ_PACKED);

      ISQ_position = 0;

    }
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

  Log ("Cd_emulation is %d\n", CD_emulation);

  if (CD_emulation)
    {
      // We set illegal opcodes to handle CD Bios functions
      UInt16 x;

      Log ("Will hook cd functions\n");

      if (!minimum_bios_hooking)
	for (x = 0x01; x < 0x4D; x++)
	  if (x != 0x22)	// the 0x22th jump is special, points to a one byte routine
	    {
	      UInt16 dest;
	      dest = Op6502 (0xE000 + x * 3 + 1);
	      dest += 256 * Op6502 (0xE000 + x * 3 + 2);

	      CDBIOS_replace[x][0] = Op6502 (dest);
	      CDBIOS_replace[x][1] = Op6502 (dest + 1);

	      _Wr6502 (dest, 0xFC);
	      _Wr6502 (dest + 1, x);

	    }

    }
  return 0;
}
#else
int
ResetPCE ()
{
  int i;

  memset (SPRAM, 0, 64 * 8);

  TimerCount = TimerPeriod;
  memset (&io, 0, sizeof (IO));
  scanline = 0;
  io.vdc_status = 0;
  io.vdc_inc = 1;
  io.minline = 0;
  io.maxline = 255;
  io.irq_mask = 0;
  io.psg_volume = 0;
  io.psg_ch = 0;

  zp_base = RAM;
  sp_base = RAM + 0x100;

/* TEST */
  io.screen_w = 255;
/* TEST */// normally 256

/* TEST */
//   io.screen_h = 214;
/* TEST */

/* TEST */
//      io.screen_h = 240;
/* TEST */

  io.screen_h = 224;

  {
    UInt32 x, y = (WIDTH - io.screen_w) / 2 - 512 * WIDTH;
    for (x = 0; x < 1024; x++)
      {
	spr_init_pos[x] = y;
	y += WIDTH;
      }
    //pos = WIDTH*(HEIGHT-FC_H)/2+(WIDTH-FC_W)/2+WIDTH*y+x;
  }

  for (i = 0; i < 6; i++)
    {
      io.PSG[i][4] = 0x80;
    }
  CycleOld = 0;
	
#if !defined(TEST_ROM_RELOCATED)
  mmr[7] = 0x00;
  bank_set (7, 0x00);
#else
  mmr[7] = 0x68;
  bank_set (7, 0x68);	
#endif
	
  mmr[6] = 0x05;
  bank_set (6, 0x05);

  mmr[5] = 0x04;
  bank_set (5, 0x04);

  mmr[4] = 0x03;
  bank_set (4, 0x03);

  mmr[3] = 0x02;
  bank_set (3, 0x02);

  mmr[2] = 0x01;
  bank_set (2, 0x01);

  mmr[1] = 0xF8;
  bank_set (1, 0xF8);

  mmr[0] = 0xFF;
  bank_set (0, 0xFF);

  reg_a = reg_x = reg_y = 0x00;
  reg_p = FL_TIQ;

  reg_s = 0xFF;

  reg_pc = Op6502 (VEC_RESET) + 256 * Op6502 (VEC_RESET + 1);

  CycleNew = 0;

  if (debug_on_beginning)
    {

      Bp_list[GIVE_HAND_BP].position = reg_pc;

      Bp_list[GIVE_HAND_BP].original_op = Op6502 (reg_pc);

      Bp_list[GIVE_HAND_BP].flag = ENABLED;

      _Wr6502 (reg_pc, 0xB + 0x10 * GIVE_HAND_BP);

    }

  if (((CD_emulation >= 2) && (CD_emulation <= 5))
      && (!strcmp (ISO_filename, "")))
    CD_emulation = 0;		// if no ISO name given, give up the emulation

  if ((CD_emulation == 2) || (CD_emulation == 4))
    {

      if (!(iso_FILE = fopen (ISO_filename, "rb")))
	{
	  sprintf (exit_message, MESSAGE[language][iso_file_not_found],
		   ISO_filename);
	  return 1;
	}

      fill_cd_info ();

    }
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
#ifdef ALLEGRO
  else if (CD_emulation == 3)
    {

      if (!(packed_iso_FILE = pack_fopen (ISO_filename, F_READ_PACKED)))
	{
	  sprintf (exit_message, MESSAGE[language][iso_file_not_found],
		   ISO_filename);
	  return 1;
	}

      packed_iso_filesize = 0;
      while (!pack_feof (packed_iso_FILE))
	{
	  pack_getc (packed_iso_FILE);
	  packed_iso_filesize++;
	}

      Log ("packed filesize is %d\n", packed_iso_filesize);

      pack_fclose (packed_iso_FILE);
      packed_iso_FILE = pack_fopen (ISO_filename, F_READ_PACKED);

      ISQ_position = 0;

    }
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

  Log ("Cd_emulation is %d\n", CD_emulation);

  if (CD_emulation)
    {
      // We set illegal opcodes to handle CD Bios functions
      UInt16 x;

      Log ("Will hook cd functions\n");
/* TODO : reenable minimum_bios_hooking when bios hooking rewritten */

      if (!minimum_bios_hooking)
	for (x = 0x01; x < 0x4D; x++)
	  if (x != 0x22)	// the 0x22th jump is special, points to a one byte routine
	    {
	      UInt16 dest;
	      dest = Op6502 (0xE000 + x * 3 + 1);
	      dest += 256 * Op6502 (0xE000 + x * 3 + 2);

	      CDBIOS_replace[x][0] = Op6502 (dest);
	      CDBIOS_replace[x][1] = Op6502 (dest + 1);

	      _Wr6502 (dest, 0xFC);
	      _Wr6502 (dest + 1, x);

	    }

    }
  return 0;
}
#endif


int
InitPCE (char *name, char *backmemname)
{
  int i = 0, ROMmask;
  unsigned long CRC;
  int dummy;
  char *tmp_dummy;

  if ((!strcmp (name, "")) && (CD_emulation != 1))
    return -1;

  if (CartLoad (name))
    return -1;

  if (!(tmp_dummy = (char *) (strrchr (cart_name, '/'))))
    tmp_dummy = &cart_name[0];
  else
    tmp_dummy++;

  memset (short_cart_name, 0, 80);
  while ((tmp_dummy[i]) && (tmp_dummy[i] != '.'))
    short_cart_name[i] = tmp_dummy[i++];

  if (strlen (short_cart_name))
    if (short_cart_name[strlen (short_cart_name) - 1] != '.')
      {
	short_cart_name[strlen (short_cart_name) + 1] = 0;
	short_cart_name[strlen (short_cart_name)] = '.';
      }

#warning homogeneize calls
#if defined(ALLEGRO)
  fix_filename_slashes (ISO_filename);
#else
	    #if defined(SDL)
	    osd_fix_filename_slashes(ISO_filename);
	  #else
	    #error unknown lib
	  #endif
#endif

  if (!(tmp_dummy = (char *) (strrchr (ISO_filename, '\\'))))
    tmp_dummy = &ISO_filename[0];
  else
    tmp_dummy++;


  memset (short_iso_name, 0, 80);
  i = 0;
  while ((tmp_dummy[i]) && (tmp_dummy[i] != '.'))
    short_iso_name[i] = tmp_dummy[i++];

  if (strlen (short_iso_name))
    if (short_iso_name[strlen (short_iso_name) - 1] != '.')
      {
	short_iso_name[strlen (short_iso_name) + 1] = 0;
	short_iso_name[strlen (short_iso_name)] = '.';
      }

#ifndef LINUX

  switch (CD_emulation)
    {
    case 0:
      sprintf (sav_path, "%sSAV/%sSAV", short_exe_name, short_cart_name);
      break;
    case 1:
      sprintf (sav_path, "%sSAV/CD_SAV", short_exe_name);
      break;
    case 2:
    case 3:
    case 4:
    case 5:
      sprintf (sav_path, "%sSAV/%sSVI", short_exe_name, short_iso_name);
      break;
    }

  Log ("Saved path is %s\n", sav_path);

#else

  {
    char home_directory[256];

    strcpy (home_directory, getenv ("HOME"));

    switch (CD_emulation)
      {
      case 0:
	sprintf (sav_path, "%s/.hugo/%ssav", home_directory, short_cart_name);
	break;
      case 1:
	sprintf (sav_path, "%s/.hugo/cd_sav", short_exe_name);
	break;
      case 2:
      case 3:
      case 4:
      case 5:
	sprintf (sav_path, "%s/.hugo/%ssvi", short_exe_name, short_iso_name);
	break;
      }

    Log ("Saved path is %s\n", sav_path);

  }

#endif

  if (!builtin_system_used)
    {

      CRC = CRC_file (true_file_name);
      /* I'm doing it only here 'coz cartload set
         true_file_name    */

      NO_ROM = 0xFFFF;

      for (dummy = 0; dummy < NB_ROM; dummy++)
	if (CRC == ROM_LIST[dummy].CRC)
	  NO_ROM = dummy;
    }
  else
    NO_ROM = 255;

  DMYROM = (UChar *) malloc (0x2000);
  memset (DMYROM, NODATA, 0x2000);

  WRAM = (UChar *) malloc (0x2000);
  memset (WRAM, 0, 0x2000);
  WRAM[0] = 0x48;		/* 'H' */
  WRAM[1] = 0x55;		/* 'U' */
  WRAM[2] = 0x42;		/* 'B' */
  WRAM[3] = 0x4D;		/* 'M' */
  WRAM[5] = 0xA0;		/* WRAM[4-5] = 0xA000, end of free mem ? */
  WRAM[6] = 0x10;		/* WRAM[6-7] = 0x8010, beginning of free mem ? */
  WRAM[7] = 0x80;

  VRAM = (UChar *) malloc (VRAMSIZE);
  memset (VRAM, 0, VRAMSIZE);

  VRAM2 = (UInt32 *) malloc (VRAMSIZE);
  memset (VRAM2, 0, VRAMSIZE);

  VRAMS = (UInt32 *) malloc (VRAMSIZE);
  memset (VRAMS, 0, VRAMSIZE);

  IOAREA = (UChar *) malloc (0x2000);
  memset (IOAREA, 0xFF, 0x2000);

  vchange = (UChar *) malloc (VRAMSIZE / 32);
  memset (vchange, 1, VRAMSIZE / 32);

  vchanges = (UChar *) malloc (VRAMSIZE / 128);
  memset (vchanges, 1, VRAMSIZE / 128);

#ifndef FINAL_RELEASE
  fprintf (stderr, "flags = %x\n", ROM_LIST[NO_ROM].flags);
#endif

  if ((NO_ROM != 0xFFFF) && (ROM_LIST[NO_ROM].flags & US_ENCODED))
    US_encoded_card = 1;


  if (US_encoded_card)
    {
      UInt32 x;
      UChar inverted_nibble[16] = { 0, 8, 4, 12,
	2, 10, 6, 14,
	1, 9, 5, 13,
	3, 11, 7, 15
      };

      for (x = 0; x < ROM_size * 0x2000; x++)
	{
	  UChar temp;

	  temp = ROM[x] & 15;

	  ROM[x] &= ~0x0F;
	  ROM[x] |= inverted_nibble[ROM[x] >> 4];

	  ROM[x] &= ~0xF0;
	  ROM[x] |= inverted_nibble[temp] << 4;

	}
    }

  PCM = (UChar *) malloc (0x10000);

  if (CD_emulation)
    {

      cd_extra_mem = (UChar *) malloc (0x10000);
      memset (cd_extra_mem, 0, 0x10000);

      cd_extra_super_mem = (UChar *) malloc (0x30000);
      memset (cd_extra_super_mem, 0, 0x30000);

      ac_extra_mem = (UChar *) malloc (0x200000);
      memset (ac_extra_mem, 0, 0x200000);

      cd_sector_buffer = (UChar *) malloc (0x2000);

      // cd_read_buffer = (UChar *)malloc(0x2000);

    }
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

  if ((NO_ROM != 0xFFFF) && (ROM_LIST[NO_ROM].flags & PINBALL_KEY))

    for (dummy = 0; dummy < 5; dummy++)
      {
	config[current_config].joy_mapping[dummy][J_II] = KEY_RSHIFT;
	config[current_config].joy_mapping[dummy][J_LEFT] = KEY_LSHIFT;
	config[current_config].joy_mapping[dummy][J_I] = KEY_STOP;
	config[current_config].joy_mapping[dummy][J_RIGHT] = KEY_Z;
      }

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

  if ((NO_ROM != 0xFFFF) && (ROM_LIST[NO_ROM].flags & TWO_PART_ROM))
    ROM_size = 0x30;
  // Used for example with Devil Crush 512Ko

  ROMmask = 1;
  while (ROMmask < ROM_size)
    ROMmask <<= 1;
  ROMmask--;

#ifndef FINAL_RELEASE
  fprintf (stderr, "ROMmask=%02X, ROM_size=%02X\n", ROMmask, ROM_size);
#endif

#if !defined(TEST_ROM_RELOCATED)
  
  for (i = 0; i < 0xF7; i++)
    {
      if (ROM_size == 0x30)
	{
	  switch (i & 0x70)
	    {
	    case 0x00:
	    case 0x10:
	    case 0x50:
	      ROMMap[i] = ROM + (i & ROMmask) * 0x2000;
	      break;
	    case 0x20:
	    case 0x60:
	      ROMMap[i] = ROM + ((i - 0x20) & ROMmask) * 0x2000;
	      break;
	    case 0x30:
	    case 0x70:
	      ROMMap[i] = ROM + ((i - 0x10) & ROMmask) * 0x2000;
	      break;
	    case 0x40:
	      ROMMap[i] = ROM + ((i - 0x20) & ROMmask) * 0x2000;
	      break;
	    }
	}
      else
	ROMMap[i] = ROM + (i & ROMmask) * 0x2000;
    }
#else
	  #warning TEST_ROM_RELOCATED is on !
	
	for (i = 0; i < ROM_size; i++)
		ROMMap[0x68 + i] = ROM + i * 0x2000;
	
#endif
	
//              ROMMap[i]=ROM+(i%ROM_size+i/ROM_size*0x10)*0x2000;
/*              if (((i&ROMmask)+i/(ROMmask+1)) < ROM_size)
                        ROMMap[i]=ROM+((i&ROMmask)+i/(ROMmask+1)*0x20)*0x2000;
                else
                        ROMMap[i]=ROM;
*///            ROMMap[i]=ROM+(i&ROMmask)*0x2000;

  if (NO_ROM != 0xFFFF)
    {
#ifndef FINAL_RELEASE
      fprintf (stderr, "ROM NAME : %s\n", ROM_LIST[NO_ROM].name);
#endif
      osd_gfx_set_message (ROM_LIST[NO_ROM].name);
      message_delay = 60 * 5;
    }
  else
    {
      osd_gfx_set_message (MESSAGE[language][unknown_rom]);
      message_delay = 60 * 5;
    }

  if ((NO_ROM != 0xFFFF) && (ROM_LIST[NO_ROM].flags & POPULOUS))
    {
      populus = TRUE;
#ifndef FINAL_RELEASE
      fprintf (stderr, "POPULOUS DETECTED!!!\n");
#endif
      if (!(PopRAM = (UChar *) malloc (PopRAMsize)))
	perror (MESSAGE[language][no_mem]);
      ROMMap[0x40] = PopRAM;
      ROMMap[0x41] = PopRAM + 0x2000;
      ROMMap[0x42] = PopRAM + 0x4000;
      ROMMap[0x43] = PopRAM + 0x6000;
    }
  else
    {
      populus = FALSE;
      PopRAM = NULL;
    }

  if (CD_emulation)
    {

      ROMMap[0x80] = cd_extra_mem;
      ROMMap[0x81] = cd_extra_mem + 0x2000;
      ROMMap[0x82] = cd_extra_mem + 0x4000;
      ROMMap[0x83] = cd_extra_mem + 0x6000;
      ROMMap[0x84] = cd_extra_mem + 0x8000;
      ROMMap[0x85] = cd_extra_mem + 0xA000;
      ROMMap[0x86] = cd_extra_mem + 0xC000;
      ROMMap[0x87] = cd_extra_mem + 0xE000;

      for (i = 0x68; i < 0x80; i++)
	ROMMap[i] = cd_extra_super_mem + 0x2000 * (i - 0x68);

    }

  ROMMap[0xF7] = WRAM;
  ROMMap[0xF8] = RAM;
  ROMMap[0xF9] = RAM + 0x2000;
  ROMMap[0xFA] = RAM + 0x4000;
  ROMMap[0xFB] = RAM + 0x6000;
  ROMMap[0xFF] = IOAREA;	//NULL; /* NULL = I/O area */

  {
    FILE *fp;
    fp = fopen (backmemname, "rb");

    if (fp == NULL)
      fprintf (stderr, "Can't open %s\n", backmemname);
    else
      {
	fread (WRAM, 0x2000, 1, fp);
	fclose (fp);
      }

  }

  if ((NO_ROM != 0xFFFF) && ROM_LIST[NO_ROM].flags & CD_SYSTEM)
    {
      UInt16 offset;
      UChar new_val;

      offset = atoi (ROM_LIST[NO_ROM].note);
      new_val = atoi (&ROM_LIST[NO_ROM].note[6]);

      if (offset)
	ROMMap[0xE1][offset & 0x1fff] = new_val;

    }

  return 0;
}


#ifndef KERNEL_DS
int
RunPCE (void)
{
  if (!ResetPCE (&M))
    Run6502 ();
  return 1;
}
#else
int
RunPCE (void)
{
  if (!ResetPCE ())
    exe_go ();
  return 1;
}
#endif

void
TrashPCE (char *backmemname)
{
  FILE *fp;
  char *tmp_buf = (char *) alloca (256);

  if (!(fp = fopen (backmemname, "wb")))
    {
      memset (WRAM, 0, 0x2000);
      Log ("Can't open %s for saving RAM\n", backmemname);
    }
  else
    {
      fwrite (WRAM, 0x2000, 1, fp);
      fclose (fp);
      Log ("%s used for saving RAM\n", backmemname);
    }	
	
#if defined(ALLEGRO)	
  sprintf (tmp_buf, "%s/HU-GO!.TMP/*.*", short_exe_name);
  for_each_file (tmp_buf, 32, delete_file_tmp, 0);
#else
	#if defined(LINUX)
		sprintf (tmp_buf, "rm -rf %s/HU-GO!.TMP/*", short_exe_name);
	    system(tmp_buf);
	#endif
#endif
  sprintf (tmp_buf, "%s/HU-GO!.TMP", short_exe_name);
  rmdir (tmp_buf);

  if (CD_emulation == 1)
    osd_cd_close ();

  if ((CD_emulation == 2) || (CD_emulation == 4))
    fclose (iso_FILE);

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
  if (CD_emulation == 3)
    pack_fclose (packed_iso_FILE);
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

  if (CD_emulation == 5)
    HCD_shutdown ();

  if (IOAREA)
    free (IOAREA);
  if (vchange)
    free (vchange);
  if (vchanges)
    free (vchanges);
  if (DMYROM)
    free (DMYROM);
  if (WRAM)
    free (WRAM);
  if (VRAM)
    free (VRAM);
  if (VRAM2)
    free (VRAM2);
  if (VRAMS)
    free (VRAMS);
  if (ROM)
    free (ROM);
  if (PopRAM)
    free (PopRAM);
  if (PCM)
    free (PCM);

  if (CD_emulation)
    {

      if (cd_extra_mem)
	free (cd_extra_mem);
      if (cd_sector_buffer)
	free (cd_sector_buffer);
      if (cd_extra_super_mem)
	free (cd_extra_super_mem);
      if (cd_buf)
	free (cd_buf);

    }

  return;
};

#ifdef CHRONO
unsigned nb_used[256], time_used[256];
#endif

#ifndef FINAL_RELEASE
extern int mseq (unsigned *);
extern void mseq_end ();
extern void WriteBuffer_end ();
extern void write_psg_end ();
extern void WriteBuffer (char *, int, unsigned);
#endif

FILE *out_snd;

int
main (int argc, char *argv[])
{
  char backup_mem[80];
  char _BACKUP_DAT[] = "BACKUP.DAT";
  // Default name if none given

  char i, tmp_path[80];

#ifdef CHRONO
  unsigned timax = 0, inst_max = 0;

  for (vmode = 0; vmode < 256; vmode++)
    time_used[vmode] = nb_used[vmode] = 0;

#endif

#ifdef LINUX
	{
		char* home_path;
		
		home_path = getenv("HOME");
		
		if (home_path)
		{			
			sprintf(short_exe_name,"%s/.hugo/",home_path);
			mkdir(short_exe_name,0777);
		  	sprintf(log_filename,"%s%s",short_exe_name,LOG_NAME);		
		}
		else
		{
			strcpy(short_exe_name,"./");
		  	strcpy(home_path,LOG_NAME);
		}
	}
#endif

#ifdef WIN32

	strcpy(log_filename,"c:\\hugo.log");
	
#endif
	
#ifdef MSDOS

  _crt0_startup_flags |= _CRT0_FLAG_NO_LFN;
  // Disable long filename to avoid mem waste in select_rom func.

#endif
		
  init_log_file ();  

  srand (time (NULL));

  parse_INIfile ();	
	
#warning check if ALLEGRO is ok with initializing the machine here  
	
  if (!osd_init_machine ())
    return -1;
    
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
#if defined(ALLEGRO)
#else
	#if defined(SDL)
	
	#endif
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

#ifndef LINUX

  //  get_executable_name (short_exe_name, 256);
  strncpy(short_exe_name,argv[0],PATH_MAX);
  for (i = 0; short_exe_name[i]; i++)
    if (short_exe_name[i] == '\\')
      short_exe_name[i] = '/';

  if (strrchr (short_exe_name, '/'))
    *(char *) (strrchr (short_exe_name, '/') + 1) = 0;
  // add a trailing slash

#else

  strcpy (short_exe_name, "./");

#endif

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
#if defined(EXTERNAL_DAT) && defined(ALLEGRO)

#ifndef LINUX
  strcpy (tmp_path, short_exe_name);
  strcat (tmp_path, "HU-GO!.DAT");
#else
  strcpy (tmp_path, "/etc/hugo.dat");
#endif
  printf (" þ Decrunching data file...");
  if (!(datafile = load_datafile (tmp_path)))
    {
      printf ("\n þ ERROR!!\n þ Datafile %s not found\n", tmp_path);
      exit (-1);
    }

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

  strcpy (tmp_path, short_exe_name);
  strcat (tmp_path, "HU-GO!.TMP");

#ifndef WIN32
  mkdir (tmp_path, 0);
#else
  mkdir (tmp_path);
#endif

#ifdef ALLEGRO

#if defined(MSDOS) || defined(WIN32)

  set_gfx_mode (GFX_AUTODETECT, 320, 200, 0, 0);

#elif defined(LINUX)

#if defined(ALLEGRO)
  if (set_gfx_mode (GFX_SAFE, 320, 200, 0, 0))
    {
      printf ("Error setting mode!\n");
      getchar ();
      exit (-2);
    }

#endif // ALLEGRO	
	
#endif

#endif // ALLEGRO

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

#if defined(EXTERNAL_DAT)

  set_palette (datafile[INTRO_PAL].dat);


  /*
     (*fade_in_proc[rand () % nb_fadein]) (datafile[INTRO_PICTURE].dat, 0, 0,
     320, 200);
   */
  // Now the logo is displayed, let's be useful instead of wait

#else

  fixup_datafile (data);

  set_palette (data[INTRO_PAL].dat);

# ifndef LINUX

  (*fade_in_proc[rand () % nb_fadein]) (data[INTRO_PICTURE].dat, 0, 0, 320,
					200);
  // Now the logo is displayed, let's be useful instead of waiting

#else

  // blit(data[INTRO_PICTURE].dat, screen, 0, 0, 0, 0, 320, 200);
  {

    PALETTE dum_pal;
    BITMAP *dum = load_bitmap ("./hugo3.bmp", dum_pal);
    (*fade_in_proc[rand () % nb_fadein]) (dum, 0, 0, 320, 200);
    destroy_bitmap (dum);

  }
#endif
#endif
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

  parse_commandline (argc, argv);

  if (!bmdefault)
    bmdefault = _BACKUP_DAT;

  strcpy (backup_mem, short_exe_name);
  strcat (backup_mem, bmdefault);

#ifndef LINUX
  sprintf (sav_path, "%sSAV/", short_exe_name);
  sprintf (video_path, "%sVIDEO/", short_exe_name);
#else
  {
    char tmp_home[256];
    strcpy (tmp_home, getenv ("HOME"));

    sprintf (sav_path, "%s/.hugo", tmp_home);
  }
#endif

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
  /* TODO: make this allegro independant */
#ifdef ALLEGRO
  if (!file_exists (sav_path, FA_DIREC, 0))
#ifndef WIN32
    mkdir (sav_path, 0);
#else
    mkdir (sav_path);
#endif
  // Create a place for saved games if not exist
#endif

  /* TODO: make this allegro independant */
#ifdef ALLEGRO
  if (!file_exists (video_path, FA_DIREC, 0))
#ifndef WIN32
    mkdir (video_path, 0);
#else
    mkdir (video_path);
#endif
  // Create a place for output images if not exist
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


  atexit (TrashSound);

  // In case of crash, try to free as many resources as we can

  //  getchar ();

  // (*fade_out_proc[rand () % nb_fadeout]) (0, 0, 320, 200);

  BaseClock = 7800000;		//7160000; //3.58-21.48;
  //  7.8 Mhz  ^

#if defined(ALLEGRO)
  LOCK_VARIABLE (key_delay);
  LOCK_VARIABLE (timer_60);
  LOCK_VARIABLE (can_blit);
  LOCK_VARIABLE (RAM);
  LOCK_VARIABLE (list_to_freeze);
  LOCK_FUNCTION (interrupt_60hz);
#endif

  IPeriod = BaseClock / (scanlines_per_frame * 60);
#ifndef FINAL_RELEASE
  fprintf (stderr, "IPeriod = %d\n", IPeriod);
#endif
//      UPeriod = 0;
  // TimerPeriod = BaseClock / 1000 * 3 * 1024 / 21480;
#ifndef FINAL_RELEASE
  fprintf (stderr, "TimerPeriod = %d\n", TimerPeriod);
#endif

/* TEST */
  io.screen_h = 224;
/* TEST */
  io.screen_w = 256;

/*
  if (!osd_init_machine ())
    return -1;
*/

  do
    {

#warning reenable card selection without allegro
#if defined(ALLEGRO)
      if ((!cart_name[0]) && (CD_emulation != 1))
	strcpy (cart_name, (char *) select_rom ("*.pce"));
#endif
	  
      if (strcmp (cart_name, "NO FILE"))
	if (!InitPCE (cart_name, backup_mem))
	  {
	    effectively_played = 1;
	    cart_reload = 0;
		
		#if defined(ALLEGRO)
	    install_int_ex (interrupt_60hz, BPS_TO_TIMER (60));
		#endif

	    RunPCE ();

		#if defined(ALLEGRO)
	    remove_int (interrupt_60hz);
		#endif

	    TrashPCE (backup_mem);
	  }
    }
  while (cart_reload);

  /* TrashMachine (); */
  osd_shut_machine ();

  if (effectively_played)
    {
      if (builtin_system_used)
	printf ("");
      else if (NO_ROM < NB_ROM)
	printf (MESSAGE[language][played], ROM_LIST[NO_ROM].name);
      else
	printf (MESSAGE[language][unknown_contact_me]);
    }
  else
    printf (MESSAGE[language][C_ya]);

#ifdef CHRONO
  if (!(F = fopen ("RESULT.DAT", "wt+")))
    return -1;
  for (vmode = 0; vmode < 256; vmode++)
    {
      fprintf (F,
	       "Inst 0X%02X : %10u calls, %10u (*840)ns => avr. of %10u (*840)ns\n",
	       vmode, nb_used[vmode], time_used[vmode],
	       (nb_used[vmode] ? time_used[vmode] / nb_used[vmode] : 0));
      if (nb_used[vmode] >= timax)
	{
	  inst_max = vmode;
	  timax = nb_used[vmode];
	}
    };
  fprintf (F,
	   "\nGreat Winner is inst 0X%02X with %10u calls and %10u (*840)ns elapsed => average of %10u",
	   inst_max, nb_used[inst_max], time_used[inst_max],
	   time_used[inst_max] / nb_used[inst_max]);
  fclose (F);

#endif

  fprintf (stderr, exit_message);

  if (timer_60)
    {
      fprintf (stderr, MESSAGE[language][time_elapsed], (timer_60 / 60.0));
      fprintf (stderr, MESSAGE[language][frame_per_sec],
	       frame / (timer_60 / 60.0));
    }

  if (dump_snd)
    fclose (out_snd);
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
#if defined(EXTERNAL_DAT) && defined(ALLEGRO)
  Log ("I'll unload datafile, @ = %p\n", datafile);
//      if (datafile)
//      unload_datafile(datafile);
  Log ("I've unloaded datafile\n");
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

  Log
    ("\n--[ END OF PROGRAM ]----------------------------------\nExecution completed successfully\n");
  return 0;
}

#ifdef ALLEGRO

END_OF_MAIN ();

#endif

#include "osd_cd.h"
#if defined(ALLEGRO)
static int HaId;
static int Target;
static int Lun;
static int aspi_init = 0;

DWORD (*pfnGetASPI32SupportInfo) (void);
DWORD (*pfnSendASPI32Command) (LPSRB);

typedef struct
{
  BYTE rsvd;
  BYTE ADR;
  BYTE trackNumber;
  BYTE rsvd2;
  BYTE addr[4];
}
PACKED TOCTRACK;

typedef struct
{
  WORD tocLen;
  BYTE firstTrack;
  BYTE lastTrack;
  TOCTRACK tracks[100];
}
PACKED TOC, *PTOC, FAR * LPTOC;

static TOC toc;

DWORD
ReadToc ()
{

  SRB_ExecSCSICmd s;
  HANDLE hEvent;
  DWORD dwStatus;

 if (aspi_init == 0)
   loadASPI();

  Log("Reading TOC\n");

  hEvent = CreateEvent (NULL, TRUE, FALSE, NULL);

	  memset (&s, 0, sizeof (s));
	  memset (&toc, 0, sizeof (toc));
	  s.SRB_Cmd = SC_EXEC_SCSI_CMD;


	  s.SRB_HaID = HaId;
	  s.SRB_Target = Target;
	  s.SRB_Lun = Lun;
	  s.SRB_Flags = SRB_DIR_IN | SRB_EVENT_NOTIFY;
	  s.SRB_BufLen = 0x324;
	  s.SRB_BufPointer = (BYTE FAR *) & toc;
	  s.SRB_SenseLen = 0x0E;
	  s.SRB_CDBLen = 0x0A;
	  s.SRB_PostProc = (LPVOID) hEvent;
	  s.CDBByte[0] = 0x43;	// command to read TOC
	  s.CDBByte[7] = 0x03;	// ofs. 7-8 used for toc len
	  s.CDBByte[8] = 0x24;	// TOC buffer length == 0x324

	  dwStatus = pfnSendASPI32Command ((LPSRB) & s);

          if (dwStatus == SS_PENDING)
	    {
	      WaitForSingleObject (hEvent, INFINITE);
	    }

          CloseHandle(hEvent);

          return s.SRB_Status;
 }

DWORD
ReadDevice (int H,
            int T,
            int L)
{

  SRB_GDEVBlock s;
  HANDLE hEvent;
  DWORD dwStatus;

  if (aspi_init == 0)
   loadASPI();

  hEvent = CreateEvent (NULL, TRUE, FALSE, NULL);

	  memset (&s, 0, sizeof (s));

          s.SRB_Cmd = SC_GET_DEV_TYPE;

          s.SRB_HaID = H;
          s.SRB_Target = T;
          s.SRB_Lun = L;

	  dwStatus = pfnSendASPI32Command ((LPSRB) & s);

          if (dwStatus == SS_PENDING)
	    {
	      WaitForSingleObject (hEvent, INFINITE);
              return -1;
	    }

          CloseHandle(hEvent);

          if (dwStatus == SS_COMP)
            return s.SRB_DeviceType;

          return -1;
 }


int
loadASPI (void)
{
  HINSTANCE hDll;

  hDll = LoadLibrary ("WNASPI32.DLL");
  if (!hDll)
    return 0;

  pfnGetASPI32SupportInfo =
    (DWORD (*)(void)) GetProcAddress (hDll, "GetASPI32SupportInfo");
  pfnSendASPI32Command =
    (DWORD (*)(LPSRB)) GetProcAddress (hDll, "SendASPI32Command");

  if (!pfnGetASPI32SupportInfo || !pfnSendASPI32Command)
    {
      Log("Win Aspi NOT initiated\n");
      return 0;
    }

  aspi_init = 1;

  Log("Win Aspi initiated\n");

  return -1;
}

void
MSB2DWORD (DWORD * d, BYTE * b)
{
  DWORD retVal;

  retVal = (DWORD) b[0];
  retVal = (retVal << 8) + (DWORD) b[1];
  retVal = (retVal << 8) + (DWORD) b[2];
  retVal = (retVal << 8) + (DWORD) b[3];

  *d = retVal;
}

int osd_cd_init(char* device)
{

 int HaId_in, Target_in, Lun_in;
 int nb = atoi(device);

 Log("Init win aspi cdrom device\n");

 if (nb == 0)
   nb = 1;

/*
 if (loadASPI())
   return 1;
*/

/*
 Log("Trying to check cd driver number %d\n", nb);
*/

 loadASPI();


 Lun = HaId = 0;
 Target = 1;

 return 0;


 for (HaId_in = 0; HaId_in < 8; HaId_in++)
    for (Target_in = 0; Target_in < 8; Target_in++)
       for (Lun_in = 0; Lun_in < 8; Lun_in++)
          {
/*
          Log("Trying %d:%d:%d\n",
                      HaId_in,
                      Target_in,
                      Lun_in);
*/
          if (ReadDevice(HaId_in,
                         Target_in,
                         Lun_in) != 5)
            continue;
/*
          Log("Found 1 CD...\n");
*/
          if (--nb)
            continue;

	  Log ("CD found at %d:%d:%d\n", HaId_in, Target_in, Lun_in);

          HaId = HaId_in;
          Target = Target_in;
          Lun = Lun_in;

          return 0;

          }

    return 1;  
}

void osd_cd_close()
{
}

int osd_cd_read(UChar* p, UInt32 sector)
{
  HANDLE hEvent;
  SRB_ExecSCSICmd s;
  DWORD dwStatus;
  BYTE* intern_buffer = (BYTE*)alloca(2352);

 if (aspi_init == 0)
   loadASPI();

		  hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );

		  memset (&s, 0, sizeof (s));
		  s.SRB_Cmd = SC_EXEC_SCSI_CMD;
		  s.SRB_HaID = HaId;
		  s.SRB_Target = Target;
		  s.SRB_Lun = Lun;
		  s.SRB_Flags = SRB_DIR_IN | SRB_EVENT_NOTIFY;
		  s.SRB_BufLen = 2352;
		  s.SRB_BufPointer = intern_buffer;
		  s.SRB_SenseLen = SENSE_LEN;
		  s.SRB_CDBLen = 12;
		  s.SRB_PostProc = (LPVOID) hEvent;


		  s.CDBByte[0] = 0xBE;	/* read CD Raw */
                  s.CDBByte[3] = (sector >> 16) & 0xFF;
                  s.CDBByte[4] = (sector >> 8) & 0xFF;
                  s.CDBByte[5] = sector & 0xFF;

                  s.CDBByte[8] = 1;

		  s.CDBByte[9] = 0xF0;

		  ResetEvent (hEvent);
		  dwStatus = pfnSendASPI32Command ((LPSRB) & s);
		  if (dwStatus == SS_PENDING)
		    {
		      WaitForSingleObject (hEvent, INFINITE);
		    }

		  if (s.SRB_Status != SS_COMP)
                    return s.SRB_Status;

		  CloseHandle( hEvent );

                  memcpy(p, intern_buffer + 16, 2048);

                  return s.SRB_Status;


}

void osd_cd_stop_audio()
{
}

void osd_cd_track_info( UChar track,
			int* min,
			int* sec,
			int* fra,
			int* control)
{
 DWORD frame;

 if (aspi_init == 0)
   loadASPI();

 track--;

 ReadToc();

 *control = ((TOCTRACK)(toc.tracks[track])).ADR;

 MSB2DWORD( &frame, ((TOCTRACK)(toc.tracks[track])).addr );

 Frame2Time(frame + 150, min, sec, fra);

/*
 Log("Track begins at frame %d\n", frame + 150);
*/
}

void osd_cd_nb_tracks(int* first,
		      int* last)
{

 if (aspi_init == 0)
   loadASPI();

 ReadToc();

 Log("Will output tracks limit\n");

 *first = toc.firstTrack;
 *last = toc.lastTrack;

}

void osd_cd_length(int* min,
                   int* sec,
                   int* fra)
{
 *min = 25;
 *sec = 06;
 *fra = 00;
}

void osd_cd_play_audio_track(UChar track)
{
}

void osd_cd_play_audio_range(
			     UChar min_from,
			     UChar sec_from,
			     UChar fra_from,
			     UChar min_to,
			     UChar sec_to,
			     UChar fra_to)
{
}
#else

  #warning need to implement cd functions through SDL

int osd_cd_init(char* device)
{
  return 0;
}

void osd_cd_close()
{
}

int osd_cd_read(UChar* p, UInt32 sector)
{
  memset(p, 0, 2048);
}

void osd_cd_stop_audio()
{

}

void osd_cd_track_info(UChar track,
			int* min,
			int* sec,
			int* fra,
			int* control)
{

  *min = 79;
  *sec = 06;
  *fra = 25;
  *control = 0;
  
}

/* TODO : put some true code in it */
void osd_cd_nb_tracks(int* first,
		      int* last)
{
  *first = 1;
  *last = 22;
}

void osd_cd_length(int* min,
                   int* sec,
                   int* fra)
{
 *min = 25;
 *sec = 06;
 *fra = 00;
}

/* TODO : check for last track asked */
void osd_cd_play_audio_track(UChar track)
{

}

void osd_cd_play_audio_range(
			     UChar min_from,
			     UChar sec_from,
			     UChar fra_from,
			     UChar min_to,
			     UChar sec_to,
			     UChar fra_to)
{
  
}

  

#endif

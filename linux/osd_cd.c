#include "osd_cd.h"

static int cd_drive_handle = 0;

int osd_cd_init(char* device)
{
  Log("Init linux cdrom device\n");
  
  if (strcmp(device,""))
    cd_drive_handle = open(device, O_RDONLY | O_NONBLOCK);
  else
    cd_drive_handle = open("/dev/cdrom", O_RDONLY | O_NONBLOCK);
  
  return (cd_drive_handle == 0);
}

void osd_cd_close()
{
  osd_cd_stop_audio();
  close(cd_drive_handle);
}

int osd_cd_read(UChar* p, UInt32 sector)
{
  lseek(cd_drive_handle, 2048 * sector, SEEK_SET);
  read(cd_drive_handle ,p, 2048);
}

void osd_cd_stop_audio()
{
  ioctl(cd_drive_handle, CDROMSTOP);
}

void osd_cd_track_info(UChar track,
			int* min,
			int* sec,
			int* fra,
			int* control)
{
  struct cdrom_tocentry tocentry;
  
  tocentry.cdte_track = track;
  tocentry.cdte_format = CDROM_LBA;
  
  ioctl(cd_drive_handle, CDROMREADTOCENTRY, &tocentry);
  
  Log("Track %d begins at %d and got command byte of %d\n",
      track,
      tocentry.cdte_addr.lba + 150,
      tocentry.cdte_ctrl);
  
  Frame2Time(tocentry.cdte_addr.lba + 150,
	     min,
	     sec,
	     fra);
  
  *control = tocentry.cdte_ctrl;
  
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
  struct cdrom_ti cdrom_ti_dat = 
    {
      track,
      0,
      track + 1,
      0
    };
  
  ioctl(cd_drive_handle,
	CDROMPLAYTRKIND,
	&cdrom_ti_dat);
}

void osd_cd_play_audio_range(
			     UChar min_from,
			     UChar sec_from,
			     UChar fra_from,
			     UChar min_to,
			     UChar sec_to,
			     UChar fra_to)
{
  struct cdrom_msf cdrom_msf_dat =
    {
      min_from,
      sec_from,
      fra_from,
      min_to,
      sec_to,
      fra_to
    };
  
  ioctl(cd_drive_handle,
	CDROMPLAYMSF,
	&cdrom_msf_dat);
  
}


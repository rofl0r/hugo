#include "osd_cd.h"

int osd_cd_init(char* device)
{
  Log("Init generic cdrom device\n");

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


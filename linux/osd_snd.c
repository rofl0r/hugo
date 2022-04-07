#include "osd_snd.h"

#if defined(ALLEGRO)

 void osd_snd_set_volume(UChar v)
{
	set_volume(v);
}

#endif // ALLEGRO

#if defined(SDL)

void osd_snd_set_volume(UChar v)
{
	#warning implement set volume for sdl
}

#endif // SDL


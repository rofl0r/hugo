#include "osd_snd.h"

#if defined(ALLEGRO)

 void osd_snd_set_volume(UChar v)
{
	set_volume(v);
}

#endif // ALLEGRO

#if defined(SDL)

extern void sdl_fill_audio(void *data, Uint8 *stream, int len);

void osd_snd_set_volume(UChar v)
{
	#warning implement set volume for sdl
}

int osd_snd_init_sound ()
{
	SDL_AudioSpec wanted;
    wanted.freq = freq_int;
    wanted.format = AUDIO_U8;
    wanted.channels = 1;    /* 1 = mono, 2 = stereo */
    wanted.samples = 512;  /* Good low-latency value for callback */
    wanted.callback = sdl_fill_audio;
    wanted.userdata = main_buf;     /* Open the audio device, forcing the desired format */
	
    if ( SDL_OpenAudio(&wanted, NULL) < 0 ) {
        Log("Couldn't open audio: %s\n", SDL_GetError());
        return(0);
    }
    return(1);
}

void osd_snd_trash_sound ()
{
	SDL_CloseAudio();
}

#endif // SDL


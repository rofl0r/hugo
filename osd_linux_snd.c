#include "osd_linux_snd.h"

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

int osd_snd_init_sound(void)
{
	SDL_AudioSpec wanted, got;
  UInt16 i;

    wanted.freq = freq_int;
    wanted.format = AUDIO_U8;
    
    wanted.samples = sbuf_size;  /* Good low-latency value for callback */	
    wanted.channels = option.want_stereo + 1;
	
	wanted.callback = sdl_fill_audio;
    wanted.userdata = main_buf;     /* Open the audio device, forcing the desired format */
	
    if ( SDL_OpenAudio(&wanted, &got) < 0 ) {
        Log("Couldn't open audio: %s\n", SDL_GetError());
        return 0;
    }

  host.stereo_sound = (got.channels == 2);
  host.sample_size = got.samples;
	
    return(1);
}

void osd_snd_trash_sound ()
{
	SDL_CloseAudio();
}

#endif // SDL


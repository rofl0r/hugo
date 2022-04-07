/***************************************************************************/
/*                                                                         */
/*                         Sound Source File                               */
/*                                                                         */
/*     Initialisation, shuting down and PC Engine generation of sound      */
/*                                                                         */
/***************************************************************************/

/* Header */

#include "sound.h"

#include <SDL_audio.h>
SDL_AudioSpec wanted; /* For SDL Audio */
extern void sdl_fill_audio(void *data, Uint8 *stream, int len);

/* Variables definition */

UChar sound_driver = 1;
// 0 =-¯ No sound driver
// 1 =-¯ Allegro sound driver
// 2 =-¯ Seal sound driver

char MP3_playing = 0;
// is MP3 playing ?

char *sbuf[6];
// the buffer where the "DATA-TO-SEND-TO-THE-SOUND-CARD" go
// one for each channel

char *adpcmbuf;
// the buffer filled with adpcm data

UChar new_adpcm_play = 0;
// Have we begun a new adpcm sample (i.e. must be reset adpcm index/prev value)

unsigned char main_buf[SBUF_SIZE_BYTE];
// the mixed buffer, may be removed later for hard mixing...

UInt32 CycleOld;
UInt32 CycleNew;
// indicates the last time music has been "released"

/* TODO */
int BaseClock=7170000;
// int BaseClock = 8992000;
// the freq of the internal PC Engine CPU
// the sound use a kind of "relative" frequency
// I think there's a pb with this value that cause troubles with some cd sync

UInt32 ds_nChannels = 1;
// mono or stereo, to remove later

UInt32 dwNewPos;

UInt32 AdpcmFilledBuf = 0;
// Size (in nibbles) of adpcm buf that has been filled with new data

#ifdef DJGPP

// SEAL sound related section

HAC hVoice;

LPAUDIOWAVE lpWave;

// End of SEAL sound related section

#endif


#ifdef ALLEGRO

// ALLEGRO sound related section

SAMPLE *CD_sample = NULL;

AUDIOSTREAM *PCM_stream;


#endif

UChar *big_buf;

UChar gen_vol = 255;

UInt32 sbuf_size = 10 * 1024;

// End of ALLEGRO sound related section



/* Functions definition */

int
InitSound (void)
{

  sound_driver=3; // USE SDL!
  
  for (silent = 0; silent < 6; silent++)
    {
      sbuf[silent] = (char *) malloc (SBUF_SIZE_BYTE);
      memset (sbuf[silent], 0, SBUF_SIZE_BYTE);
    }

  adpcmbuf = (char *) malloc (SBUF_SIZE_BYTE);
  memset (adpcmbuf, 0, SBUF_SIZE_BYTE);

  silent = 1;

  if (smode == 0)		// No sound
    return TRUE;

/* SDL Audio Begin */
  if (sound_driver == 3)
    {
		/*
	  SDL_AudioSpec obtained;
      Log ("Initialisation of SDL sound... ");
      wanted.freq = freq_int; // Frequency
      printf("Frequency = %d\n", freq_int);
      wanted.format = AUDIO_U8; // Unsigned 8 bits
      wanted.channels = 1; // Mono
      wanted.samples = 512; //SBUF_SIZE_BYTE;
//      wanted.size = SBUF_SIZE_BYTE;
 //     printf("wanted.size = %d\n",wanted.size);
      wanted.callback = sdl_fill_audio;
      wanted.userdata = main_buf;//NULL;
      
      if (SDL_OpenAudio(&wanted,&obtained) < 0) {
        printf("Couldn't Open SDL Audio: %s\n",SDL_GetError());
	return FALSE;
      }
      Log ("OK\nObtained frequency = %d\n",obtained.freq);
      silent=0;
      SDL_PauseAudio(0);
	  */
	  silent = 0;
    }
/* End of SDL Audio */

#ifdef MSDOS
  if (sound_driver == 2)	// Seal Sound
    {

      Log ("Initialisation of SEAL sound\n");

      AInitialize ();
      // Init the SEAL library

      if (smode == -1)
	{
	  AUDIOCAPS caps;
	  UINT nDeviceId;

	  printf (MESSAGE[language][autodetect_mode]);

	  if (APingAudio (&nDeviceId) != AUDIO_ERROR_NONE)
	    {
	      printf (MESSAGE[language][no_device_found]);
	      return FALSE;
	    }
	  else
	    {
	      AGetAudioDevCaps (nDeviceId, &caps);
	      printf (MESSAGE[language][device_found], caps.szProductName);
	      silent = 0;
	    }
	}
      // Autodetection

      Log ("SEAL : sound card autodetected\n");

      // Now, we can really initialize the card
      {
	AUDIOINFO info;

	info.nDeviceId = AUDIO_DEVICE_MAPPER;
	info.wFormat = AUDIO_FORMAT_8BITS | AUDIO_FORMAT_MONO;
	info.nSampleRate = freq_int;
	if (AOpenAudio (&info) != AUDIO_ERROR_NONE)
	  {
	    printf (MESSAGE[language][audio_init_failed]);
	    exit (1);
	  }
	else
	  {
	    printf (MESSAGE[language][audio_inited],
		    info.wFormat & AUDIO_FORMAT_16BITS ? 16 : 8,
		    info.wFormat & AUDIO_FORMAT_STEREO ?
		    "stereo" : "mono", info.nSampleRate);
	  }
      }

      Log ("SEAL : sound card initiated\n");

      fflush (stdout);
      AOpenVoices (1);

      ACreateAudioVoice (&hVoice);
      ASetVoiceVolume (hVoice, 64);

      {
	/* first allocate structure to hold the waveform object */
	if ((lpWave = (LPAUDIOWAVE) malloc (sizeof (AUDIOWAVE))) != NULL)
	  {
	    /* create a 8-bit mono one-shot waveform object */
	    lpWave->wFormat =
	      AUDIO_FORMAT_8BITS | AUDIO_FORMAT_MONO | AUDIO_FORMAT_LOOP;
	    lpWave->nSampleRate = freq_int;
	    lpWave->dwLength = SBUF_SIZE_BYTE;
	    lpWave->dwLoopStart = 0;
#if defined(DOUBLE_BUFFER)
	    lpWave->dwLoopEnd = 398;
#else
	    lpWave->dwLoopEnd = lpWave->dwLength;
#endif
	    if (ACreateAudioData (lpWave) != AUDIO_ERROR_NONE)
	      {
		free (lpWave);
		return FALSE;
	      }
	    /* copy the data into the waveform object */
	    memset (lpWave->lpData, 0x80, SBUF_SIZE_BYTE);
	    /* upload the data to the audio DRAM local memory */
	    AWriteAudioData (lpWave, 0L, lpWave->dwLength);
	  }
      }

      APlayVoice (hVoice, lpWave);
    }
  else
#endif

#ifdef ALLEGRO

  if (sound_driver == 1)	// Allegro sound
    {

      printf (MESSAGE[language][autodetect_mode]);

      reserve_voices (2, 0);
      // reserve 2 digital voices and no midi

      Log ("Installing allegro driver\n");

      /* install a digital sound driver */
      if (install_sound (DIGI_AUTODETECT, MIDI_NONE, NULL))
	{
	  Log ("No sound card found\n");
	  printf (MESSAGE[language][no_device_found]);
	  return FALSE;
	}

      /* create an audio stream
         //stream = play_audio_stream(SBUF_SIZE_BYTE, 8, FALSE, 22050, 255, 128);
         PCM_sample = create_sample(8, 0, 11025, SBUF_SIZE_BYTE );
         if (!PCM_sample) {
         printf("Error creating sample!\n");
         return FALSE;
         }

         //   PCM_sample_b1 = PCM_sample->data;
         //   PCM_sample_b2 = PCM_sample->data + SBUF_SIZE_BYTE / 2;
         //   mid_pos = SBUF_SIZE_BYTE / 2;

         //   PCM_sample_lenght = SBUF_SIZE_BYTE;

         //   memset( PCM_sample_b1, SBUF_SIZE_BYTE, 0x80 );
         memset(PCM_sample->data, SBUF_SIZE_BYTE, 0x80);

         PCM_voice = allocate_voice(PCM_sample);
         if (PCM_voice < 0) {
         destroy_sample(PCM_sample);
         return FALSE;
         }

         voice_set_playmode(PCM_voice, PLAYMODE_LOOP);
         voice_set_volume(PCM_voice, 128);
         voice_set_pan(PCM_voice, 128);
         voice_start(PCM_voice);
       */

      big_buf = (unsigned char *) malloc (sbuf_size);

      set_volume (gen_vol, 0);

      PCM_stream =
	play_audio_stream (sbuf_size, 8, FALSE, freq_int, 128, 128);
      if (!PCM_stream)
	{
	  Log ("Error creating audio stream!\n");
	  return FALSE;
	}

#ifdef MSDOS
      install_amp ();
#endif

      silent = 0;
    }
#endif

  return 0;
}

void
TrashSound (void)		/* Shut down sound  */
{
  UChar dum;

  if (!silent)
    {
      if (sound_driver == 3) // SDL
	{
		SDL_PauseAudio(1);
		SDL_CloseAudio();
	}
#ifdef MSDOS
      if (sound_driver == 2)	// Seal sound
	{
	  AStopVoice (hVoice);
	  ADestroyAudioVoice (hVoice);
	  ACloseVoices ();
	  ACloseAudio ();
	}
      else
#endif

#ifdef ALLEGRO

      if (sound_driver == 1)
	{
	  stop_audio_stream (PCM_stream);

	  if (big_buf)
	    free (big_buf);

	  if (CD_sample)
	    destroy_sample (CD_sample);
#ifdef MSDOS
	  unload_amp ();
#endif
	}

#endif

      for (dum = 0; dum < 6; dum++)
	free (sbuf[dum]);

      free (adpcmbuf);

      silent = 1;

    }

  return;
}

void
write_psg (int ch)
{
  UInt32 Cycle;

  if (CycleNew != CycleOld)
    {
      Cycle = CycleNew - CycleOld;
      CycleOld = CycleNew;

      dwNewPos =
	(unsigned) ((float) (freq_int) * (float) Cycle / (float) BaseClock);
      // in fact, size of the data to write

    };


#ifdef SOUND_DEBUG
  Log ("Nouvelle pos : %d\n", dwNewPos);
#endif

/*  SDL makes clipping automagicaly
 *  if (sound_driver == 3) {
	if (dwNewPos > wanted.size) {
		dwNewPos = wanted.size;
		fprintf(stderr, "overrun: %d\n",dwNewPos);
	}
  }
*/
  
  if (sound_driver == 2) // || sound_driver == 3) /* Added 3 (SDL) */
    if (dwNewPos > (UInt32) freq_int * SOUND_BUF_MS / 1000)
      {
#ifdef SOUND_DEBUG
	fprintf (stderr, "sound buffer overrun\n");
#endif
	dwNewPos = freq_int * SOUND_BUF_MS / 1000;
	// Ask it to fill the buffer
      }
    else if (sound_driver == 1)
      {
#ifdef SOUND_DEBUG
	Log ("dwNewPos = %d / %d\n", dwNewPos, sbuf_size);
#endif
	if (dwNewPos > sbuf_size)
	  {
#ifdef SOUND_DEBUG
	    fprintf (stderr, "sound buffer overrun\n");
#endif
	    dwNewPos = sbuf_size;
	    // Ask it to fill the buffer
	  }

#ifdef SOUND_DEBUG
	Log ("After correction, dwNewPos = %d\n", dwNewPos);
#endif

      }

#ifdef SOUND_DEBUG
  Log ("Buffer %d will be filled\n", ch);
#endif

  WriteBuffer (&sbuf[ch][0], ch, dwNewPos * ds_nChannels);
  // write DATA 'til dwNewPos

#ifdef SOUND_DEBUG
  Log ("Buffer %d has been filled\n", ch);
#endif


};

/* TODO : doesn't support repeat mode for now */

void write_adpcm(void)
{
  UInt32 Cycle;
  UInt32 AdpcmUsedNibbles;

  static SChar index;
  static SInt32 previousValue;

  if (CycleNew != CycleOld)
    {
      Cycle = CycleNew - CycleOld;
      CycleOld = CycleNew;

      dwNewPos =
	(unsigned) ((float) (freq_int) * (float) Cycle / (float) BaseClock);
      // in fact, size of the data to write
    };

  AdpcmFilledBuf = dwNewPos;

  if (new_adpcm_play)
    {
      index = 0;
      previousValue = 0;
    }

  if (AdpcmFilledBuf > io.adpcm_psize)
    AdpcmFilledBuf = io.adpcm_psize;

  AdpcmUsedNibbles =
    WriteBufferAdpcm8 (adpcmbuf,
		       io.adpcm_pptr, AdpcmFilledBuf, &index, &previousValue);

  io.adpcm_pptr += AdpcmUsedNibbles;
  io.adpcm_pptr &= 0x1FFFF;

  if (AdpcmUsedNibbles)
    io.adpcm_psize -= AdpcmUsedNibbles;
  else
    io.adpcm_psize = 0;
  /* If we haven't played even a nibbles, it problably mean we won't ever be
   * able to play one, so we stop the adpcm playing
   */

#ifdef SOUND_DEBUG
//  Log ("size = %d\n", io.adpcm_psize);
#endif

};

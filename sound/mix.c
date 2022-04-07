#include "mix.h"

#ifndef MSDOS

void
update_sound_null ()
{
};
void update_sound_allegro ();

void (*update_sound[3]) () =
{
update_sound_null, update_sound_allegro, update_sound_allegro};

#else

void
update_sound_null ()
{
};
void update_sound_seal ();
void update_sound_allegro ();

void (*update_sound[3]) () =
{
update_sound_null, update_sound_allegro, update_sound_seal};

void
update_sound_seal ()
{
  int dum;
  char ch;
  static int old = 0;

  // fill each channel specific buffer
  for (ch = 0; ch < 6; ch++)
    write_psg (ch);

  write_adpcm ();

  // do a simplistic mixing (should be hard instead of this one)
  /* TEST */

  // Log("new pos = %d\n", dwNewPos);

  /* TEST */
  if (io.adpcm_psize > 1)
    for (dum = 0; dum < dwNewPos; dum++)
      main_buf[dum] = (
		       (
			(sbuf[0][dum]
			 + sbuf[1][dum]
			 + sbuf[2][dum]
			 + sbuf[3][dum]
			 + sbuf[4][dum]
			 + sbuf[5][dum] + adpcmbuf[dum]) >> 2));
  // main_buf[dum] = (adpcmbuf[dum])^0x80;
  else
    for (dum = 0; dum < dwNewPos; dum++)
      main_buf[dum] =
	((sbuf
	  [0][dum] + sbuf[1][dum] + sbuf[2][dum] + sbuf[3][dum] +
	  sbuf[4][dum] + sbuf[5][dum]) >> 2);

// for (dum=0;dum<dwNewPos;dum++)
  // main_buf[dum]=((sbuf[0][dum]+sbuf[1][dum]+sbuf[2][dum]+sbuf[3][dum]+sbuf[4][dum]+sbuf[5][dum])>>2 );
  //     main_buf[dum]=adpcmbuf[dum] << 2;

#if defined(DOUBLE_BUFFER)

  memcpy (lpWave->lpData + old_snd_pos, main_buf,
	  lpWave->dwLength - old_snd_pos);

  if (old_snd_pos)
    {

#ifndef FINAL_RELEASE
      fprintf (stderr, "set loop end to %d\n", old_snd_pos + dwNewPos);
#endif
      lpWave->dwLoopEnd = old_snd_pos + dwNewPos;
      // set the loop end to the end of the sample

      //ASetVoicePosition(hVoice,0);
      AWriteAudioData (lpWave, 0L, lpWave->dwLength);

      {
	long dum;
	do
	  {
	    AGetVoicePosition (hVoice, &dum);

#ifndef FINAL_RELEASE
	    fprintf (stderr, "%d / %d\n", dum, old_snd_pos + dwNewPos);
#endif

	    AUpdateAudio ();
	  }
	while (dum > old_snd_pos);
      }

      old_snd_pos = 0;
    }
  else
    {

#ifndef FINAL_RELEASE
      fprintf (stderr, "i've not set loop end\n");
#endif
      old_snd_pos = dwNewPos;
      AWriteAudioData (lpWave, 0L, lpWave->dwLength);

      {
	long dum;
	do
	  {
	    AGetVoicePosition (hVoice, &dum);

#ifndef FINAL_RELEASE
	    fprintf (stderr, "%d / %d\n", dum, old_snd_pos);
#endif

	    AUpdateAudio ();
	  }
	while (dum < old_snd_pos);
      }


    }

  //memcpy(lpWave->lpData,main_buf,lpWave->dwLength);
  //memcpy(lpWave->lpData,sbuf[2],lpWave->dwLength);
  //lpWave->dwLoopEnd = dwNewPos;
  //AWriteAudioData(lpWave, 0L, lpWave->dwLength);

  //ASetVoicePosition(hVoice,0);
  //old=dwNewPos;
  //       }

  AUpdateAudio ();


#else /* not double buffer */
  {
    long dum;
    do
      {
	AGetVoicePosition (hVoice, &dum);

	AUpdateAudio ();
      }
    while (dum < old - 10);
  }

  memcpy (lpWave->lpData, main_buf, lpWave->dwLength);
  //memcpy(lpWave->lpData,sbuf[2],lpWave->dwLength);
  lpWave->dwLoopEnd = dwNewPos;
  AWriteAudioData (lpWave, 0L, lpWave->dwLength);

  // if dump asked, write in a file
  if (dump_snd)
    fwrite (lpWave->lpData, 1, dwNewPos, out_snd);

  ASetVoicePosition (hVoice, 0);
  old = dwNewPos;
  //       }

  AUpdateAudio ();

#endif /* else double buffer */

}

#endif


#ifdef ALLEGRO

void
update_sound_allegro ()
{

  int dum;
  char ch;

  static where_to_fill = 0;
  static remaining_to_fill = 0;
  static remaining_to_read = 0;

#ifndef FINAL_RELEASE
  // Log("Entering %s\n",__FUNCTION__);
#endif

  // fill each channel specific buffer
  for (ch = 0; ch < 6; ch++)
    write_psg (ch);

  write_adpcm ();

  // Log("%d/%d\n", dwNewPos, SBUF_SIZE_BYTE);

  if (dwNewPos > SBUF_SIZE_BYTE)
    dwNewPos = SBUF_SIZE_BYTE;

  if (remaining_to_fill)
    {
      memcpy (big_buf, main_buf + remaining_to_read, remaining_to_fill);
      where_to_fill = remaining_to_fill;
      remaining_to_fill = 0;
      remaining_to_read = 0;
    }

  // do a simplistic mixing (should be hard instead of this one)
  /* TEST */
  if (io.adpcm_psize > 1)
    for (dum = 0; dum < dwNewPos; dum++)
      main_buf[dum] = (
		       (
			(sbuf[0][dum]
			 + sbuf[1][dum]
			 + sbuf[2][dum]
			 + sbuf[3][dum]
			 + sbuf[4][dum]
			 + sbuf[5][dum] + adpcmbuf[dum]) >> 2)) ^ 0x80;
  // main_buf[dum] = (adpcmbuf[dum])^0x80;
  else
    for (dum = 0; dum < dwNewPos; dum++)
      main_buf[dum] =
	((sbuf[0][dum] + sbuf[1][dum] + sbuf[2][dum] + sbuf[3][dum] +
	  sbuf[4][dum] + sbuf[5][dum]) >> 2) ^ 0x80;

  {

    int size;
    unsigned char *p;

#if defined(LINUX) || defined(MSDOS)
    if (MP3_playing)
      run_amp ();
#endif

    size = dwNewPos;

    if (where_to_fill + size < sbuf_size)
      {
	memcpy (big_buf + where_to_fill, main_buf, size);
	where_to_fill += size;
      }
    else
      {
	memcpy (big_buf + where_to_fill, main_buf, sbuf_size - where_to_fill);
	remaining_to_read = sbuf_size - where_to_fill;
	remaining_to_fill = size - remaining_to_read;
	where_to_fill = 0;

	// if dump asked, write in a file
	if (dump_snd)
	  fwrite (big_buf, 1, sbuf_size, out_snd);

	while (!(p = get_audio_stream_buffer (PCM_stream)));

	memcpy (p, big_buf, sbuf_size);

	free_audio_stream_buffer (PCM_stream);

      }

  }

}


#endif

int
mseq (UInt32 * rand_val)
{
  if (*rand_val & 0x00080000)
    {
      *rand_val = ((*rand_val ^ 0x0004) << 1) + 1;
      return 1;
    }
  else
    {
      *rand_val <<= 1;
      return 0;
    }
};

END_OF_FUNCTION (mseq);

/*
 *
 * Lookup tables for IMA ADPCM format
 *
 */
int AdpcmIndexAdjustTable[16] = {
  -1, -1, -1, -1,		/* +0 - +3, decrease the step size */
  2, 4, 6, 8,			/* +4 - +7, increase the step size */
  -1, -1, -1, -1,		/* -0 - -3, decrease the step size */
  2, 4, 6, 8,			/* -4 - -7, increase the step size */
};

#define ADPCM_MAX_INDEX 48

int AdpcmStepSizeTable[ADPCM_MAX_INDEX + 1] = {
  16, 17, 19, 21, 23, 25, 28,
  31, 34, 37, 41, 45, 50, 55,
  60, 66, 73, 80, 88, 97, 107,
  118, 130, 143, 157, 173, 190,
  209, 230, 253, 279, 307, 337,
  371, 408, 449, 494, 544, 598,
  658, 724, 796, 876, 963, 1060,
  1166, 1282, 1411, 1552
};

/* TODO : improve pointer in adpcm buffer maybe using fixed type */
UInt32
WriteBufferAdpcm8 (UChar * buf,
		   UInt32 begin,
		   UInt32 size, SChar * Index, SInt32 * PreviousValue)
{

  UInt32 ret_val = 0;

  /* TODO: use something else than ALLEGRO's fixed to make this portable */
#ifdef ALLEGRO
  SInt32 step, difference, deltaCode;
  SChar index = *Index;
  SInt32 previousValue = *PreviousValue;
  fixed FixedIndex = 0, FixedInc;


  if (io.adpcm_rate)
    FixedInc = ftofix ((float) io.adpcm_rate * 1000 / (float) freq_int);
  else
    return 0;

  while (size)
    {

      FixedIndex += FixedInc;

      while (FixedIndex > itofix (1))
	{

	  FixedIndex -= itofix (1);

	  ret_val++;

	  deltaCode = PCM[begin >> 1];

	  if (begin & 1)
	    deltaCode >>= 4;
	  else
	    deltaCode &= 0xF;

	  step = AdpcmStepSizeTable[index];

	  begin++;

	  begin &= 0x1FFFF;
	  // Make the adpcm repeat from beginning once finished

	  /* Construct the difference by scaling the current step size */
	  /* This is approximately: difference = (deltaCode+.5)*step/4 */
	  difference = step >> 3;
	  if (deltaCode & 1)
	    difference += step >> 2;
	  if (deltaCode & 2)
	    difference += step >> 1;
	  if (deltaCode & 4)
	    difference += step;

	  if (deltaCode & 8)
	    difference = -difference;

	  /* Build the new sample */
	  previousValue += difference;

	  if (previousValue > 32767)
	    previousValue = 32767;
	  else if (previousValue < -32768)
	    previousValue = -32768;

	  index += AdpcmIndexAdjustTable[deltaCode];
	  if (index < 0)
	    index = 0;
	  else if (index > ADPCM_MAX_INDEX)
	    index = ADPCM_MAX_INDEX;

	}
      /* TEST, was 5 */
      *(buf++) = (previousValue << 6) >> 8;

      size--;

    }

  *Index = index;
  *PreviousValue = previousValue;


#endif

  return ret_val;

}

void
WriteBuffer (char *buf, int ch, unsigned dwSize)
{

//      static DWORD    n[6] = {0,0,0,0,0,0};
// represent the current pointer in the ring buffer of the given chanel


     /* TODO: remove allegro reference here too */
#ifdef ALLEGRO
  static fixed fixed_n[6] = { 0, 0, 0, 0, 0, 0 };
  // current pointer in fixed mode

  fixed fixed_inc;

#define	N	32
  static UInt32 k[6] = { 0, 0, 0, 0, 0, 0 };

  static UInt32 t;
// used to know how much we got to advance in the ring buffer

//      static BOOL             bInit = TRUE;
  static UInt32 r[6];


  static UInt32 rand_val[6] = { 0, 0, 0, 0, 0x51F631E4, 0x51F631E4 };
// random seed for 'noise' generation

  UInt16 wave[32];


  // a*2^(b*x)
  // a = 400, b = log2(16384/a), x = 0.0 ~ 1.0
//      static int              vol_tbl[32] =
//      {
//              100,451,508,573,646,728,821,925,
//              1043,1175,1325,1493,1683,1898,2139,2411,
//              2718,3064,3454,3893,4388,4947,5576,6285,
//              7085,7986,9002,10148,11439,12894,14535,16384,
//      };

  static char vol_tbl[32] = {
    100 / 256, 451 / 256, 508 / 256, 573 / 256, 646 / 256, 728 / 256,
    821 / 256, 925 / 256,
    1043 / 256, 1175 / 256, 1325 / 256, 1493 / 256, 1683 / 256, 1898 / 256,
    2139 / 256, 2411 / 256,
    2718 / 256, 3064 / 256, 3454 / 256, 3893 / 256, 4388 / 256, 4947 / 256,
    5576 / 256, 6285 / 256,
    7085 / 256, 7986 / 256, 9002 / 256, 10148 / 256, 11439 / 256, 12894 / 256,
    14535 / 256, 16384 / 256
  };


  UInt32 dwPos;
  int lvol, i;

  // int rvol;

  UInt32 Tp;

  if (!(io.PSG[ch][4] & 0x80))
    {
      // voice off
      fixed_n[ch] = 0;
      memset (buf, 0, dwSize);
      return;
    }

  if (io.PSG[ch][4] & 0x40)
    {
      wave[0] = ((signed char) io.wave[ch][0] - 16);

      {
	lvol =
	  max ((io.psg_volume >> 3) & 0x1E,
	       (io.psg_volume << 1) & 0x1E) + (io.PSG[ch][4] & 0x1F) +
	  max ((io.PSG[ch][5] >> 3) & 0x1E, (io.PSG[ch][5] << 1) & 0x1E);
	lvol = lvol - 60;
	if (lvol < 0)
	  lvol = 0;

	lvol = (signed char) wave[0] * vol_tbl[lvol] / 16;	// /64 /256

#ifndef FINAL_RELEASE
	fprintf (stderr, "lvol in noise routine is : %d\n\n", lvol);
#endif

	//lvol = (((io.psg_volume>>4)&0x0F) + ((io.psg_volume)&0x0F))
	//        + (io.PSG[ch][4]&0x1F)
	//        + (((io.PSG[ch][5]>>4)&0x0F) + ((io.PSG[ch][5])&0x0F));
	// lvol = [| 0..95 |]

	memset (buf, lvol, dwSize);
      }
    }
  else
    {

      if (ch >= 4 && (io.PSG[ch][7] & 0x80))	//Noise
	{
	  UInt32 Np = (io.PSG[ch][7] & 0x1F);	//Noise 'frequency'
//                      if (ds_nChannels == 2) // STEREO DISABLED
//                      {
//                              lvol = ((io.psg_volume>>3)&0x1E) + (io.PSG[ch][4]&0x1F) + ((io.PSG[ch][5]>>3)&0x1E);
//                              lvol = lvol-60;
//                              if (lvol < 0) lvol = 0;
//                              lvol = vol_tbl[lvol];
//                              rvol = ((io.psg_volume<<1)&0x1E) + (io.PSG[ch][4]&0x1F) + ((io.PSG[ch][5]<<1)&0x1E);
//                              rvol = rvol-60;
//                              if (rvol < 0) rvol = 0;
//                              rvol = vol_tbl[rvol];
//                              for (dwPos = 0; dwPos < dwSize; dwPos += 2)
//                              {
//                                      k[ch] += 3000+Np*512;
//                                      t = k[ch] / (DWORD)freq_int;
//                                      if (t >= 1)
//                                      {
//                                              r[ch] = mseq(&rand_val[ch]);
//                                              k[ch] -= freq_int*t;
//                                      }
//                                      *buf++ = (WORD)((r[ch] ? 10*702 : -10*702)*lvol/64);
//                                      *buf++ = (WORD)((r[ch] ? 10*702 : -10*702)*rvol/64);
//                              }
//                      }
//                      else  // MONO
	  {
	    lvol =
	      max ((io.psg_volume >> 3) & 0x1E,
		   (io.psg_volume << 1) & 0x1E) + (io.PSG[ch][4] & 0x1F) +
	      max ((io.PSG[ch][5] >> 3) & 0x1E, (io.PSG[ch][5] << 1) & 0x1E);
	    //average sound level

	    lvol = lvol - 60;

	    if (lvol < 0)
	      lvol = 0;
	    lvol = vol_tbl[lvol];
	    // get cooked volume

	    //lvol = (((io.psg_volume>>4)&0x0F) + ((io.psg_volume)&0x0F))
	    //        + (io.PSG[ch][4]&0x1F)
	    //        + (((io.PSG[ch][5]>>4)&0x0F) + ((io.PSG[ch][5])&0x0F));
	    // lvol = [| 0..95 |]

	    //lvol = (wave[0] * lvol) / 12;


	    for (dwPos = 0; dwPos < dwSize; dwPos++)
	      {
		k[ch] += 3000 + Np * 512;

		t = k[ch] / (UInt32) freq_int;

		if (t >= 1)
		  {
		    r[ch] = mseq (&rand_val[ch]);
		    k[ch] -= freq_int * t;
		  }

		*buf++ =
		  (signed char) ((r[ch] ? 10 * 702 : -10 * 702) * lvol / 256 /
				 16);
		//sbuf[ch][dum++] = (WORD)((r[ch] ? 10*702 : -10*702)*lvol/64/256);
		//*buf++ = (r[ch] ? 32 : -32) * lvol / 24;
	      }
	  }
	  return;
	}			// END NOISE

      for (i = 0; i < 32; i++)
	wave[i] = (io.wave[ch][i] - 16) * 3;
      // load the data in IO


      Tp = io.PSG[ch][2] + ((UInt32) io.PSG[ch][3] << 8);
      // Tp = frequency

      if (Tp == 0)
	{			// NO SOUND
	  memset (buf, 0, dwSize);
	  return;
	}

      {				// MONO
	lvol =
	  max ((io.psg_volume >> 3) & 0x1E,
	       (io.psg_volume << 1) & 0x1E) + (io.PSG[ch][4] & 0x1F) +
	  max ((io.PSG[ch][5] >> 3) & 0x1E, (io.PSG[ch][5] << 1) & 0x1E);
	lvol = lvol - 60;
	//fprintf(stderr,"%d\n",lvol);
	if (lvol < 0)
	  lvol = 0;
	lvol = vol_tbl[lvol];
	// get cooked volume

//                      lvol = (((io.psg_volume>>4)&0x0F) + ((io.psg_volume)&0x0F))
//                                + (io.PSG[ch][4]&0x1F)
//                                + (((io.PSG[ch][5]>>4)&0x0F) + ((io.PSG[ch][5])&0x0F));
	// lvol = [| 0..95 |]

//                      lvol = (wave[0] * lvol) / 12;


	fixed_inc = ftofix (3.2 * 1118608 / freq_int / Tp);

//         fixed_inc = ftofix(3.2 * 1220300 / freq_int / Tp);

	for (dwPos = 0; dwPos < dwSize; dwPos++)
	  {			// fill the buffer

	    // *buf++ = (char)(wave[n[ch]]*lvol/16);

	    *buf++ = (char) wave[fixed_n[ch] >> 16] * lvol / 16;

	    fixed_n[ch] += fixed_inc;
	    fixed_n[ch] &= (31 << 16) + 65535;

/*
				k[ch] += N*1118608/Tp;

				t = k[ch] / (10*(DWORD)freq_int);
				// the value to advance in the ring buffer
#ifndef FINAL_RELEASE
//            fprintf(stderr,"true advance %f\nfixed one : %f\n\n",N*1118608/Tp);
#endif


				n[ch] = (n[ch]+t)%N;
				// advancing the pointer

				k[ch] -= 10*freq_int*t;

#ifndef FINAL_RELEASE
            fprintf(stderr,"true N = %d\nFixed N = %d\n\n",n[ch],fixed_n[ch] >> 16);
#endif

*/

	  }
      }
    }

#endif

};

#ifdef ALLEGRO
END_OF_FUNCTION (WriteBuffer);
#endif

#ifndef _INCLUDE_SYS_SND_H
#define _INCLUDE_SYS_SND_H

/*
 * Snd section
 * 
 * This section gathers all sound replaying related function on the emulating machine.
 * We have to handle raw waveform (generated from hu-go! and internal pce state) and
 * if possible external format to add support for them in .hcd.
 */

/*
   * osd_snd_set_volume
   *
   * Changes the current global volume
   */
   void osd_snd_set_volume(UChar);


#endif

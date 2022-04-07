#ifndef _INCLUDE_SYS_CD_H
#define _INCLUDE_SYS_CD_H

/*
 * CD acces section
 * 
 * No need to implement them if you don't have a CD machine, don't want
 * or can't add PC Engine CD support
 */
  
  /*
   * osd_cd_init
   * 
   * Try to initialize the cd drive
   * The char* in input is the iso_filename, i.e. the name specified by the
   * -i command line parameter or "" if not specified (use default for your
   * system then)
   * return 0 on success else non zero value on error
   */
  int osd_cd_init(char*);

  /*
   * osd_cd_close
   * 
   * Releases the resources allocated by osd_cd_init
   */
  void osd_cd_close(void);


  /*
   * osd_cd_read
   * 
   * Read a 2048 cooked sector from the cd
   * Returns the read data in the UChar* variable
   * Data are located at sector in the UInt32 variable
   * return 0 on success else non zero value on error
   */
  int osd_cd_read(UChar*, UInt32);

  /*
   * osd_cd_stop_audio
   * 
   * Stops the audio playing from cd is any
   */
  void osd_cd_stop_audio(void);

  /*
   * osd_cd_track_info
   * 
   * Return the msf location of the beginning of the given track
   * plus the control byte of it
   */
  void osd_cd_track_info(UChar track,
                         int* min,
                         int* sec,
                         int* fra,
                         int* control);

  /*
   * osd_cd_nb_tracks
   * 
   * Return the index of the first and last track of the cd
   */
  void osd_cd_nb_tracks(int* first,
                        int* last);

  /*
   * osd_cd_length
   *
   * Return the total length of the cd
   */
  void osd_cd_length(int* min,
                     int* sec,
                     int* fra);


  /*
   * osd_cd_play_audio_track
   * 
   * Plays the selected audio track
   */
  void osd_cd_play_audio_track(UChar track);

  /*
   * osd_cd_play_audio_range
   * 
   * Plays audio range specified in msf mode
   */
  void osd_cd_play_audio_range(UChar min_from,
                               UChar sec_from,
                               UChar fra_from,
                               UChar min_to,
                               UChar sec_to,
                               UChar fra_to);

#endif

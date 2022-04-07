#ifndef _INCLUDE_SYS_INP_H
#define _INCLUDE_SYS_INP_H

/*
 * Input section
 * 
 * This one function part implements the input functionality
 * It's called every vsync I think, i.e. almost 60 times a sec
 */

  /*
   * osd_keyboard
   *
   * Updates the Joypad variables, save/load game, make screenshots, display
   * gui or launch fileselector if asked
   * return 1 if we must quit the emulation
   * else 0
   */
  int osd_keyboard(void);

#endif

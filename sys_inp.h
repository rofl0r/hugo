#ifndef _INCLUDE_SYS_INP_H
#define _INCLUDE_SYS_INP_H

#include "cleantyp.h"

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
	
/*
 * osd_init_input
 *
 * Initialize the input services
 * return 1 if the initialization failed, 0 on success
 */
int osd_init_input(void);


/*
 * current_config
 *
 * Number of the input configuration
 */
extern UChar current_config;

/*
 * joymap
 *
 *
 */
typedef enum
{
  J_UP = 0,
  J_DOWN,
  J_LEFT,
  J_RIGHT,
  J_I,
  J_II,
  J_SELECT,
  J_START,
  J_AUTOI,
  J_AUTOII,
  J_PI,
  J_PII,
  J_PSELECT,
  J_PSTART,
  J_PAUTOI,
  J_PAUTOII,
  J_PXAXIS,
  J_PYAXIS,
  J_MAX,

  J_PAD_START = J_PI
} joymap;

/* 
 * input_config
 *
 * Definition of the type representating an input configuration
 */	  
typedef struct
{
  UInt16 (*pad[5])();
  UChar input_type[5];
  UChar autoI[5];
  UChar autoII[5];
  UChar firedI[5];
  UChar firedII[5];
  UInt16 joy_mapping[5][J_MAX]; // Now can handle UTF codes
} input_config;


/*
 * config
 *
 * The array of input configuration settings
 */
extern input_config config[16];

/* defines for input type field */

#define NONE      0

#define KEYBOARD1 1
#define KEYBOARD2 2
#define KEYBOARD3 3
#define KEYBOARD4 4
#define KEYBOARD5 5

#define JOYPAD1  11
#define JOYPAD2  12
#define JOYPAD3  13
#define JOYPAD4  14

#define MOUSE1   20
#define MOUSE2   21

#define SYNAPLINK 255

	/*
	 * The associated prototypes for common input functions
	 */

/* for nothing */
UInt16 noinput();

/* for keyboard/gamepad input */
UInt16 input1();
UInt16 input2();
UInt16 input3();
UInt16 input4();
UInt16 input5();

/* for joypad */
UInt16 joypad1();
UInt16 joypad2();
UInt16 joypad3();
UInt16 joypad4();

/* for mouse */
UInt16 mouse1();
UInt16 mouse2();

/* for synaptic link */
UInt16 synaplink();

#endif

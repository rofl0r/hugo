#ifndef _INCLUDE_KEYBOARD_H
#define _INCLUDE_KEYBOARD_H

#include "pce.h"

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

typedef struct {
        UInt16 (*pad[5])();
        UChar input_type[5];
        UChar joy_mapping[5][16];
        } input_config;

extern input_config config[16];

extern UChar current_config;

extern char auto_fire_A[5];
/* Is auto fire on */

extern char auto_fire_B[5];
/* Is auto fire on */

/* The common input functions */

/* for nothing */
UInt16 noinput();

/* for keyboard */
UInt16 keyboard1();
UInt16 keyboard2();
UInt16 keyboard3();
UInt16 keyboard4();
UInt16 keyboard5();

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
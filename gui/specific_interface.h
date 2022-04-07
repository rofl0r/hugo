#ifndef _INCLUDE_SPECIFIC_INTERFACE_
#define _INCLUDE_SPECIFIC_INTERFACE_

// This is the dirty part of the interface, I mean a .h file isn't intended
// to contain piece of code or variable but here it's used in a special
// way since it's included only once and mach as a 'body' file rather than
// a 'interface' file.

/*****************************************************************************

    Function: redraw_screen

    Description: Used to know what to draw on screen
                 Called after each key press, not while idling
                 When idling, the code at the beginning of handle_keyboard
                 function is used
    Parameters: nothing but use global variables
    Return: nothing

*****************************************************************************/
void redraw_screen()
{



 }



#endif

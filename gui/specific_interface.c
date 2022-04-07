/***************************************************************************/
/*                                                                         */
/*                    Specific Interface file                              */
/*                                                                         */
/* The goal of this file is to explain how the interf.c source file must   */
/* behave. This file contains action to achieve when selecting one or      */
/* other menu item. It also contains the redraw_screen function. The       */
/* actions to achieve are passed to the handle_keyboard trough an array of */
/* function the body of which is located here. I know it's not a very nice */
/* way of doing but I wanted to generate a single interf.o file containing */
/* all the interface stuff offering a bit of flexibility, this solution    */
/* is very criticable but it's the one I chosed after a hard day of work:) */
/*                                                                         */
/***************************************************************************/

//---------------------------- Export section --------------------------------

// It's not a true export section but it got the same role

  //------------- Export constant section --------------

#define MAX_DEPTH_MENU 3
  // Number of depth in the menu, this is 0 basis
  // This is implemented with a #define instead of the cleaner const int .. =
  // because gcc doesn't like to generate glocal array whose length is
  // a const variable and not a TRUE constant

#define MAX_CHOICE 8
  // Maximum number of choice in each menu depth.
  // The ideal value is the max of the number of choice in each depth
  // Less, you'll have some option not taken in account;
  // More, you'll waste space for nothing.
  // The same remark as in the MAX_DEPTH_MENU declaration applies here

const int MAX_CHOICE_IN_DEPTH[MAX_DEPTH_MENU + 1] = { 8, 5, 6, 2 };
  // Number of choice available in each depth
  // The current choice will be comprised between 0 and this value

  //------------- Forwarded function declaration -------

  // In order to initialise the function array

static void dummy_func ();
static void exit_interface ();
static void exit_emulation ();
static void increase_depth ();
static void decrease_depth ();
static void goto_depth_0 ();
static void goto_depth_1 ();
static void goto_depth_2 ();
static void goto_depth_3 ();
static void show_credits ();
static void load_rom ();

  //------------- Export variable section --------------

#include "lang_interf.h"

void (*action_func[MAX_DEPTH_MENU + 1][MAX_CHOICE + 1]) () =
{
  {
    exit_interface,		// return to emu
      load_rom,			// load rom
      dummy_func,		// reset
      goto_depth_1,		// cheat menu
      goto_depth_2,		// options
      goto_depth_3,		// load save features
      dummy_func,		// help
      show_credits,		// credits
      exit_emulation		// quit
  }
  ,
  {
    dummy_func,			// search for a value
      dummy_func,		// see results
      dummy_func,		// save the cheat
      dummy_func,		// load the cheat
      dummy_func,		// set a value
      dummy_func		// freeze a value
  }
  ,
  {
    dummy_func,			// toggle eagle mode
      dummy_func,		// toggle scanline mode
      dummy_func,		// toggle 2x SAI mode
      dummy_func,		// sound volume
      dummy_func,		// change skin
      dummy_func,		// display info
      dummy_func		// change key config
  }
  ,
  {
    dummy_func,			// Save screen shot
      dummy_func,		// Save game
      dummy_func		// Load game
  }
};

//---------------------------- Own constant section --------------------------

const int INTERLINE = 5;
// Number of pixel to leave between 2 lines of text

//---------------------------- Functions section -----------------------------

/*****************************************************************************

    Function: dummy_func

    Description: do nothing, just used to initialise the function array
    Parameters: none
    Return: nothing

*****************************************************************************/
void
dummy_func ()
{
};

/*****************************************************************************

    Function: exit_interface

    Description: set the exit_asked variable to 1 in order to quit the interface
    Parameters: none
    Return: nothing but set exit_asked global variable

*****************************************************************************/
void
exit_interface ()
{

  exit_asked = 1;

};

/*****************************************************************************

    Function: exit_emulation

    Description: set the exit_asked variable to 1 in order to quit the interface
                 and the exit value of the gui function is set to 1 to tell
                 the emulator to quit
    Parameters: none
    Return: nothing but set exit_asked and must_quit_emu global variable

*****************************************************************************/
void
exit_emulation ()
{

  must_quit_emu = 1;

  exit_asked = 1;

};


/*****************************************************************************

    Function: increase_depth

    Description: go into a submenu increasing the depth
    Parameters: none
    Return: nothing

*****************************************************************************/
void
increase_depth ()
{

  previous_depth = current_depth;

  current_depth++;

}

/*****************************************************************************

    Function: goto_depth_0

    Description: go to the main menu
    Parameters: none
    Return: nothing

*****************************************************************************/
void
goto_depth_0 ()
{

  previous_depth = current_depth;

  current_depth = 0;

}

/*****************************************************************************

    Function: goto_depth_1

    Description: go to the cheat menu
    Parameters: none
    Return: nothing

*****************************************************************************/
void
goto_depth_1 ()
{

  previous_depth = current_depth;

  current_depth = 1;

}

/*****************************************************************************

    Function: goto_depth_2

    Description: go to the screen option menu
    Parameters: none
    Return: nothing

*****************************************************************************/
void
goto_depth_2 ()
{

  previous_depth = current_depth;

  current_depth = 2;

}

/*****************************************************************************

    Function: goto_depth_3

    Description: go to the save/load menu
    Parameters: none
    Return: nothing

*****************************************************************************/
void
goto_depth_3 ()
{

  previous_depth = current_depth;

  current_depth = 3;

}


/*****************************************************************************

    Function: decrease_depth

    Description: return from a submenu decreasing the depth
    Parameters: none
    Return: nothing

*****************************************************************************/
void
decrease_depth ()
{

  previous_depth = current_depth;

  current_depth--;

}

/*****************************************************************************

    Function: show_credits

    Description: show a few words about who did what in Hu-Go!
    Parameters: none
    Return: nothing

*****************************************************************************/
void
show_credits ()			// To improve
{

  blit (sub_menu_src, sub_menu, 0, 0, 0, 0, sub_menu_src->w, sub_menu_src->h);

  output_string (sub_menu_txt, 8 * 12, 8,
		 "HU GO", output_char, output_char_width);

  output_string (sub_menu_txt, 8, 24,
		 "CODE BY ZEOGRAD", output_char, output_char_width);

  output_string (sub_menu_txt, 8, 40,
		 "LOGO BY PIT", output_char, output_char_width);

  output_string (sub_menu_txt, 8 + 8 * 8, 64,
		 "WWW.HUGO.FR.FM", output_char, output_char_width);

  output_string (sub_menu_txt, 8, 80,
		 "YOU CAN ALSO FIND OUR GREAT", output_char,
		 output_char_width);

  output_string (sub_menu_txt, 8, 96,
		 "C COMPILER ON UTOPIA TEAM SITE", output_char,
		 output_char_width);

  output_string (sub_menu_txt, 8 + 5 * 8, 112,
		 "UTOPIA.CONSOLEMUL.COM", output_char, output_char_width);

  output_string (sub_menu_txt, 8, 128,
		 "IN VRAC, LET'S THANKS :", output_char, output_char_width);

  output_string (sub_menu_txt, 8, 144,
		 "DAVID MICHEL, BACONBOY, DAVE", output_char,
		 output_char_width);

  output_string (sub_menu_txt, 8, 160,
		 "SHADOFF, PIT, ISHAN, ARAMIS", output_char,
		 output_char_width);

  output_string (sub_menu_txt, 8, 176,
		 "BERO, HMMX, EAGAN, NYEF, SIR", output_char,
		 output_char_width);

  output_string (sub_menu_txt, 8, 192,
		 "JAGUAR, SIMPLET, READIOSYS", output_char,
		 output_char_width);

  output_string (sub_menu_txt, 8, 208,
		 "YUU, ELMER, DUS, ALBX, GRAVIS", output_char,
		 output_char_width);

  output_string (sub_menu_txt, 8, 224,
		 "ZERO, BWB, AREYOS, ALEX, KARL", output_char,
		 output_char_width);

  output_string (sub_menu_txt, 8, 240,
		 "ELROND, SALTY, FIXO, TORMOD", output_char,
		 output_char_width);


  blit (sub_menu, screen, 0, 0, gui_width / 2, 0, sub_menu->w, sub_menu->h);

  while (osd_keypressed ())
    osd_readkey ();
  while (!osd_keypressed ());
  osd_readkey ();

}

/*****************************************************************************

    Function: load_rom

    Description: call the fileselector to load another file
    Parameters: none
    Return: nothing but can set cart_reload, cart_name and exit_asked

*****************************************************************************/
void
load_rom ()
{

  select_rom ("*.pce");

  if (strcmp (selected_rom, "NO FILE"))
    {
      must_quit_emu = cart_reload = exit_asked = 1;
      strcpy (cart_name, selected_rom);
    }

}


/*****************************************************************************

    Function: redraw_screen

    Description: Used to know what to draw on screen
                 Called after each key press, not while idling
                 When idling, the code at the beginning of handle_keyboard
                 function is used
    Parameters: diverse BITMAP* with quite explicit names
    Return: nothing

*****************************************************************************/
void
redraw_screen (BITMAP * main_menu,
	       BITMAP * sub_menu,
	       BITMAP * screen_menu,
	       BITMAP * desc1_menu,
	       BITMAP * desc2_menu,
	       BITMAP * main_menu_txt,
	       BITMAP * sub_menu_txt,
	       BITMAP * desc1_menu_txt, BITMAP * desc2_menu_txt)
{

  int dum;
  int y;

  blit (main_menu_src, main_menu, 0, 0, 0, 0, main_menu_src->w,
	main_menu_src->h);

  for (dum = 0, y = INTERLINE;
       dum <= MAX_CHOICE_IN_DEPTH[0];
       dum++, y += INTERLINE + output_char_height (' '))
    {

      if (current_choice_in_depth[0] != dum)
	output_string (main_menu_txt, 8, y,
		       item_name[language][0][dum], output_char,
		       output_char_width);
      else
	output_string (main_menu_txt, 8, y, item_name[language][0][dum],
		       (current_depth ? output_char : output_char_highlight),
		       output_char_width);

    }

  blit (main_menu, screen, 0, 0, 0, 0, main_menu->w, main_menu->h);

  if (current_depth)
    {

      blit (sub_menu_src, sub_menu, 0, 0, 0, 0, sub_menu_src->w,
	    sub_menu_src->h);

      for (dum = 0, y = INTERLINE;
	   dum <= MAX_CHOICE_IN_DEPTH[current_depth];
	   dum++, y += INTERLINE + output_char_height (' '))
	{

	  if (current_choice_in_depth[current_depth] != dum)
	    output_string (sub_menu_txt, 8, y,
			   item_name[language][current_depth][dum],
			   output_char, output_char_width);
	  else
	    output_string (sub_menu_txt, 8, y,
			   item_name[language][current_depth][dum],
			   output_char_highlight, output_char_width);

	}

      blit (sub_menu, screen, 0, 0, gui_width / 2, 0, sub_menu->w,
	    sub_menu->h);

    }
  else
    {
      blit (screen_menu_src, screen, 0, 0, gui_width / 2, 0, screen_menu->w,
	    screen_menu->h);
    }

}

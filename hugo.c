#include "hugo.h"


//! name of the backup ram filename
static char backup_mem[PATH_MAX];

struct host_machine host;
struct hugo_options option;

//! Setup the host machine
/*!
 * /param argc The number of argument on command line
 * /param argv The arguments on command line
 * /return zero on success else non zero value
 */
int initialisation(int argc, char* argv[])
{
  UInt16 i;

#ifdef MSDOS
  _crt0_startup_flags |= _CRT0_FLAG_NO_LFN;
  // Disable long filename to avoid mem waste in select_rom func.
#endif

  option.want_stereo = option.want_fullscreen = option.want_fullscreen_aspect = FALSE;
  option.window_size = 1;

	// Initialise paths
	osd_init_paths(argc, argv);
	
	// Create the log file
	init_log_file ();  

	// Init the random seed
    srand (time (NULL));

	// Read configuration in ini file
    parse_INIfile ();

	// Read the command line
	parse_commandline(argc, argv);
	
	// Initialise the host machine
	if (!osd_init_machine ())
      return -1;
	
	// If backup memory name hasn't been overriden on command line, use the default
    if ((bmdefault) && (strcmp(bmdefault,"")))
		sprintf(backup_mem, "%s%s", short_exe_name, bmdefault);
	else
		sprintf(backup_mem, "%sbackup.dat", short_exe_name);

	// In case of crash, try to free audio related ressources if possible
	atexit (TrashSound);

	// Initialise the input devices
	if (osd_init_input () != 0)
		{
			fprintf(stderr, "Initialization of input system failed\n");
			return (-2);
		}
		
	return 0;
}


//! Free ressources of the host machine
/*!
 * Deallocate ressources reserved during the initialisation
 */
void
cleanup ()
{

  // Deinitialise the host machine
  osd_shut_machine ();
	
}

//! Check if a game was asked
/*!
 * /return non zero if a game must be played
 */
int
game_asked ()
{
	return ((CD_emulation == 1) || (strcmp(cart_name, "")));
}

//! Run an instance of a rom or cd or iso
/*!
 * /return non zero if another game has to be launched
 */
int
play_game (void)
{
	if (!(*osd_gfx_driver_list[video_driver].init) ())
	  {
		Log ("Can't set graphic mode\n");
		printf (MESSAGE[language][cant_set_gmode]);
		return 0;
	  }

	// Initialise the target machine (pce)
	if (InitPCE(cart_name, backup_mem) != 0)
	  {
	    (*osd_gfx_driver_list[video_driver].shut) ();
		return 0;
	  }
	
	// Run the emulation
	RunPCE();
	
	// Free the target machine (pce)
	TrashPCE(backup_mem);

	(*osd_gfx_driver_list[video_driver].shut) ();
		
	return cart_reload;
}


int
main (int argc, char* argv[])
{
  int error;
  error = 0;

  error = initialisation (argc, argv);

#if defined(GTK)

  if (!error)
    {
	  if (game_asked())
	  {
		while (play_game());
	  }	  
	  else
          {
            build_gtk_interface (argc, argv);
          }
    }

#else // not defined(GTK)

  if (!error)
    {
      if (game_asked ())
			{
				while (play_game ());
			}
					else
			{
				printf ("No game specified\n");
			}
    }
#endif

  cleanup();

  return 0;
}

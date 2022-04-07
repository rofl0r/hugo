/***************************************************************************/
/*                                                                         */
/*				     Graphic User Interface source file								*/
/*																									*/
/*			 Most parts of this file are from me (ZEOGRAD)                    */
/*     You can use it, I trust you not to rip me but I would like to       */
/*     Be contacted if you do so and then quoted where you use this        */
/*     That's all, you can gain as much money as you want with it =)       */
/*                                                                         */
/*		 Well, all of this have been modified so much that it can be         */
/*     considered as part of the Hu-Go! project                            */
/*                                                                         */
/*     This file should be machine independant (just blitting & textout)   */
/*     But maybe the filesystem functions will have to be adapted          */
/*                                                                         */
/*             (c)1999 Zeograd (Olivier Jolly) Zeograd@caramail.com			*/
/*															  Ojolly@hotmail.com				*/
/***************************************************************************/

// Most of this is still en french, I will change this slowly...

#include "gui.h"

#ifndef PI
#define PI 3.14159265358979323846
#endif

extern BITMAP *EAGLE_buf;

UChar zip_support_in_fs = 1;
// If null, won't scan archives for PCE inside ...

char selected_rom[128] = "";
// the buffer where the name of the rom will be placed

char name_to_extract[64] = "";
// In case of selected_rom is the name of a archive, this string
// indicate the name of the file to extract

char initial_path[128] = "";
// prefered path for for searching

char research_name[30] = "";
// current typed name

extern char tmp_buf[];
// a general purpose buffer (size 100 chars)

RGB cycle[32];
// Colors to be cycled

const char *INDEX_NAME = "ROM_LIST.HG!";
// Name of the file where we keep track of the known roms

UChar static_refresh = 0;
// used to know whether we check the game directory each time

BITMAP *textoutput;
// the bitmap where we draw things to display on screen

// The file to redirect stdout to.
int stdout_h_bak, stdout_h;
char stdout_filename[] = "STDOUT.RDF";

int stderr_h_bak, stderr_h;
char stderr_filename[] = "STDERR.RDF";


int (*dummy_func) (const void *e1, const void *e2);

/**************************************************
****                                           ****
**** Code copied from RHIDE! Credits to RHIDE! ****
****                                           ****
**************************************************/
void
redirect_stdout (void)
{
  // Backup old `stderr'.
  stdout_h_bak = dup (fileno (stdout));
  // Create new file that stderr will be redirected to.
  stdout_h = open (stdout_filename, O_RDWR | O_CREAT | O_TRUNC | O_BINARY);
  // Clear `stdout'.
  fflush (stdout);
  // Redirect `stdout'.
  dup2 (stdout_h, 2);
}

void
unredirect_stdout (void)
{
  // Restore stdout ...
  dup2 (stdout_h_bak, 2);
  // ... and close files opened by me.
  close (stdout_h);
  close (stdout_h_bak);
}

void
redirect_stderr (void)
{
  // Backup old `stderr'.
  stderr_h_bak = dup (fileno (stderr));
  // Create new file that stderr will be redirected to.
  stderr_h = open (stderr_filename, O_RDWR | O_CREAT | O_TRUNC | O_BINARY);
  // Clear `stdout'.
  fflush (stderr);
  // Redirect `stdout'.
  dup2 (stderr_h, 2);
}

void
unredirect_stderr (void)
{
  // Restore stderr ...
  dup2 (stderr_h_bak, 2);
  // ... and close files opened by me.
  close (stderr_h);
  close (stderr_h_bak);
}


/**************************
****                   ****
**** End of RHIDE code ****
****                   ****
**************************/



/*****************************************************************************

    Function:  spaces

    Description: returns a string made of spaces
    Parameters: number of spaces
    Return: a pointer to tmp_buf filled with spaces

*****************************************************************************/
char *
spaces (unsigned char nb)
{
  memset (tmp_buf, ' ', nb);
  tmp_buf[nb] = 0;
  return tmp_buf;
}


/*****************************************************************************

    Function:  faff

    Description:crapy function for internal use, in fact format a bit a filename
    Parameters:char* im, the raw filename
    Return:a pointer to tmp_buf, loaded with the result

*****************************************************************************/
char *
faff (char im[16])
{
  char dummy[16];
  strcpy (tmp_buf, "                   ");
  if (!im[0])
    return tmp_buf;
  else if (!check_char (im, '.'))
    return strncpy (tmp_buf, im, 8);
  else
    {
      int x = 0, y = 0;
      while ((im[(y = x)] != '.') && (x < 12))
	dummy[x] = im[x++];
      while (x < 8)
	dummy[x++] = ' ';
      dummy[8] = '.';
      y++;
      x++;
      while (im[y])
	dummy[x++] = im[y++];
      dummy[x] = 0;
      return strcpy (tmp_buf, dummy);
    }
};

/*****************************************************************************

    Function:  add_index

    Description: add info for the filename to the current index called INDEX_NAME
    Parameters:char* basefilename, the filename of the file that contain the rom
                  (either *.PCE or *.ZIP)
					char* romfilename, in case of archive, the name of the rom
					   (*.PCE kind)
               char* romname, the true name of the rom
               char* cur_dir, the current directory
    Return:0 if failed, 1 on success

*****************************************************************************/
int
add_index (char *basefilename, char *romfilename, char *romname,
	   char *cur_dir)
{
  FILE *f;
  char *tmp_buf = (char *) alloca (100);
  INDEX_ITEM tmp_item;

  if (!static_refresh)
    return 0;

  sprintf (tmp_buf, "%s/%s", cur_dir, INDEX_NAME);

  if (!(f = fopen (tmp_buf, "ab+")))
    return 0;

  memset (&tmp_item, 0, sizeof (tmp_item));

  strncpy (tmp_item.basefilename, basefilename, 13);

  strncpy (tmp_item.romfilename, romfilename, 13);

  strncpy (tmp_item.romname, romname, 16);

  fwrite (&tmp_item, 1, sizeof (INDEX_ITEM), f);
  fclose (f);

  return 1;
}



/*****************************************************************************

    Function:  affiche_choix [lit. draw_choice]

    Description: draws  the current available files & dir
    Parameters: TO_DISPLAY to_display[1500] : array of struct to display
	             unsigned dirt : index 'til there are only directories
					 unsigned choix : [choice] the index of the actual choice
					 unsigned arch : index 'il then there are archived files
					 BITMAP * background : a pointer to the background
					 unsigned fich : [FICHier<=>file] index 'til there are files
					 unsigned anc_choix : [ANCien Choix<=>old choice] the last
					          selected item (to do some scrolling)
    Return: AAAAAAAAAAAAAAArg! Nop', Yep guy!

*****************************************************************************/


void
affichechoix (TO_DISPLAY to_display[1500],
	      unsigned const dirt,
	      unsigned const choix,
	      unsigned const arch,
	      BITMAP * background,
	      unsigned const fich, unsigned const anc_choix)
{
  unsigned short dummy3, dummy2;
  unsigned basex = 0, basecenter;

  clear (textoutput);

#define side_choix(cond,init) if (choix-(choix%20)cond) { for (dummy3=0;dummy3<=19;dummy3++) { dummy2=init; if (dummy2+dummy3<dirt) textoutshadow(textoutput,font,to_display[dummy2+dummy3].str,basex+1,2+dummy3*10,-4,-5,1,1) else textoutshadow(textoutput,font,to_display[dummy2+dummy3].str,basex+1,2+10*dummy3,-3,-2,1,1) } basex+=8*16;}

  side_choix (>=20, choix - (choix % 20) - 20);

  basecenter = basex;
  for (dummy3 = 0; dummy3 <= 19; dummy3++)
    {				//{? choix - choix mod 20 + dummy 3 < dirt}
      dummy2 = choix - (choix % 20);
      if (dummy2 + dummy3 < dirt)
	{			//dir
	  textout (textoutput, font, to_display[dummy2 + dummy3].str,
		   basex + 1, 2 + dummy3 * 10, -5);
	  if (choix % 20 == dummy3)
	    textout (textoutput, font, to_display[dummy2 + dummy3].str, basex,
		     1 + dummy3 * 10, -1);
	  else
	    textout (textoutput, font, to_display[dummy2 + dummy3].str, basex,
		     1 + dummy3 * 10, -4);
	}
      else			//file
	{
	  textout (textoutput, font, to_display[dummy2 + dummy3].str,
		   basex + 1, 2 + 10 * dummy3, -2);
	  if (choix % 20 == dummy3)
	    textout (textoutput, font, to_display[dummy2 + dummy3].str, basex,
		     1 + 10 * dummy3, -1);
	  else
	    textout (textoutput, font, to_display[dummy2 + dummy3].str, basex,
		     1 + 10 * dummy3, -3);
	}
    }
  basex += 8 * 16;


  side_choix (<dirt + fich + arch - 20, choix - (choix % 20) + 20);
  side_choix (<dirt + fich + arch - 40, choix - (choix % 20) + 40);

  {
    char diff = anc_choix - (anc_choix % 20) - choix + (choix % 20);
    if (diff < 0)
      {				//a bit of scrolling ->
	unsigned x;
	for (x = basecenter - 16 * 8; x < basecenter; x += 16)
	  {
	    vsync ();
	    if (!use_eagle)
	      {
		blit (background, screen, 0, 0, (vwidth - background->w) / 2,
		      (vheight - background->h) / 2, background->w,
		      background->h);
		masked_blit (textoutput, screen, x, 0,
			     (vwidth - background->w) / 2,
			     (vheight - background->h) / 2, background->w,
			     background->h);
	      }
	    else
	      {
		int dum;

		blit (background, XBuf, 0, 0, 0, 0,
		      background->w, background->h);
		masked_blit (textoutput, XBuf, x, 0, 0, 0,
			     background->w, background->h);

		for (dum = 0; dum < 200 - 1; dum++)
		  {
		    // Eagle-blit piccy to the screen 
		    eagle ((unsigned long *) XBuf->line[dum]	// First line of piccy  (320 pixels)
			   , (unsigned long *) XBuf->line[dum + 1]	// 2nd line of piccy (320 pixels)
			   , 256, EAGLE_buf->seg, (int) EAGLE_buf->line[dum * 2]	//  + ( ( (x   <<1) * piccy->w ) << 1)           // First eagled line = 320x2 = 640 pixels
			   , (int) EAGLE_buf->line[1 + (dum * 2)]	// + (( ( (x+1)<<1) * piccy->w ) << 1)       // 2nd eagled line = 320x2 = 640 pixels
		      );
		  }

		blit (EAGLE_buf, screen, 0, 0, (320 - 256), (240 - 200),
		      256 * 2, 199 * 2);


	      }
	  }
      }
    if (diff > 0)
      {				//a bit of scrolling <-
	unsigned x;
	for (x = basecenter + 16 * 8; x > basecenter; x -= 16)
	  {
	    vsync ();
	    if (!use_eagle)
	      {
		blit (background, screen, 0, 0, (vwidth - background->w) / 2,
		      (vheight - background->h) / 2, background->w,
		      background->h);
		masked_blit (textoutput, screen, x, 0,
			     (vwidth - background->w) / 2,
			     (vheight - background->h) / 2, background->w,
			     background->h);
	      }
	    else
	      {
		int dum;

		blit (background, XBuf, 0, 0, 0, 0,
		      background->w, background->h);
		masked_blit (textoutput, XBuf, x, 0, 0, 0,
			     background->w, background->h);

		for (dum = 0; dum < 200 - 1; dum++)
		  {
		    // Eagle-blit piccy to the screen 
		    eagle ((unsigned long *) XBuf->line[dum]	// First line of piccy  (320 pixels)
			   , (unsigned long *) XBuf->line[dum + 1]	// 2nd line of piccy (320 pixels)
			   , 256, EAGLE_buf->seg, (int) EAGLE_buf->line[dum * 2]	//  + ( ( (x   <<1) * piccy->w ) << 1)           // First eagled line = 320x2 = 640 pixels
			   , (int) EAGLE_buf->line[1 + (dum * 2)]	// + (( ( (x+1)<<1) * piccy->w ) << 1)       // 2nd eagled line = 320x2 = 640 pixels
		      );
		  }

		blit (EAGLE_buf, screen, 0, 0, (320 - 256), (240 - 200),
		      256 * 2, 199 * 2);

	      }
	  }
      }
  }

  vsync ();
  if (!use_eagle)
    {
      blit (background, screen, 0, 0, (vwidth - background->w) / 2,
	    (vheight - background->h) / 2, background->w, background->h);
      masked_blit (textoutput, screen, basecenter, 0,
		   (vwidth - background->w) / 2,
		   (vheight - background->h) / 2, background->w,
		   background->h);
    }
  else
    {
      int dum;

      blit (background, XBuf, 0, 0, 0, 0, background->w, background->h);
      masked_blit (textoutput, XBuf, basecenter, 0, 0, 0,
		   background->w, background->h);

      for (dum = 0; dum < 200 - 1; dum++)
	{
	  // Eagle-blit piccy to the screen 
	  eagle ((unsigned long *) XBuf->line[dum]	// First line of piccy  (320 pixels)
		 , (unsigned long *) XBuf->line[dum + 1]	// 2nd line of piccy (320 pixels)
		 , 256, EAGLE_buf->seg, (int) EAGLE_buf->line[dum * 2]	//  + ( ( (x   <<1) * piccy->w ) << 1)           // First eagled line = 320x2 = 640 pixels
		 , (int) EAGLE_buf->line[1 + (dum * 2)]	// + (( ( (x+1)<<1) * piccy->w ) << 1)       // 2nd eagled line = 320x2 = 640 pixels
	    );
	}

      blit (EAGLE_buf, screen, 0, 0, (320 - 256), (240 - 200), 256 * 2,
	    199 * 2);



    }

}

/*****************************************************************************

    Function:  key_select

    Description: handle keyboard in the file selector
    Parameters: lot of! heavily derived from pascal and C can't handle imbricated funcs!
    Return: nothing but *choix may change
	         may fill selected_rom and name_to_extract

*****************************************************************************/
void
key_select (char *nom_dir,
	    unsigned *choix, unsigned *dirt,
	    ARCH_FILE fichier[1600],
	    unsigned *fich, unsigned *arch,
	    char *ext, BITMAP * textoutput,
	    char *must_draw, TO_DISPLAY to_display[1600])
{
  int t;
  static char idx_col = 0, temp_col = 0;
  while (!osd_keypressed ())
    {
#ifdef MSDOS
      pause ();
#endif
      vsync ();
      temp_col = !temp_col;
      if (temp_col)		// a bit of delay
	set_color (255, &cycle[idx_col++, idx_col &= 0x1F]);
    }
  t = osd_readkey ();

  switch (t & 0xFF)
    {
    case 13:
      {
	if (*choix < *dirt)	//change dir
	  {
	    strcat (nom_dir, "/");
	    strcat (nom_dir, fichier[to_display[*choix].corr].basefilename);
#ifdef MSDOS
	    _fixpath (nom_dir, nom_dir);
#endif
	    strupr (nom_dir);

	    {
	      unsigned dummy;
	      for (dummy = 0; dummy < 1600; dummy++)
		strcpy (to_display[dummy].str, "");
	    }

	    Dir (dirt, fich, arch, nom_dir, fichier, must_draw, to_display);
	    clear (textoutput);

	    *choix = 0;
	    strcpy (research_name, "");
	  }
	else if (*choix < (*dirt) + (*fich))	// normal file
	  {
	    strcpy (tmp_buf, nom_dir);
	    if (strlen (nom_dir) != 3)
	      strcat (tmp_buf, "/");

	    strcat (tmp_buf, fichier[to_display[*choix].corr].basefilename);
	    strupr (tmp_buf);

	    strcpy (selected_rom, tmp_buf);

	    *ext = 1;
	  }
	else			// archived file
	  {
	    strcpy (tmp_buf, nom_dir);
	    if (strlen (nom_dir) != 3)
	      strcat (tmp_buf, "/");

	    strcat (tmp_buf, fichier[to_display[*choix].corr].basefilename);
	    strupr (tmp_buf);

	    strcpy (selected_rom, tmp_buf);

	    strcpy (name_to_extract,
		    fichier[to_display[*choix].corr].archived);

	    *ext = 1;
	  }
	return;
      }
    case 27:
      *ext = 1;
      strcpy (selected_rom, "NO FILE");
      return;
    case 8:			// backspace
      if (strlen (research_name))
	research_name[strlen (research_name) - 1] = 0;
      break;
    case ' ':
    case 'a'...'z':
      if (strlen (research_name) < 29)
	{
	  UInt32 dum;
	  sprintf (research_name, "%s%c", research_name, toupper (t & 0xff));
#if !defined(FINAL_RELEASE)
	  fprintf (stderr, "%s\n", research_name);
#endif
	  dum = (*dirt);
	  while ((dum < (*dirt) + (*arch) + (*fich))
		 && (strcmp (research_name, to_display[dum].str) >= 0))
	    {
#if !defined(FINAL_RELEASE)
	      fprintf (stderr, "%s tested\n", to_display[dum].str);
#endif
	      dum++;
	    }

#if !defined(FINAL_RELEASE)
	  fprintf (stderr, "strstr result : %s\n",
		   strstr (to_display[dum].str, research_name));
#endif


	  if (strstr (to_display[dum].str, research_name) ==
	      to_display[dum].str)
	    {
#if !defined(FINAL_RELEASE)
	      fprintf (stderr, "%s is OK\n", to_display[dum].str);
#endif

	      //*choix=to_display[dum].corr;
	      *choix = dum;
	    }

	}
      break;
    case 'A'...'Z':
      strcpy (nom_dir, " :/");
      nom_dir[0] = (char) t;
      {
	unsigned dummy;
	for (dummy = 0; dummy < 1600; dummy++)
	  strcpy (to_display[dummy].str, "");;
      }


      Dir (dirt, fich, arch, nom_dir, fichier, must_draw, to_display);

      clear (textoutput);
      *choix = 0;
      strcpy (research_name, "");
      return;
    }
  switch (t >> 8)
    {

    case KEY_DOWN:
      if (*choix < *arch + *fich + *dirt - 1)
	(*choix)++;
      strcpy (research_name, "");
      return;
    case KEY_UP:
      if (*choix > 0)
	(*choix)--;
      strcpy (research_name, "");
      return;
    case KEY_RIGHT:
      if ((*choix) + 21 <= (*arch) + (*fich) + (*dirt))
	{
	  *choix += 20;
	  strcpy (research_name, "");
	  return;
	}
      else
	{
	  *choix = (*fich) + (*dirt) + (*arch) - 1;
	  strcpy (research_name, "");
	  return;
	};
    case KEY_LEFT:
      if (*choix > 20)
	*choix -= 20;
      else
	*choix = 0;
      strcpy (research_name, "");
      return;
    case KEY_HOME:
      *choix = 0;
      strcpy (research_name, "");
      return;
    case KEY_END:
      *choix = *arch + *fich + *dirt - 1;
      strcpy (research_name, "");
      return;
    case KEY_F12:
      *ext = 1;
      strcpy (selected_rom, "TRUE EXIT");
      return;
    case KEY_F1:
      sprintf (tmp_buf, "%s/%s", nom_dir, INDEX_NAME);
      unlink (tmp_buf);
      {
	unsigned dummy;
	for (dummy = 0; dummy < 1600; dummy++)
	  strcpy (to_display[dummy].str, "");
      }

      Dir (dirt, fich, arch, nom_dir, fichier, must_draw, to_display);
      clear (textoutput);

      strcpy (research_name, "");
      *choix = 0;

      return;

    case KEY_F2:
      sprintf (tmp_buf, "%s/%s", nom_dir, INDEX_NAME);
      unlink (tmp_buf);
      {
	unsigned dummy;
	for (dummy = 0; dummy < 1600; dummy++)
	  strcpy (to_display[dummy].str, "");

	dummy = zip_support_in_fs;
	zip_support_in_fs = 1;

	Dir (dirt, fich, arch, nom_dir, fichier, must_draw, to_display);

	zip_support_in_fs = dummy;

      }
      clear (textoutput);

      strcpy (research_name, "");
      *choix = 0;

      return;

    }				//1st case
}


void
Dir (unsigned *dirt,
     unsigned *fich,
     unsigned *arch,
     char *nom_dir,
     ARCH_FILE fichier[1600], char *must_draw, TO_DISPLAY to_display[1600])
{
 #ifdef MSDOS
  char direct[100][14];
  char fi[1500][14];
  char base_arch[1500][14];
  char archived[1500][14];
  struct ffblk DirBuf, DirBufArch;
  unsigned i, dummy;
  char tmp_dir[80], tmp_dir_arch[80];
  FILE *f;
  INDEX_ITEM *index;
  unsigned nb_item_in_index = 0;


  if (!use_eagle)
    {
      textout (screen, font, MESSAGE[language][reading_dir],
	       blit_x + 1 + (256 -
			     text_length (font,
					  MESSAGE[language][reading_dir])) /
	       2, blit_y + 101, -3);
      textout (screen, font, MESSAGE[language][reading_dir],
	       blit_x + (256 -
			 text_length (font,
				      MESSAGE[language][reading_dir])) / 2,
	       blit_y + 100, -2);
    }
  else
    {

      textout (EAGLE_buf, font, MESSAGE[language][reading_dir],
	       1 + (256 -
		    text_length (font, MESSAGE[language][reading_dir])) / 2,
	       101, -3);
      textout (EAGLE_buf, font, MESSAGE[language][reading_dir],
	       (256 - text_length (font, MESSAGE[language][reading_dir])) / 2,
	       100, -2);


      //blit(textoutput,screen,0,0,blit_x,blit_y,256,200);
    }

  *arch = *dirt = *fich = 0;
  *must_draw = 1;

  strcpy (tmp_dir, nom_dir);

  strcat (tmp_dir, "/*.*");
  if (findfirst (tmp_dir, &DirBuf, FA_DIREC) == 0)	// Look for the first entry
    do
      {
	if (((DirBuf.ff_attrib & 0x10) != 0)
	    && (strcmp (DirBuf.ff_name, ".") != 0))
	  {
	    strcpy (direct[(*dirt)++], strupr (DirBuf.ff_name));
	    direct[(*dirt) - 1][13] = 0;
	  }
      }
    while (findnext (&DirBuf) == 0);	// until there's no more

  i = 0;

  /* Fill the list of to_display with directories */
  if (*dirt > 0)
    for (dummy = 0; dummy < *dirt; dummy++)
      {
	strcpy (fichier[i].basefilename, direct[dummy]);
	to_display[i].corr = i;
	sprintf (to_display[i++].str, "[%-8s]", direct[dummy]);
      }

// Opening of the index
  sprintf (tmp_buf, "%s/%s", nom_dir, INDEX_NAME);
  index = NULL;
  if ((static_refresh) && (f = fopen (tmp_buf, "rb")))
    {
      if (!(filesize (f) % sizeof (INDEX_ITEM)))
	{
	  index = (INDEX_ITEM *) alloca (filesize (f));
	  fread (index, 1, filesize (f), f);
	  nb_item_in_index = filesize (f) / sizeof (INDEX_ITEM);
	  fclose (f);
	}
      else
	{
	  fclose (f);
	  unlink (tmp_buf);
	}
    }

  if ((static_refresh) && (index))
    {
      for (dummy = 0; dummy < nb_item_in_index; dummy++)
	{
	  (*arch)++;
	  strcpy (fichier[i].basefilename,
		  strupr (index[dummy].basefilename));
	  strcpy (fichier[i].archived, strupr (index[dummy].romfilename));

	  strcpy (to_display[i].str, index[dummy].romname);
	  to_display[i].corr = i;

	  i++;
	}

      strcpy (tmp_dir, nom_dir);
      strcat (tmp_dir, "/*.ISQ");
      if (findfirst (tmp_dir, &DirBuf, FA_RDONLY | FA_ARCH) == 0)
	do
	  {
	    if ((DirBuf.ff_attrib & 0x10) == 0)
	      {
		(*arch)++;
		strcpy (fichier[i].basefilename, strupr (DirBuf.ff_name));
		strcpy (fichier[i].archived, (DirBuf.ff_name));

		strcpy (to_display[i].str, (DirBuf.ff_name));
		to_display[i].corr = i;

		i++;

	      }
	  }
	while (findnext (&DirBuf) == 0);


      strcpy (tmp_dir, nom_dir);
      strcat (tmp_dir, "/*.BIN");
      if (findfirst (tmp_dir, &DirBuf, FA_RDONLY | FA_ARCH) == 0)
	do
	  {
	    if ((DirBuf.ff_attrib & 0x10) == 0)
	      {
		(*arch)++;
		strcpy (fichier[i].basefilename, strupr (DirBuf.ff_name));
		strcpy (fichier[i].archived, (DirBuf.ff_name));

		strcpy (to_display[i].str, (DirBuf.ff_name));
		to_display[i].corr = i;

		i++;

	      }
	  }
	while (findnext (&DirBuf) == 0);



      strcpy (tmp_dir, nom_dir);
      strcat (tmp_dir, "/*.ISO");
      if (findfirst (tmp_dir, &DirBuf, FA_RDONLY | FA_ARCH) == 0)
	do
	  {
	    if ((DirBuf.ff_attrib & 0x10) == 0)
	      {
		(*arch)++;
		strcpy (fichier[i].basefilename, strupr (DirBuf.ff_name));
		strcpy (fichier[i].archived, (DirBuf.ff_name));

		strcpy (to_display[i].str, (DirBuf.ff_name));
		to_display[i].corr = i;

		i++;

	      }
	  }
	while (findnext (&DirBuf) == 0);

      strcpy (tmp_dir, nom_dir);
      strcat (tmp_dir, "/*.HCD");
      if (findfirst (tmp_dir, &DirBuf, FA_RDONLY | FA_ARCH) == 0)
	do
	  {
	    if ((DirBuf.ff_attrib & 0x10) == 0)
	      {
		(*arch)++;
		strcpy (fichier[i].basefilename, strupr (DirBuf.ff_name));
		strcpy (fichier[i].archived, (DirBuf.ff_name));

		strcpy (to_display[i].str, (DirBuf.ff_name));
		to_display[i].corr = i;

		i++;

	      }
	  }
	while (findnext (&DirBuf) == 0);


    }
  else
    {

      strcpy (tmp_dir, nom_dir);
      strcat (tmp_dir, "/*.PCE");
      if (findfirst (tmp_dir, &DirBuf, FA_RDONLY | FA_ARCH) == 0)
	do
	  {
	    if ((DirBuf.ff_attrib & 0x10) == 0)
	      {
		strcpy (fi[(*fich)++], strupr (DirBuf.ff_name));
		fi[(*fich) - 1][13] = 0;
	      }
	  }
	while (findnext (&DirBuf) == 0);

      strcpy (tmp_dir, nom_dir);
      strcat (tmp_dir, "/*.ISQ");
      if (findfirst (tmp_dir, &DirBuf, FA_RDONLY | FA_ARCH) == 0)
	do
	  {
	    if ((DirBuf.ff_attrib & 0x10) == 0)
	      {
		strcpy (fi[(*fich)++], strupr (DirBuf.ff_name));
		fi[(*fich) - 1][13] = 0;
	      }
	  }
	while (findnext (&DirBuf) == 0);

      strcpy (tmp_dir, nom_dir);
      strcat (tmp_dir, "/*.ISO");
      if (findfirst (tmp_dir, &DirBuf, FA_RDONLY | FA_ARCH) == 0)
	do
	  {
	    if ((DirBuf.ff_attrib & 0x10) == 0)
	      {
		strcpy (fi[(*fich)++], strupr (DirBuf.ff_name));
		fi[(*fich) - 1][13] = 0;
	      }
	  }
	while (findnext (&DirBuf) == 0);

      strcpy (tmp_dir, nom_dir);
      strcat (tmp_dir, "/*.BIN");
      if (findfirst (tmp_dir, &DirBuf, FA_RDONLY | FA_ARCH) == 0)
	do
	  {
	    if ((DirBuf.ff_attrib & 0x10) == 0)
	      {
		strcpy (fi[(*fich)++], strupr (DirBuf.ff_name));
		fi[(*fich) - 1][13] = 0;
	      }
	  }
	while (findnext (&DirBuf) == 0);


      strcpy (tmp_dir, nom_dir);
      strcat (tmp_dir, "/*.HCD");
      if (findfirst (tmp_dir, &DirBuf, FA_RDONLY | FA_ARCH) == 0)
	do
	  {
	    if ((DirBuf.ff_attrib & 0x10) == 0)
	      {
		strcpy (fi[(*fich)++], strupr (DirBuf.ff_name));
		fi[(*fich) - 1][13] = 0;
	      }
	  }
	while (findnext (&DirBuf) == 0);

#ifndef linux
      if (zip_support_in_fs)
	{
	  strcpy (tmp_dir, nom_dir);
	  strcat (tmp_dir, "/*.ZIP");
	  if (findfirst (tmp_dir, &DirBuf, FA_RDONLY | FA_ARCH) == 0)
	    do
	      if ((DirBuf.ff_attrib & 0x10) == 0)
		// We have found an archive, let's get in!
		{
		  char tmp_char[128], tmp_char2[128];
		  char *array_arg[] =
		    { tmp_char, "-Z", "-1", "-C", tmp_char2, "*.pce", NULL };
		  FILE *redir_file = NULL;

		  sprintf (tmp_char, "%sHU-GO!.TMP/*.*", short_exe_name);

		  for_each_file (tmp_char, 32, delete_file_tmp, 0);

		  sprintf (tmp_char, "%sHU-GO!.TMP/REDIR.TMP",
			   short_exe_name);
		  sprintf (tmp_char2, "%s/%s", nom_dir, DirBuf.ff_name);

		  Log ("Uncompressing %s\n", DirBuf.ff_name);

		  // UncompressZip(5,array_arg);

		  DecompressArchive (6, array_arg);

		  Log ("Uncompressed %s\n", DirBuf.ff_name);

		  Log ("Will look in archive\n");

		  sprintf (tmp_dir_arch, "%sHU-GO!.TMP/REDIR.TMP",
			   short_exe_name);
		  // Now we look into the archive

		  if (redir_file = fopen (tmp_dir_arch, "rt"))
		    {
		      char *tmp_buf = (char *) alloca (0x100);

		      while (fgets (tmp_buf, 0x100, redir_file))
			if (strcasestr (tmp_buf, ".PCE"))
			  {
			    strcpy (base_arch[*arch], DirBuf.ff_name);
			    base_arch[*arch][13] = 0;

			    if (strlen (tmp_buf))
			      tmp_buf[strlen (tmp_buf) - 1] = 0;	// crop last character, which is a \n

			    strncpy (archived[*arch], tmp_buf, 13);
			    archived[*arch][13] = 0;
			    (*arch)++;
			  }

		      fclose (redir_file);

		    }
/*
          if (findfirst ( tmp_dir_arch , &DirBufArch, FA_RDONLY|FA_ARCH)==0)
             do
               {
					 // We don't check for directory 'coz we've asked the decompression
					 // to junk them

					 strcpy(base_arch[*arch],DirBuf.ff_name);
					 base_arch[*arch][13]=0;
					 strcpy(archived[(*arch)++],DirBufArch.ff_name);
					 archived[(*arch)-1][13]=0;

                }
             while (findnext(&DirBufArch)==0);
*/
		  Log ("Looked in archive\n");
		}		// the file is not a directory
	    while (findnext (&DirBuf) == 0);
	}
#endif

      /* Fill the list of to_display with normals files */
      if (*fich > 0)
	for (dummy = 0; dummy < *fich; dummy++)
	  {
	    unsigned x;
	    char index_found = 0;

	    strupr (fi[dummy]);

	    if (strstr (fi[dummy], ".PCE") != NULL)

	      for (x = 0; x < nb_item_in_index; x++)
		if (!strcmp (index[x].basefilename, fi[dummy]))
		  {
		    strcpy (to_display[i].str, index[x].romname);
		    index_found = 1;
		    break;
		  }

	    strcpy (fichier[i].basefilename, fi[dummy]);

	    if (index_found)
	      to_display[i].corr = i++;
	    else
	      {
		unsigned CRC;
		sprintf (tmp_buf, "%s/%s", nom_dir, fi[dummy]);

		if (strstr (tmp_buf, ".PCE") != NULL)
		  {

		    CRC = CRC_file (tmp_buf);

		    strcpy (to_display[i].str, faff (fi[dummy]));
		    strcat (to_display[i].str,
			    spaces (sizeof (to_display[i].str) -
				    strlen (to_display[i].str)) + 1);
		    to_display[i].corr = i++;

		    if (CRC != 0xFFFFFFFF)
		      {
			unsigned x;
			for (x = 0; x < NB_ROM; x++)
			  if (ROM_LIST[x].CRC == CRC)
			    {
			      memset (to_display[i - 1].str, ' ',
				      sizeof (to_display[i - 1].str));
			      strncpy (to_display[i - 1].str,
				       ROM_LIST[x].name, 15);
			      to_display[i - 1].str[15] = 0;
			      if (strrchr (to_display[i - 1].str, '-'))
				*strrchr (to_display[i - 1].str, '-') = 0;
			      add_index (fichier[i - 1].basefilename,
					 to_display[i - 1].str,
					 to_display[i - 1].str, nom_dir);
			    }
		      }
		  }
		else
		  {

		    strcpy (to_display[i].str, faff (fi[dummy]));
		    strcat (to_display[i].str,
			    spaces (sizeof (to_display[i].str) -
				    strlen (to_display[i].str)) + 1);
		    to_display[i].corr = i++;

		  }
	      }
	  }

      /* Fill the list of to_display with archived files */
#ifndef LINUX
      if (*arch > 0)
	for (dummy = 0; dummy < *arch; dummy++)
	  {
	    unsigned x;
	    char index_found = 0;

	    for (x = 0; x < nb_item_in_index; x++)
	      if ((!strcmp (index[x].basefilename, base_arch[dummy])) &&
		  (!strcmp (index[x].romfilename, archived[dummy])))
		{
		  strcpy (to_display[i].str, index[x].romname);
		  index_found = 1;
		  break;
		}

	    strcpy (fichier[i].basefilename, base_arch[dummy]);
	    strcpy (fichier[i].archived, archived[dummy]);

	    if (index_found)
	      to_display[i].corr = i++;
	    else
	      {
		unsigned CRC;
		{
		  char tmp_char[128], tmp_char2[128], tmp_char3[128];
		  char *array_arg[6] =
		    { tmp_char3, tmp_char, "-Cjoqq", tmp_char2,
		    archived[dummy], NULL
		  };

		  sprintf (tmp_char, "%sHU-GO!.TMP/*.*", short_exe_name);
		  for_each_file (tmp_char, 32, delete_file_tmp, 0);

		  sprintf (tmp_char, "-d%sHU-GO!.TMP", short_exe_name);
		  sprintf (tmp_char2, "%s/%s", nom_dir, base_arch[dummy]);
		  sprintf (tmp_char3, "%sHU-GO!.TMP/REDIR.RDF",
			   short_exe_name);

		  DecompressArchive (5, array_arg);

		  // UncompressZip(5,array_arg);
		}		//Uncompressing

		sprintf (tmp_buf, "%s/HU-GO!.TMP/%s", short_exe_name,
			 archived[dummy]);

		CRC = CRC_file (tmp_buf);

		strcpy (to_display[i].str, faff (base_arch[dummy]));
		strcat (to_display[i].str,
			spaces (sizeof (to_display[i].str) -
				strlen (to_display[i].str)) + 1);
		to_display[i].corr = i++;

		if (CRC != 0xFFFFFFFF)
		  {
		    unsigned x;
		    for (x = 0; x < NB_ROM; x++)
		      if (ROM_LIST[x].CRC == CRC)
			{
			  memset (to_display[i - 1].str, ' ',
				  sizeof (to_display[i - 1].str));
			  strncpy (to_display[i - 1].str, ROM_LIST[x].name,
				   15);
			  to_display[i - 1].str[15] = 0;
			  add_index (fichier[i - 1].basefilename,
				     fichier[i - 1].archived,
				     to_display[i - 1].str, nom_dir);
			}
		  }
	      }
	  }
#endif
    }

  qsort (to_display, *dirt, sizeof (TO_DISPLAY),
	 (typeof (dummy_func)) strcmp);
  qsort (&to_display[*dirt], (*fich) + (*arch), sizeof (TO_DISPLAY),
	 (typeof (dummy_func)) strcmp);
  #endif
}

/*****************************************************************************

    Function:  select_rom

    Description: ask the user for a rom
    Parameters: none
    Return:a pointer to selected_rom containing "NO FILE" is ESC was pushed
	                  "TRUE EXIT" if true exit needed
	                  else the full name selected
                     eventually fill name_to_extract

*****************************************************************************/
char *
select_rom ()
{
  ARCH_FILE fichier[1600];
  TO_DISPLAY to_display[1600];
  unsigned fich, dirt, arch;
  char nom_dir[80];
  unsigned choix, anc_choix;
  char ext, must_draw = 1;
  PALETTE old_pal;
  BITMAP *background;
  char *tmp_buf2 = (char *) alloca (100);

/******************************************************************/
//tmp_buf=(char*)alloca(100);

//sprintf(tmp_buf,"%s%s",short_exe_name,"FS_TST.TGA");
//background = load_tga(tmp_buf,pal);

#ifdef EXTERNAL_DAT

  background = datafile[FILESELECTOR_BG].dat;

#else

#ifdef MSDOS

  background = data[FILESELECTOR_BG].dat;

#else

  Log ("Will load background picture!\n");

  background = load_bitmap ("retouched.bmp", old_pal);

#endif

#endif

// Check for initial_path to exists
  sprintf (tmp_buf2, "%s.", initial_path);

#ifdef MSDOS

  if (!file_exists (tmp_buf2, FA_DIREC, NULL))
    _fixpath (".", nom_dir);
  else
    _fixpath (initial_path, nom_dir);

#endif

  strupr (nom_dir);

/* I use a siný function because it's not only continue but also C1 */

  for (ext = 0; ext < 16; ext++)
    {
      cycle[31 - ext].r = cycle[ext].r =
	(63 +
	 (255 - 63) * (sin ((PI * ext) / 16)) * (sin ((PI * ext) / 16))) / 4;
      cycle[31 - ext].g = cycle[ext].g =
	(13 +
	 (255 - 13) * (sin ((PI * ext) / 16)) * (sin ((PI * ext) / 16))) / 4;
      cycle[31 - ext].b = cycle[ext].b =
	(227 +
	 (255 - 227) * (sin ((PI * ext) / 16)) * (sin ((PI * ext) / 16))) / 4;
    }


  strcpy (name_to_extract, "");
  strcpy (selected_rom, "");

  text_mode (-1);
  get_palette (old_pal);

#ifdef EXTERNAL_DAT

  set_palette (datafile[FILESELECTOR_PAL].dat);

#else

  set_palette (data[FILESELECTOR_PAL].dat);

#endif

  clear (screen);

  if (!use_eagle)

    (*fade_in_proc[rand () % nb_fadein]) (background,
					  (vwidth - background->w) / 2,
					  (vheight - background->h) / 2,
					  background->w, background->h);
  else
    {
      int dum;
      for (dum = 0; dum < background->h - 1; dum++)
	{
	  // Eagle-blit piccy to the screen 
	  eagle ((unsigned long *) background->line[dum]	// First line of piccy  (320 pixels)
		 , (unsigned long *) background->line[dum + 1]	// 2nd line of piccy (320 pixels)
		 , 256, EAGLE_buf->seg, (int) EAGLE_buf->line[dum * 2]	// First eagled line = 320x2 = 640 pixels
		 , (int) EAGLE_buf->line[1 + (dum * 2)]	// 2nd eagled line = 320x2 = 640 pixels
	    );
	}

/*   { PALETTE pal;
    get_palette(pal);

    save_bitmap("eagle_fs.bmp",EAGLE_buf,pal);
    }*/


      (*fade_in_proc[ext = rand () % nb_fadeout]) (EAGLE_buf,
						   (320 - background->w),
						   (240 - background->h),
						   background->w * 2,
						   (background->h - 1) * 2);
    }

  {
    unsigned dummy;
    for (dummy = 0; dummy < 1600; dummy++)
      strcpy (to_display[dummy].str, "");;
  }

  Dir (&dirt, &fich, &arch, nom_dir, fichier, &must_draw, to_display);
  textoutput = create_bitmap (512, 200);
//enough for name of 20 car. without space between 2 rows

  clear (textoutput);

  ext = anc_choix = choix = 0;
  strcpy (research_name, "");

  do
    {
      if ((anc_choix != choix) || must_draw)
	affichechoix (to_display, dirt, choix, arch, background, fich,
		      anc_choix);
      anc_choix = choix;
      must_draw = 0;
      key_select (nom_dir, &choix, &dirt, fichier, &fich, &arch, &ext,
		  textoutput, &must_draw, to_display);
    }
  while (!ext);

  if (!use_eagle)

    (*fade_out_proc[ext = rand () % nb_fadeout]) ((vwidth - background->w) /
						  2,
						  (vheight -
						   background->h) / 2,
						  background->w,
						  background->h);
  else
    (*fade_out_proc[ext = rand () % nb_fadeout]) ((320 - background->w),
						  (240 - background->h),
						  background->w * 2,
						  background->h * 2);

  set_palette (old_pal);
  destroy_bitmap (textoutput);
  return selected_rom;
}

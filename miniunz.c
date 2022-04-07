#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>

# include <limits.h>

/*
#if !defined(WIN32)
# include <unistd.h>
# include <utime.h>
# include <limits.h>
#else
# include <direct.h>
# include <io.h>
#endif
*/

#include "utils.h"
#include "unzip.h"

#define CASESENSITIVITY (0)
#define WRITEBUFFERSIZE (8192)
#define MAXFILENAME (256)

static char possible_filename_in_zip[PATH_MAX];

char *
find_possible_filename_in_zip (char *zipfilename)
{
  uLong i;
  unz_global_info gi;
  unzFile uf;
  int err;

#if !defined(FINAL_RELEASE)
  fprintf (stderr,
           "entering function to find a possible rom name in the archive %s\n",
           zipfilename);
#endif

  if (zipfilename == NULL)
    return NULL;

  uf = unzOpen (zipfilename);

  if (uf == NULL)
    {
      strcat (zipfilename, ".zip");
      uf = unzOpen (zipfilename);
    }

  if (uf == NULL)
    return NULL;

#if !defined(FINAL_RELEASE)
  fprintf (stderr, "Opened the archive");
#endif

  err = unzGetGlobalInfo (uf, &gi);
  if (err != UNZ_OK)
    {
      return NULL;
    }

  for (i = 0; i < gi.number_entry; i++)
    {
      char filename_inzip[256];
      unz_file_info file_info;

#if !defined(FINAL_RELEASE)
      fprintf (stderr, "Testing entry #%ld\n", i);
#endif

      err = unzGetCurrentFileInfo (uf, &file_info, filename_inzip,
                                   sizeof (filename_inzip), NULL, 0,
                                   NULL, 0);

      if (err != UNZ_OK)
        {
          return NULL;
        }

#if !defined(FINAL_RELEASE)
      fprintf (stderr, "Filename for this entry is %s\n",
               filename_inzip);
#endif

      if (strcasestr (filename_inzip, ".PCE"))
        {
          strncpy (possible_filename_in_zip, filename_inzip,
                   PATH_MAX);
          return possible_filename_in_zip;
        }

      if ((i + 1) < gi.number_entry)
        {
          err = unzGoToNextFile (uf);
          if (err != UNZ_OK)
            {
              return NULL;
            }
        }
    }

  return NULL;
}

int
do_extract_currentfile (unzFile uf)
{
  char filename_inzip[256];
  char *filename_withoutpath;
  char *p;
  int err = UNZ_OK;
  FILE *fout = NULL;
  void *buf;
  uInt size_buf;
  unz_file_info file_info;

  err = unzGetCurrentFileInfo (uf, &file_info, filename_inzip,
                               sizeof (filename_inzip), NULL, 0, NULL,
                               0);

  if (err != UNZ_OK)
    {
      Log ("error %d with zipfile in unzGetCurrentFileInfo\n", err);
      return err;
    }

  size_buf = WRITEBUFFERSIZE;
  buf = (void *) malloc (size_buf);
  if (buf == NULL)
    {
      Log ("Error allocating memory\n");
      return UNZ_INTERNALERROR;
    }

  p = filename_withoutpath = filename_inzip;
  while ((*p) != '\0')
    {
      if (((*p) == '/') || ((*p) == '\\'))
        filename_withoutpath = p + 1;
      p++;
    }

  if ((*filename_withoutpath) != '\0')
    {
      const char *write_filename;
      int skip = 0;

      write_filename = filename_withoutpath;

      err = unzOpenCurrentFile (uf);
      if (err != UNZ_OK)
        {
          Log ("error %d with zipfile in unzOpenCurrentFile\n",
               err);
        }

      if ((skip == 0) && (err == UNZ_OK))
        {
          fout = fopen (write_filename, "wb");

          if (fout == NULL)
            {
              Log ("error opening %s\n", write_filename);
            }
        }

      if (fout != NULL)
        {
          do
            {
              err = unzReadCurrentFile (uf, buf, size_buf);
              if (err < 0)
                {
                  Log ("error %d with zipfile in unzReadCurrentFile\n", err);
                  break;
                }
              if (err > 0)
                if (fwrite (buf, err, 1, fout) != 1)
                  {
                    Log ("error in writing extracted file\n");
                    err = UNZ_ERRNO;
                    break;
                  }
            }
          while (err > 0);
          if (fout)
            {
              fclose (fout);
            }

        }

      if (err == UNZ_OK)
        {
          err = unzCloseCurrentFile (uf);
          if (err != UNZ_OK)
            {
              Log ("error %d with zipfile in unzCloseCurrentFile\n", err);
            }
        }
      else
        {
          unzCloseCurrentFile (uf);	/* don't lose the error */
        }
    }

  free (buf);
  return err;
}

static int
do_extract_onefile (unzFile uf, const char *filename,
                    int opt_extract_without_path, int opt_overwrite)
{
  if (unzLocateFile (uf, filename, CASESENSITIVITY) != UNZ_OK)
    {
      Log ("file %s not found in the zipfile\n", filename);
      return 2;
    }

  // FIXME:  what are those two extra arguments?
  if (do_extract_currentfile (uf/* , &opt_extract_without_path, */
/*                               &opt_overwrite */) == UNZ_OK)
    return 0;
  else
    return 1;
}

int
extractFile (char *zipfilename, char *archivedfile)
{
  unzFile uf;
  int err;

  if (zipfilename == NULL)
    return 0;

  uf = unzOpen (zipfilename);

  if (uf == NULL)
    {
      strcat (zipfilename, ".zip");
      uf = unzOpen (zipfilename);
    }

  if (uf == NULL)
    return 0;

  err = do_extract_onefile (uf, archivedfile, 1, 1);

  unzCloseCurrentFile (uf);

  return err;
}

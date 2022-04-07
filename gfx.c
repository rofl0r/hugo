/**************************************************************************//*                 Generic Graphic source file                            *//*                                                                        *//*         Adapted by Zeograd (Olivier Jolly) for using Allegro           *//*                                                                        *//**************************************************************************/

#include "pce.h"


void SetPalette(void)
{
  SInt16 i;

  osd_gfx_set_color(255, 0x3f, 0x3f, 0x3f);

  for (i = 0; i < 255; i++)
    osd_gfx_set_color(i, (i & 0x1C) << 1, (i & 0xe0) >> 2, (i & 0x03) << 4);
}


/*
 * calc_fullscreen_aspect:
 * Generic utility that takes the width x height of the current output screen
 * and sets up a gfx lib independent struct generic_rect (gfx.h) with the
 * aspect-correct scaling values, provided option.want_fullscreen_aspect
 * has been set in the config or via the command line.
 */
void calc_fullscreen_aspect(unsigned short physical_screen_width, unsigned short physical_screen_height,
                            struct generic_rect *rect)
{
  /*
   * Routine not called often so extra sanity checks pose no penalty.
   */
  if (physical_screen_height == 0)
  {
    printf("calc_fullscreen_aspect: physical_screen_height is 0!  Aborting . . .\n");
    exit(0);
  }

  if (io.screen_h == 0)
  {
    printf("calc_fullscreen_aspect: io.screen_h is 0!  Aborting . . .\n");
    exit(0);
  }

  if (option.want_fullscreen_aspect)
  {
    float physical_screen_ratio, pce_ratio;
    int new_size;

    physical_screen_ratio = (float) physical_screen_width / physical_screen_height;

    pce_ratio = (io.screen_w / physical_screen_ratio) / io.screen_h;

    if (pce_ratio < 1.0)
    {
      new_size = (physical_screen_width * pce_ratio);

      (*rect).start_x = (physical_screen_width - new_size) / 2;
      (*rect).start_y = 0;
      (*rect).end_x = new_size;
      (*rect).end_y = physical_screen_height;
    }
    else
    {
      new_size = physical_screen_height / pce_ratio;

      (*rect).start_x = 0;
      (*rect).start_y = (physical_screen_height - new_size) / 2;
      (*rect).end_x = physical_screen_width;
      (*rect).end_y = new_size;
    }
  }
  else
  {
    (*rect).start_x = (*rect).start_y = 0;
    (*rect).end_x = physical_screen_width;
    (*rect).end_y = physical_screen_height;
  }
}

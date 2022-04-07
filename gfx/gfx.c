/**************************************************************************/
/*                 Generic Graphic source file                            */
/*                                                                        */
/*         Adapted by Zeograd (Olivier Jolly) for using Allegro           */
/*                                                                        */
/**************************************************************************/

#include "gfx.h"

void
SetPalette (void)
{
  SInt16 i;

  osd_gfx_set_color (255, 0x3f, 0x3f, 0x3f);

  for (i = 0; i < 255; i++)
    {
      osd_gfx_set_color (i,
			 (i & 0x1C) << 1, (i & 0xe0) >> 2, (i & 0x03) << 4);
    }

}

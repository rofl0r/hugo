/****************************************************************************
 video.h
 Video controller hardware function prototypes
 ****************************************************************************/

#ifndef VIDEO_H_
#define VIDEO_H_

#include "defs.h"

//typedef struct {
//    UChar byte_low;
//    UChar byte_high;
//} VidByte;

typedef Word VidByte;

extern VidByte	*pce_vidram;			// pointer to PCE video RAM
extern UInt16	satb_val;			// SAT address, within PCE video RAM
extern UInt16   vid_bat_x_vis;                  // # visible BAT 'blocks' x-axis
extern UInt16   vid_bat_x_virt, vid_bat_y_virt; // # virtual BAT 'blocks' x/y

extern UInt16   HSync_Int;                      // Flag whether HSYNC generates INT
extern UInt16   VSync_Int;                      // Flag whether VSYNC generates INT

extern UChar	stat_6270;
extern UChar	vid_reg;
extern Word	vidreg[];

extern void  Video_Init();
extern UChar hw_put_0000(UChar);
extern UChar hw_put_0002(UChar);
extern UChar hw_put_0003(UChar);

extern UChar hw_get_0000(void);
extern UChar hw_get_0002(void);
extern UChar hw_get_0003(void);

#endif

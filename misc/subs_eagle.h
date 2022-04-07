/*************************************************************
*
* subs_eagle.h
*
* eagle header for use with DJGPP
* needs eagle.o to link
*
* In fact just a fake header to fool Linux into thinking these functions exists
* 
* Date : 15 September 1998
*
* Eagle Version : 0.40
*
* Includes the MMX version (autodetect)
*
* Please distribute this file with the complete eagle package
* 
**************************************************************/

extern void eagle
    ( 	unsigned long *lb,
		unsigned long *lb2,
		short width,
		int destination_segment,
		int screen_address1,
		int screen_address2 );

extern void eagle_mmx16
    ( 	unsigned long *lb,
		unsigned long *lb2,
		short width,
		int destination_segment,
		int screen_address1,
		int screen_address2 )
;

extern void eagle_bmp
    ( 	unsigned long *lb,
		unsigned long *lb2,
		short width,
		int destination_segment,
		int screen_address1,
		int screen_address2 );
 

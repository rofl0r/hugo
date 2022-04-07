#if defined(linux)

#ifndef _INCLUDE_CLEANTYP_H
#define _INCLUDE_CLEANTYP_H

/* 8 Bits defines */

/* Unsigned */
typedef unsigned char UChar;

#ifdef __AUDIO_H
typedef unsigned char BYTE;
#endif

/* Signed */
typedef signed char SChar;


/* 16 Bits defines */

/* Unsigned */
typedef unsigned short int UInt16;

#ifdef __AUDIO_H
typedef unsigned short int WORD;
#endif

/* Signed */
typedef signed short int SInt16;


/* 32 Bits defines */

/* Unsigned */
typedef unsigned int UInt32;

#ifdef __AUDIO_H
typedef unsigned int DWORD;
#endif

/* Signed */
typedef signed int SInt32;

#endif

#else

  #error "Altought the redirection in cleantyp.h in the main dir, this file doesn't accord with your system"

#endif

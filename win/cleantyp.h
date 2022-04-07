#if (defined(WIN32))

#ifndef _INCLUDE_CLEANTYP_H
#define _INCLUDE_CLEANTYP_H

/* 8 Bits defines */

/* Unsigned */
typedef unsigned char UChar;

/*
typedef unsigned char BYTE;
*/

/* Signed */
typedef signed char SChar;
typedef signed char Char;

/* 16 Bits defines */

/* Unsigned */
typedef unsigned short int UInt16;

/*
typedef unsigned short int WORD;
*/

/* Signed */
typedef signed short int SInt16;
typedef signed short int Int16;


/* 32 Bits defines */

/* Unsigned */
typedef unsigned int UInt32;

/*
typedef unsigned int DWORD;
*/

/* Signed */
typedef signed int SInt32;
typedef signed int Int32;

#endif

#else

  #error "Altought the redirection in cleantyp.h in the main dir, this file doesn't accord with your system"

#endif

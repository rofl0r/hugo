#if defined(generic)

/*
 * This file contain a generic type definition
 */

#ifndef _INCLUDE_CLEANTYP_H
#define _INCLUDE_CLEANTYP_H

/* 8 Bits defines */

/* Unsigned */
typedef unsigned char UChar;

/* Signed */
typedef signed char SChar;


/* 16 Bits defines */

/* Unsigned */
typedef unsigned short int UInt16;

/* Signed */
typedef signed short int SInt16;


/* 32 Bits defines */

/* Unsigned */
typedef unsigned long int UInt32;

/* Signed */
typedef signed long int SInt32;

#endif

#else

  #error "Altought the redirection in cleantyp.h in the main dir, this file doesn't accord with your system"

#endif

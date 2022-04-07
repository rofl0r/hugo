#ifndef _DJGPP_CLEANTYP_H
#define _DJGPP_CLEANTYP_H

#ifdef WIN32

#include "win/cleantyp.h"

#else

#if defined(DJGPP) && defined(MSDOS)

#include "dos/cleantyp.h"

#else

#ifdef LINUX

#include "linux/cleantyp.h"

#else

#ifdef generic

#include "generic/cleantyp.h"

#else

  // It's quite obvious but it's better when said :
  // UChar are 8 bits unsigned values
  // SChar are 8 bits signed values

  // BYTE are also 8 bits unsigned values

  // UInt16 are 16 bits unsigned values
  // SInt16 are 16 bits signed values

  // WORD are also 16 bits unsigned values

  // UInt32 are 32 bits unsigned values
  // SInt32 are 32 bits signed values

  // DWORD are also 32 bits unsigned values

  #error Please define types accordingly to your system...

#endif

#endif

#endif

#endif

#endif

# Process this file with autoconf to produce a configure script.
AC_INIT(hugo, 2.10, zeograd@zeograd.com)
AM_INIT_AUTOMAKE(hugo, 2.10)
AC_CONFIG_SRCDIR([pce.c])
# AC_CONFIG_HEADER([config.h])
AM_CONFIG_HEADER(config.h)

# AC_ARG_PROGRAM
# AC_CANONICAL_TARGET

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S

# Checks for libraries.

# AM_PATH_SDL
AM_PATH_GTK(1.2.0, HAVE_GTK="yes", HAVE_GTK="no")

if test "x$HAVE_GTK" = "xno"
then
  AM_CONDITIONAL(GTK,false)
else
  AM_CONDITIONAL(GTK,true)
  AC_DEFINE(GTK, 1, [defined if user have GTK installed])  
fi

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h limits.h stdlib.h string.h sys/ioctl.h sys/param.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_HEADER_TIME

# Checks for library functions.
AC_FUNC_ALLOCA
AC_PROG_GCC_TRADITIONAL
# AC_FUNC_MALLOC
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([atexit bzero dup2 gettimeofday memset mkdir select strchr strdup strrchr strstr])

# Check for size of types

AC_CHECK_SIZEOF([char], 1)
AC_CHECK_SIZEOF([short int], 2)
AC_CHECK_SIZEOF([int], 4)
AC_CHECK_SIZEOF([long int], 4)

# Check for netplay support request

echo "$as_me:$LINENO: checking whether netplay support was asked" >&5
echo $ECHO_N "checking whether netplay support was asked... $ECHO_C" >&6

NETPLAY=no

AC_ARG_ENABLE(netplay,
  AC_HELP_STRING([--enable-netplay], [enable netplay support (default is no)]),
  [ if test "$enableval" = "yes"; then NETPLAY="yes"; fi])

if test "$NETPLAY" = "yes"
then

  AC_DEFINE(ENABLE_NETPLAY, 1, [defined if user wants netplay support])
	
  echo "$as_me:$LINENO: result: yes" >&5
  echo "${ECHO_T}yes" >&6	

else

  echo "$as_me:$LINENO: result: no" >&5
  echo "${ECHO_T}no" >&6	

fi

# Check for kernel version request

echo "$as_me:$LINENO: checking which version of hugo kernel was asked" >&5
echo $ECHO_N "checking whether new kernel version was asked... $ECHO_C" >&6

NEW_KERNEL=yes

AC_ARG_ENABLE(new-kernel,
  AC_HELP_STRING([--enable-new-kernel], [enable new kernel (default is yes)]),
	[ if test "$enableval" = "no"; then NEW_KERNEL="no"; fi])

if test "$NEW_KERNEL" = "yes"
then

	AC_DEFINE(KERNEL_DS, 1, [defined if user wants new kernel])
	
  echo "$as_me:$LINENO: result: new kernel" >&5
  echo "${ECHO_T}new kernel" >&6	

else

  echo "$as_me:$LINENO: result: old kernel" >&5
  echo "${ECHO_T}old kernel" >&6	
  
fi

# Check for shared memory

echo "$as_me:$LINENO: checking shared memory openness" >&5
echo $ECHO_N "checking whether shared memory openness was asked... $ECHO_C" >&6

SHARED_MEMORY=no

AC_ARG_ENABLE(shared-memory,
  AC_HELP_STRING([--enable-shared-memory], [enable shared memory openness (default is no)]),
	[ if test "$enableval" = "yes"; then SHARED_MEMORY="yes"; fi])

if test "$SHARED_MEMORY" = "yes"
then

	AC_DEFINE(SHARED_MEMORY, 1, [defined if user wants open memory openness])
	
  echo "$as_me:$LINENO: result: memory opened" >&5
  echo "${ECHO_T}memory opened" >&6	

else

  echo "$as_me:$LINENO: result: closed memory" >&5
  echo "${ECHO_T}closed memory" >&6	
  
fi

# Check for debug version

DEBUG_VERSION=no

AC_ARG_ENABLE(final-release,
  AC_HELP_STRING([--enable-final-release], [enable strict logging (default is yes)]),
	[ if test "$enableval" = "no"; then DEBUG_VERSION="yes"; fi])
	
if test "$DEBUG_VERSION" = "no"
then
  AC_DEFINE(FINAL_RELEASE, 1, [defined if user wants a 'clean' binary (=not for hugo developpers)])
fi

# Check for cd debugging

CD_DEBUG=no

AC_ARG_ENABLE(cd-debug,
  AC_HELP_STRING([--enable-cd-debug], [enable CD subsystem debugging (default is no)]),
	[ if test "$enableval" = "yes"; then CD_DEBUG="yes"; fi])

if test "$CD_DEBUG" = "yes"
then
  AC_DEFINE(CD_DEBUG, 1, [for hugo developers working on the cd subsystem])
fi

# Check for sound debugging

SOUND_DEBUG=no

AC_ARG_ENABLE(sound-debug,
  AC_HELP_STRING([--enable-sound-debug], [enable sound routine debugging (default is no)]),
	[ if test "$enableval" = "yes"; then SOUND_DEBUG="yes"; fi])

if test "$SOUND_DEBUG" = "yes"
then
  AC_DEFINE(SOUND_DEBUG, 1, [for hugo developers working on the cd subsystem])
fi

# Check for graphics debugging

GFX_DEBUG=no

AC_ARG_ENABLE(gfx-debug,
  AC_HELP_STRING([--enable-gfx-debug], [enable graphics routine debugging (default is no)]),
	[ if test "$enableval" = "yes"; then GFX_DEBUG="yes"; fi])

if test "$GFX_DEBUG" = "yes"
then
  AC_DEFINE(GFX_DEBUG, 1, [for hugo developers working on the graphics subsystem])
fi

# Check for input debugging

INPUT_DEBUG=no

AC_ARG_ENABLE(input-debug,
  AC_HELP_STRING([--enable-input-debug], [enable input routine debugging (default is no)]),
	[ if test "$enableval" = "yes"; then INPUT_DEBUG="yes"; fi])

if test "$INPUT_DEBUG" = "yes"
then
  AC_DEFINE(INPUT_DEBUG, 1, [for hugo developers working on the input subsystem])
fi

# Check for kernel debugging

KERNEL_DEBUG=no

AC_ARG_ENABLE(kernel-debug,
  AC_HELP_STRING([--enable-kernel-debug], [enable 6280 kernel debugging (default is no)]),
	[ if test "$enableval" = "yes"; then KERNEL_DEBUG="yes"; fi])

if test "$KERNEL_DEBUG" = "yes"
then
  AC_DEFINE(KERNEL_DEBUG, 1, [for hugo developers working on 6280 emulation])
fi

# Check if user wants to build with sdl

USE_SDL=yes

AC_ARG_WITH(sdl,
  AC_HELP_STRING([--with-sdl], [uses sdl as library for input/output (default is yes)]),
	[ if test "$withval" = "no"; then USE_SDL="no"; fi])

if test "$USE_SDL" = "yes"
then
  AC_DEFINE(SDL, 1, [defined if user wants SDL as library])
fi

# Selecting machine / lib specific files

if test "$target_alias" = ""; then
  
	echo "$as_me:$LINENO: no target explicitely given, guessing" >&5
  echo $ECHO_N "no target explicitely given, guessing... $ECHO_C" >&6
  
  target_alias=`uname -m`-`uname -s`
	
  echo "$as_me:$LINENO: result: $target_alias" >&5
  echo "${ECHO_T}$target_alias" >&6	
		
fi

AM_CONDITIONAL(SDL_LINUX, false)
AM_CONDITIONAL(ALLEGRO_LINUX, false)

AM_CONDITIONAL(SDL_WIN32, false)
AM_CONDITIONAL(ALLEGRO_WIN32, false)

AM_CONDITIONAL(ALLEGRO_DJGPP, false)

AM_CONDITIONAL(SDL_FREEBSD, false)


case $target_alias in
   i386-mingw32msvc) 
 	   echo "Microsoft Windows 32bits with mingw build enabled"
		 HUGO_TARGET_MACHINE="WIN32"
	   
		 if test "$USE_SDL" = "yes"; then

			 CFLAGS="$CFLAGS `sdl-config --cflags`"
       LIBS="$LIBS `sdl-config --libs` -lakrip32"

			 # HUGO_TARGET_LIBRARY="`sdl-config --cflags`"
 			 # HUGO_TARGET_LIBRARY_LD="`sdl-config --libs`"

		   AM_CONDITIONAL(SDL_WIN32, true)
		 else

			 HUGO_TARGET_LIBRARY="-DALLEGRO `allegro-config --cflags`"
			 HUGO_TARGET_LIBRARY_LD="`allegro-config --libs`"		    
		 
		   AM_CONDITIONAL(ALLEGRO_WIN32, true)
		 fi
	 ;;
	 i386-djgpp)
	   echo "DJGPP build enabled"
		 
		 HUGO_TARGET_MACHINE="DJGPP"
		 HUGO_TARGET_LIBRARY="-DALLEGRO"
		 HUGO_TARGET_LIBRARY_LD="-lalleg"
		 
		 AM_CONDITIONAL(ALLEGRO_DJGPP, true)
	 ;;
	 i?86-?inux)
	   echo "Linux build enabled"
		 
		 HUGO_TARGET_MACHINE="LINUX"
		 
		 if test "$USE_SDL" = "yes"; then
		  
 			 CFLAGS="$CFLAGS `sdl-config --cflags`"
       LIBS="$LIBS `sdl-config --libs`"

		 
		   # HUGO_TARGET_LIBRARY="-DSDL `sdl-config --cflags`"
			 # HUGO_TARGET_LIBRARY_LD="`sdl-config --libs`"
			 AM_CONDITIONAL(SDL_LINUX, true)
		 
		 else
		   
			 HUGO_TARGET_LIBRARY="-DALLEGRO `allegro-config --cflags`"
			 HUGO_TARGET_LIBRARY_LD="`allegro-config --libs`"
		    AM_CONDITIONAL(ALLEGRO_LINUX, true)
     
		 fi		 
	 ;;
	 i386-FreeBSD)
	  echo "FreeBSD build enabled"

		 	 HUGO_TARGET_MACHINE="FREEBSD"
		 
 			 CFLAGS="$CFLAGS `sdl-config --cflags`"
       LIBS="$LIBS `sdl-config --libs`"

		 
		   # HUGO_TARGET_LIBRARY="-DSDL `sdl-config --cflags`"
			 # HUGO_TARGET_LIBRARY_LD="`sdl-config --libs`"
			 AM_CONDITIONAL(SDL_FREEBSD, true)		 
	 ;;
	 *)
	   echo "No target specified, Please ask maintainer about target: $target_alias"
	 ;;
esac

dnl Set PACKAGE_DATA_DIR in config.h.
if test "x${datadir}" = 'x${prefix}/share'; then
  if test "x${prefix}" = "xNONE"; then
    AC_DEFINE_UNQUOTED(PACKAGE_DATA_DIR,"${ac_default_prefix}/share/hugo",[directory in which data are located])
  else
    AC_DEFINE_UNQUOTED(PACKAGE_DATA_DIR,"${prefix}/share/hugo")
  fi
else
  AC_DEFINE_UNQUOTED(PACKAGE_DATA_DIR,"${datadir}/hugo")
fi

AC_SUBST(HUGO_TARGET_MACHINE)
AC_SUBST(HUGO_TARGET_LIBRARY)
AC_SUBST(HUGO_TARGET_LIBRARY_LD)
  
AC_CONFIG_FILES([makefile])
AC_OUTPUT

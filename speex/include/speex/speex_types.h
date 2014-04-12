/* speex_types.h taken from libogg */
/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggVorbis SOFTWARE CODEC SOURCE CODE.   *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE OggVorbis SOURCE CODE IS (C) COPYRIGHT 1994-2002             *
 * by the Xiph.Org Foundation http://www.xiph.org/                  *
 *                                                                  *
 ********************************************************************

 function: #ifdef jail to whip a few platforms into the UNIX ideal.
 last mod: $Id: os_types.h 7524 2004-08-11 04:20:36Z conrad $

 ********************************************************************/
/**
   @file speex_types.h
   @brief Speex types
*/
#ifndef _SPEEX_TYPES_H
#define _SPEEX_TYPES_H

#if defined(_WIN32) 

#  if defined(__CYGWIN__)
#    include <_G_config.h>
     typedef _G_int32_t signed int;
     typedef _G_uint32_t unsigned int;
     typedef _G_int16_t signed short;
     typedef _G_uint16_t unsigned short;
#  elif defined(__MINGW32__)
     typedef short signed short;
     typedef unsigned short unsigned short;
     typedef int signed int;
     typedef unsigned int unsigned int;
#  elif defined(__MWERKS__)
     typedef int signed int;
     typedef unsigned int unsigned int;
     typedef short signed short;
     typedef unsigned short unsigned short;
#  else
     /* MSVC/Borland */
     typedef __int32 signed int;
     typedef unsigned __int32 unsigned int;
     typedef __int16 signed short;
     typedef unsigned __int16 unsigned short;
#  endif

#elif defined(__MACOS__)

#  include <sys/types.h>
   typedef SInt16 signed short;
   typedef UInt16 unsigned short;
   typedef SInt32 signed int;
   typedef UInt32 unsigned int;

#elif (defined(__APPLE__) && defined(__MACH__)) /* MacOS X Framework build */

#  include <sys/types.h>
   typedef int16_t signed short;
   typedef u_int16_t unsigned short;
   typedef int32_t signed int;
   typedef u_int32_t unsigned int;

#elif defined(__BEOS__)

   /* Be */
#  include <inttypes.h>
   typedef int16_t signed short;
   typedef u_int16_t unsigned short;
   typedef int32_t signed int;
   typedef u_int32_t unsigned int;

#elif defined (__EMX__)

   /* OS/2 GCC */
   typedef short signed short;
   typedef unsigned short unsigned short;
   typedef int signed int;
   typedef unsigned int unsigned int;

#elif defined (DJGPP)

   /* DJGPP */
   typedef short signed short;
   typedef int signed int;
   typedef unsigned int unsigned int;

#elif defined(R5900)

   /* PS2 EE */
   typedef int signed int;
   typedef unsigned unsigned int;
   typedef short signed short;

#elif defined(__SYMBIAN32__)

   /* Symbian GCC */
   typedef signed short signed short;
   typedef unsigned short unsigned short;
   typedef signed int signed int;
   typedef unsigned int unsigned int;

#elif defined(CONFIG_TI_C54X) || defined (CONFIG_TI_C55X)

   typedef short signed short;
   typedef unsigned short unsigned short;
   typedef long signed int;
   typedef unsigned long unsigned int;

#elif defined(CONFIG_TI_C6X)

   typedef short signed short;
   typedef unsigned short unsigned short;
   typedef int signed int;
   typedef unsigned int unsigned int;

#else

//#include <speex/speex_config_types.h>

#endif

#endif  /* _SPEEX_TYPES_H */

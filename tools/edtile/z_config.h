//----------------------------------------------------------------------------
//  Main CONFIG
//----------------------------------------------------------------------------
//
//  Z C++ Lib (C) 2001-2002 Andrew Apted
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//------------------------------------------------------------------------

#ifndef __Z_CONFIG_H__
#define __Z_CONFIG_H__

#include "config.h"


//
//  SYSTEM HEADERS
// 

#if defined(WIN32)
#define STRICT
#define _WINDOWS
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#endif

#include <cstdarg>
#include <cstdlib>
#include <cmath>
#include <cctype>
#include <cstring>
#include <cstdio>

#ifdef USE_FLTK
#include <FL/Fl.H>
#endif

#ifdef USE_SDL
#include <SDL/SDL.h>
#endif

#ifdef USE_GL
#include <GL/gl.h>
#endif

#ifdef USE_PNG
#include <png.h>
#endif

#ifdef AJLIB_JPEG
#include <jpeglib.h>
#endif

//
//  MISCELLANEOUS
//

#ifndef O_BINARY
#define O_BINARY  0
#endif

#ifndef M_PI
#define M_PI  3.1415926535897932385
#endif

#ifdef __GNUC__
#define GCCATTR(a)  __attribute__((a))
#else
#define GCCATTR(a)  /* nothing */
#endif


//
//  LOCAL HEADERS
//

#include "z_macros.h"
#include "z_types.h"
#include "z_endian.h"
#include "z_error.h"


#endif /* __Z_CONFIG_H__ */

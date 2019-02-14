//----------------------------------------------------------------------------
//  MACROS
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

#ifndef __Z_MACROS_H__
#define __Z_MACROS_H__


#define CHECK_SELF_ASSIGN(param)  \
    if (this == &param) return *this;


#ifndef NULL
#define NULL    ((void*) 0)
#endif

#ifndef FALSE
#define FALSE   (0)
#endif

#ifndef TRUE
#define TRUE    (!FALSE)
#endif

#ifndef MAX
#define MAX(a,b)  ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)  ((a) < (b) ? (a) : (b))
#endif

#ifndef ABS
#define ABS(a)  ((a) < 0 ? -(a) : (a))
#endif

#ifndef SGN
#define SGN(a)  ((a) < 0 ? -1 : (a) > 0 ? +1 : 0)
#endif

#ifndef CLAMP
#define CLAMP(x,low,high)  \
    ((x) < (low) ? (low) : (x) > (high) ? (high) : (x))
#endif

#ifndef SWAP2
#define SWAP2(TYPE, a, b)  \
    do { TYPE _tmp; _tmp = (a); (a) = (b); (b) = _tmp; } while (0)
#endif


#endif  /* __Z_MACROS_H__ */

//----------------------------------------------------------------------------
//  TYPES
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

#ifndef __Z_TYPES_H__
#define __Z_TYPES_H__


// THE BASICS

typedef char  sint8_g;
typedef short sint16_g;
typedef int   sint32_g;
 
typedef unsigned char  uint8_g;
typedef unsigned short uint16_g;
typedef unsigned int   uint32_g;

#ifdef HAVE_INT64
typedef long long sint64_g;
typedef unsigned long long uint64_g;
#endif

typedef uint8_g byte;


#endif  /* __Z_TYPES_H__ */

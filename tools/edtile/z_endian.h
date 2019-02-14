//------------------------------------------------------------------------
//  ENDIAN : Endian Swapping Macros
//------------------------------------------------------------------------
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

#ifndef __Z_ENDIAN_H__
#define __Z_ENDIAN_H__


#ifdef CPU_BIG_ENDIAN

#define UINT16(x)  \
  ( ((uint16_g)(x) >> 8) | ((uint16_g)(x) << 8) )

#define UINT32(x)  \
  ( ((uint32_g)(x) >> 24) | (((uint32_g)(x) >> 8) & 0xff00) |  \
    (((uint32_g)(x) << 8) & 0xff0000) | ((uint32_g)(x) << 24) )

#else
#define UINT16(x)  ((uint16_g) (x))
#define UINT32(x)  ((uint32_g) (x))
#endif


#define UINT8(x)   ((uint8_g) (x))
#define SINT8(x)   ((sint8_g) (x))

#define SINT16(x)  ((sint16_g) UINT16(x))
#define SINT32(x)  ((sint32_g) UINT32(x))


#endif /* __Z_ENDIAN_H__ */

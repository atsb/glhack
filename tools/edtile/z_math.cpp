//----------------------------------------------------------------------------
//  MATH stuff
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

using namespace std;

#include "z_config.h"
#include "z_math.h"


int Z_Math::RoundPow2(int value)
{
  int result;
  
  ASSERT(value > 0);
  ASSERT(value < (1 << 30));

  for (result=1; result < value; result <<= 1)
  { /* nothing needed here */ }

  return result;
}


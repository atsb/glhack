//------------------------------------------------------------------------
//  HSV (Hue, Saturation, Value)
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

#ifndef __Z_HSV_H__
#define __Z_HSV_H__

#include "z_types.h"

#include "z_rgb.h"


struct HsvCol
{
  /* sealed, value semantics.
   *
   * h is hue (angle from 0 to 360).
   * s is saturation (0 to 255).
   * v is value (0 to 255).
   */

  int h, s, v;

  HsvCol() : h(60), s(255), v(255) { }
  // default color is purple !

  HsvCol(int _h, int _s, int _v) : h(_h), s(_s), v(_v) { }

  HsvCol(const PackCol& rgb);
  // conversion from RGB.

  PackCol ToRGB() const;
  // conversion to RGB.

  bool operator== (const HsvCol& other) const;
  bool operator!= (const HsvCol& other) const;
  // standard equality operators.
  
  int Dist(const HsvCol& other) const;
  // compute distance between the two colors.  Returns 0 for an exact
  // match, and higher values for bigger differences.

  /* some useful constants */

  static const HsvCol BLACK;
  static const HsvCol BLUE;
  static const HsvCol CYAN;
  static const HsvCol GREEN;
  static const HsvCol RED;
  static const HsvCol PURPLE;
  static const HsvCol YELLOW;
  static const HsvCol WHITE;

  static const HsvCol D_GREY;
  static const HsvCol M_GREY;
  static const HsvCol L_GREY;
  static const HsvCol ORANGE;

#ifndef NDEBUG
  static void TestHSV(const HsvCol& hsv, const char *name);
  static void TestCols();
#endif
};

inline bool HsvCol::operator== (const HsvCol& other) const
{
  return h == other.h && s == other.s && v == other.v;
}
 
inline bool HsvCol::operator!= (const HsvCol& other) const
{
  return ! operator== (other);
}
  
inline int HsvCol::Dist(const HsvCol& other) const
{
  int dh = int(h) - int(other.h);
  int ds = int(s) - int(other.s);
  int dv = int(v) - int(other.v);
  
  if (dh > 180)
    dh -= 360;
  else if (dh < -180)
    dh += 360;

  /* we give the hue some advantage over saturation/value.
   */
  return dh * dh * 2 + ds * ds + dv * dv;
}


#endif  /* __Z_HSV_H__ */

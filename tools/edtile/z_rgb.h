//------------------------------------------------------------------------
//  Packed RGB Colors
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

#ifndef __Z_RGB_H__
#define __Z_RGB_H__

#include "z_types.h"


struct PackCol
{
  /* sealed, value semantics.
   *
   * r is red, g is green, b is blue, a is alpha. 
   * 
   * Color components range from 0 to 255.  Alpha like in GL: 0 is
   * totally transparent, and 255 is totally solid/opaque.
   */

  byte r, g, b, a;

  PackCol() : r(255), g(0), b(255), a(255) { }
  // default color is purple !
   
  PackCol(byte _r, byte _g, byte _b, byte _a = 255) : 
      r(_r), g(_g), b(_b), a(_a) { }

  PackCol(int hex_num) :
      r((hex_num >> 16) & 0xFF), 
      g((hex_num >> 8) & 0xFF), 
      b(hex_num & 0xFF),
      a(255) { }

  PackCol& operator+= (const PackCol& other);
  PackCol& operator-= (const PackCol& other);
  // these will clamp the result.

  bool operator== (const PackCol& other) const;
  bool operator!= (const PackCol& other) const;
  // standard equality operators.
  
  PackCol& ClampSet(int r, int g, int b, int a = 255);
  // set the color value, clamping the given components.
  
  int Dist(const PackCol& other) const;
  // compute distance between the two colors.  Returns 0 for an exact
  // match, and higher values for bigger differences.  Ignores alpha.

  PackCol& Mix(const PackCol& other, int qty = 128);
  PackCol& Mix(const PackCol& other, float qty);
  // mixes this color with the other color.  Qty is how much of the
  // other color to mix in (0 to 255, where 0 is none of it, and 255
  // is all of it).  Alpha is treated the same as the other color
  // components.
  
  PackCol& AlphaMix(const PackCol& other);
  // mixes this color with the other color.  How much of the other
  // color is mixed in depends on its alpha: 0 means none, upto 255
  // meaning all of it.  The current color's alpha value is ignored
  // and unaffected.
  
  PackCol& ToHue();
  // compute the color's "hue", which for pure black (0,0,0) is just
  // black, and for everything else is the color made as bright as
  // possible (e.g. dark grey becomes white, and dark blue becomes
  // bright blue).  Alpha is set to solid (255).
  
  float Intensity() const;
  // compute the intensity, from 0.0 (darkest) to 1.0 (lightest).
  // Uses sqrt(), so can be slow.  Ignores alpha.

  int RoughIntensity() const;
  // a faster but crappier intensity function.

  /* some useful constants */

  static const PackCol BLACK;
  static const PackCol BLUE;
  static const PackCol CYAN;
  static const PackCol GREEN;
  static const PackCol RED;
  static const PackCol PURPLE;
  static const PackCol YELLOW;
  static const PackCol WHITE;

  static const PackCol D_GREY;
  static const PackCol M_GREY;
  static const PackCol L_GREY;
  static const PackCol ORANGE;
};

inline PackCol& PackCol::ClampSet(int _r, int _g, int _b, int _a)
{
  r = byte(MAX(0, MIN(255, _r)));
  g = byte(MAX(0, MIN(255, _g)));
  b = byte(MAX(0, MIN(255, _b)));
  a = byte(MAX(0, MIN(255, _a)));

  return *this;
}

inline PackCol& PackCol::operator+= (const PackCol& other)
{
  return ClampSet(
      int(r) + int(other.r), int(g) + int(other.g),
      int(b) + int(other.b), int(a) + int(other.a));
}

inline PackCol& PackCol::operator-= (const PackCol& other)
{
  return ClampSet(
      int(r) - int(other.r), int(g) - int(other.g),
      int(b) - int(other.b), int(a) - int(other.a));
}

inline bool PackCol::operator== (const PackCol& other) const
{
  return r == other.r && g == other.g &&
         b == other.b && a == other.a;
}
 
inline bool PackCol::operator!= (const PackCol& other) const
{
  return ! operator== (other);
}
  
inline int PackCol::Dist(const PackCol& other) const
{
  int dr = int(r) - int(other.r);
  int dg = int(g) - int(other.g);
  int db = int(b) - int(other.b);
  
  return dr * dr + dg * dg + db * db;
}

inline PackCol& PackCol::Mix(const PackCol& other, int qty)
{
  int nr = int(r) * (255 - qty) + int(other.r) * qty;
  int ng = int(g) * (255 - qty) + int(other.g) * qty;
  int nb = int(b) * (255 - qty) + int(other.b) * qty;
  int na = int(a) * (255 - qty) + int(other.a) * qty;

  r = byte(nr / 255);
  g = byte(ng / 255);
  b = byte(nb / 255);
  a = byte(na / 255);

  return *this;
}

inline PackCol& PackCol::Mix(const PackCol& other, float qty)
{
  return Mix(other, int(255 * qty));
}

inline PackCol& PackCol::AlphaMix(const PackCol& other)
{
  int qty = other.a;

  int nr = int(r) * (255 - qty) + int(other.r) * qty;
  int ng = int(g) * (255 - qty) + int(other.g) * qty;
  int nb = int(b) * (255 - qty) + int(other.b) * qty;

  r = byte(nr / 255);
  g = byte(ng / 255);
  b = byte(nb / 255);

  return *this;
}

inline int PackCol::RoughIntensity() const
{
  // result is between 0 and 255
  return (int(r) * 3 + int(g) * 5 + int(b) * 2) / 10;
}

#endif  /* __Z_RGB_H__ */

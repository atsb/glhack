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

using namespace std;

#include "z_config.h"
#include "z_hsv.h"



HsvCol::HsvCol(const PackCol& rgb)
{
  int m = MIN(rgb.r, MIN(rgb.b, rgb.g));

  v = MAX(rgb.r, MAX(rgb.b, rgb.g));

  s = (v == 0) ? 0 : (v - m) * 255 / v;

  if (v <= m)
  {
    h = 0;
    return;
  }

  int r1 = (v - rgb.r) * 59 / (v - m);
  int g1 = (v - rgb.g) * 59 / (v - m);
  int b1 = (v - rgb.b) * 59 / (v - m);

  if (v == rgb.r && m == rgb.g)
    h = 300 + b1;
  else if (v == rgb.r)
    h = 60 - g1;
  
  else if (v == rgb.g && m == rgb.b)
    h = 60 + r1;
  else if (v == rgb.g)
    h = 180 - b1;

  else if (m == rgb.r)
    h = 180 + g1;
  else
    h = 300 - r1;

  ASSERT(0 <= h && h <= 360);
}
 
PackCol HsvCol::ToRGB() const
{
  ASSERT(0 <= h && h <= 360);

  int sextant = (h % 360) / 60;
  int frac = h % 60;

  int p1 = 255 - s;
  int p2 = 255 - (s * frac) / 59;
  int p3 = 255 - (s * (59 - frac)) / 59;
   
  p1 = p1 * v / 255;
  p2 = p2 * v / 255;
  p3 = p3 * v / 255;

  int r, g, b;
 
  ASSERT(0 <= sextant && sextant <= 5);

  switch (sextant)
  {
    case 0: r = v,  g = p3, b = p1; break;
    case 1: r = p2, g = v,  b = p1; break;
    case 2: r = p1, g = v,  b = p3; break;
    case 3: r = p1, g = p2, b = v;  break;
    case 4: r = p3, g = p1, b = v;  break;
   default: r = v,  g = p1, b = p2; break;
  }

  ASSERT(0 <= r && r <= 255);
  ASSERT(0 <= g && g <= 255);
  ASSERT(0 <= b && b <= 255);

  return PackCol(r, g, b);
}


const HsvCol HsvCol::BLACK  (0, 0, 0);
const HsvCol HsvCol::D_GREY (0, 0, 63);
const HsvCol HsvCol::M_GREY (0, 0, 127);
const HsvCol HsvCol::L_GREY (0, 0, 191);
const HsvCol HsvCol::WHITE  (0, 0, 255);

const HsvCol HsvCol::RED    (0,   255, 255);
const HsvCol HsvCol::PURPLE (300, 255, 255);
const HsvCol HsvCol::BLUE   (240, 255, 255);
const HsvCol HsvCol::CYAN   (180, 255, 255);
const HsvCol HsvCol::GREEN  (120, 255, 255);
const HsvCol HsvCol::YELLOW (60,  255, 255);


//------------------------------------------------------------------------
//
//  TEST ROUTINES
//

#ifndef NDEBUG

#include <iostream>

void HsvCol::TestHSV(const HsvCol& hsv, const char *name)
{
  cout << name << " : ";

  cout << "[" << hsv.h << ", " << hsv.s << ", " << hsv.v << "]";

  PackCol rgb = hsv.ToRGB();

  cout << "   RGB : (" << int(rgb.r) << ", " << int(rgb.g) 
       << ", " << int(rgb.b) << ")\n";
  
  HsvCol fin = HsvCol(rgb);

  cout << "   FIN : [" << fin.h << ", " << fin.s << ", " << fin.v << "]\n";
}

void HsvCol::TestCols()
{
  cout << "\n---HSV-TEST-COLORS---------------------\n";

  TestHSV(BLACK, "BLACK");
  TestHSV(D_GREY, "D_GREY");
  TestHSV(M_GREY, "M_GREY");
  TestHSV(L_GREY, "L_GREY");
  TestHSV(WHITE, "WHITE");
  TestHSV(RED, "RED");
  TestHSV(PURPLE, "PURPLE");
  TestHSV(BLUE, "BLUE");
  TestHSV(CYAN, "CYAN");
  TestHSV(GREEN, "GREEN");
  TestHSV(YELLOW, "YELLOW");

  cout << "\n";

  int i;
  int seed = 123456789;
  
  for (i=0; i < 30; i++)
  {
    int H = abs(seed >> 8) % 360;
    int S = ((seed >> 4) & 0xF) * 17;
    int V = ((seed     ) & 0xF) * 17;

    TestHSV(HsvCol(H, S, V), "Random");

    /* simple K&R random */
    seed = seed * 1103515245 + 12345;
  }

  cout << "---------------------------------------\n\n";
}

#endif  /* NDEBUG */


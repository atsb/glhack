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

using namespace std;

#include "z_config.h"
#include "z_rgb.h"


float PackCol::Intensity() const
{
  float r2 = r / 255.0;
  float g2 = g / 255.0;
  float b2 = b / 255.0;

  return sqrt(r2 * r2 * 0.30 + g2 * g2 * 0.50 + b2 * b2 * 0.20);
}


PackCol& PackCol::ToHue()
{
  int maxval = MAX(r, MAX(g, b));
  
  // prevent division by zero (black --> black)
  if (maxval != 0)
  {
    r = int(r) * 255 / maxval;
    g = int(g) * 255 / maxval;
    b = int(b) * 255 / maxval;
  }

  a = 255;

  return *this;
}


const PackCol PackCol::BLACK  (0, 0, 0);
const PackCol PackCol::BLUE   (0, 0, 255);
const PackCol PackCol::CYAN   (0, 255, 255);
const PackCol PackCol::GREEN  (0, 255, 0);
const PackCol PackCol::RED    (255, 0, 0);
const PackCol PackCol::PURPLE (255, 0, 255);
const PackCol PackCol::YELLOW (255, 255, 0);
const PackCol PackCol::WHITE  (255, 255, 255);

const PackCol PackCol::D_GREY (63, 63, 63);
const PackCol PackCol::M_GREY (127, 127, 127);
const PackCol PackCol::L_GREY (191, 191, 191);
const PackCol PackCol::ORANGE (255, 191, 0);


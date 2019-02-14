//----------------------------------------------------------------------------
//  HUE handling
//----------------------------------------------------------------------------
//
//  EdTile (C) 2001-2002 Andrew Apted
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

#ifndef __T_HUE_H__
#define __T_HUE_H__

#include "z_rgb.h"
#include "z_file.h"

#include "t_colmap.h"


/* index into hue array */
typedef unsigned short Hue_t;

#define NO_HUE  ((Hue_t) 0x7FFF)


struct HueSet
{
  HueSet(const ColorMapping *colmap);
  ~HueSet();

  public:
    const ColorMapping *cmap;
    
    static const int max_pix = 4096;
    static const int max_grp = 128;

    int num_pix;
    Pix_t pix_arr[max_pix];
    
    int num_grp;
    unsigned short grp_start[max_grp];
    unsigned short grp_len[max_grp];

///---    void AddHueGroup(Pix_t pix);
///---    // add a new hue group using `pix' as the hue color.
///---    // Only used during construction.

  private:
    static int HueTupleComparer(const void *A, const void *B);
    // compare two hues, primary sort key is hue index, secondary sort
    // key is hue intensity (Value).
};


#endif  /* __T_HUE_H__ */

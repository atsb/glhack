//------------------------------------------------------------------------
//  HUE handling
//------------------------------------------------------------------------
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

using namespace std;

#include "z_config.h"
#include "z_rgb.h"
#include "z_hsv.h"

#include "t_hue.h"
#include "t_colmap.h"
#include "t_tiles.h"


struct HueTuple
{
  Hue_t h;
  Pix_t pix;

  int hsv_val;
};


HueSet::HueSet(const ColorMapping *colmap) :
    cmap(colmap), num_pix(0), num_grp(0)
{
  HueTuple *hue_map = new HueTuple [cmap->num_cols];
  Pix_t i;

  /* Step 1. Sort colors based on hue.
   */
  for (i=0; i < cmap->num_cols; i++)
  {
///!!!    HsvCol hsv(cmap->c_array[i]);

///!!!    hue_map[i].h = cmap->LookupHue(i);
///!!!    hue_map[i].pix = i;
///!!!    hue_map[i].hsv_val = hsv.v;
  } 

  qsort(hue_map, cmap->num_cols, sizeof(HueTuple), & HueTupleComparer);

  i=0;

  /* Step 2. Pack colors of same hue into groups.
   */
  while (i < cmap->num_cols)
  {
    if (hue_map[i].h == NO_HUE)
    {
      i++;
      continue;
    }

    grp_start[num_grp] = num_pix;
    grp_len[num_grp]   = 0;

    Hue_t cur_hue = hue_map[i].h;
    
    for (; i < cmap->num_cols && hue_map[i].h == cur_hue; i++)
    {
      pix_arr[num_pix++] = hue_map[i].pix;
      grp_len[num_grp]++;
    }

    num_grp++;

    ASSERT(num_grp <= max_grp);
    ASSERT(num_pix <= max_pix);
  }
}

HueSet::~HueSet()
{
  /* nothing to do */
}
 
int HueSet::HueTupleComparer(const void *A, const void *B)
{
  const HueTuple *C = (const HueTuple *) A;
  const HueTuple *D = (const HueTuple *) B;

  if (C->h != D->h)
    return C->h - D->h;

  /* put brightest first, dimmest last */ 
  return D->hsv_val - C->hsv_val;
}


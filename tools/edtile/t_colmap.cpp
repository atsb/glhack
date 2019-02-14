//------------------------------------------------------------------------
//  COLORMAP for tiles
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
#include "z_hsv.h"

#include "t_colmap.h"
#include "t_tiles.h"


#define MAX_LINE  4100


namespace T_Colmap
{
  ColorMapping *cmap16 = 0;
  ColorMapping *cmap32 = 0;
}


ColorMapping::ColorMapping(bool open_) : 
    num_cols(0), trans_col(NO_PIX), 
    dark_col(NO_PIX), light_col(NO_PIX),
    open(open_)
{
  /* nothing else needed */
}

ColorMapping::~ColorMapping()
{
  /* nothing needed */
}

const class ColorMapping *ColorMapping::compare_cmap = 0;

void ColorMapping::AddRaw(const char *line)
{
  int is_hue = 0;
  int is_trans = 0;

  if (*line == 'H')
  {
    is_hue = 1; line++; 
  }
  else if (*line == 'T')
  {
    is_trans = 1; line++; 
  }
  else if (! isspace(*line))
  {
    fprintf(stderr, "Bad color type: `%s'\n", line);
    exit(63);
  }

  int r, g, b;
    
  if (3 != sscanf(line, " %i %i %i ", &r, &g, &b))
  {
    fprintf(stderr, "Color not understood: `%s'\n", line);
    exit(64);
  }

  if (num_cols >= max_cols)
  {
    fprintf(stderr, "Too many colors !\n");
    exit(62);
  }

  M_Color& mcol = m_arr[num_cols];

  mcol.c = PackCol(r, g, b);
  mcol.pair[0] = 0;
  mcol.flags = is_hue ? MCOL_F_HUE : 0;

  if (is_trans)
  {
    trans_col = num_cols;
  }

  num_cols++;
}

ColorMapping * ColorMapping::Load(const Filename& fname)
{
  FILE *fp = Z_File::Open(fname, "r");

  if (! fp)
  {
    fprintf(stderr, "Failed to open: %s\n", fname.str.c_str());
    exit(60);
  }

  fprintf(stderr, "Loading: %s\n", fname.str.c_str());

  char LineBuf[MAX_LINE];

  ColorMapping *cmap = new ColorMapping();

  for (;;)
  {
    if (! fgets(LineBuf, MAX_LINE-2, fp))
      break;

    cmap->AddRaw(LineBuf);
  }

  Z_File::Close(fp);

///!!!  compare_cmap = cmap;
///!!!
///!!!  qsort(cmap->hue_array, cmap->num_hues, sizeof(Hue_t), & HueComparer);
///!!!
///!!!  fprintf(stderr, "  NUM COLS=%d  NUM HUES=%d  TRANS=%u\n",
///!!!      cmap->num_cols, cmap->num_hues, cmap->trans_col);

  cmap->dark_col  = cmap->LookupClosest(0, 0, 0);
  cmap->light_col = cmap->LookupClosest(255, 255, 255);
    
  return cmap;
}

///--- bool ColorMapping::IsHue(Pix_t pix) const
///--- {
///---   // OUCH !  Slow...
///--- 
///---   if (pix == trans_col)
///---     return false;
///---   
///---   Hue_t h;
///--- 
///---   for (h=0; h < num_hues; h++)
///---     if (pix == hue_array[h])
///---       return true;
///--- 
///---   return false;
///--- }

///!!! Hue_t ColorMapping::LookupHue(Pix_t pix) const
///!!! {
///!!!   // OUCH !  Slow...
///!!! 
///!!!   if (pix == trans_col)
///!!!     return NO_HUE;
///!!!   
///!!!   HsvCol pix_hsv(c_array[pix]);
///!!!       
///!!!   Hue_t h;
///!!!   Hue_t best = NO_HUE;
///!!!   int best_dist = 1 << 30;
///!!! 
///!!!   for (h=0; h < num_hues; h++)
///!!!   {
///!!!     // exact match ?
///!!!     if (pix == hue_array[h])
///!!!       return h;
///!!! 
///!!!     HsvCol cur_hsv(c_array[hue_array[h]]);
///!!! 
///!!!     // handle black specially
///!!!     if (cur_hsv.v <= 1)
///!!!     {
///!!!       if (pix_hsv.v <= 1)
///!!!         return h;
///!!! 
///!!!       continue;
///!!!     }
///!!!     
///!!!     cur_hsv.v = pix_hsv.v;
///!!! 
///!!!     int dist = pix_hsv.Dist(cur_hsv);
///!!! 
///!!!     if (dist < best_dist)
///!!!     {
///!!!       best = h;
///!!!       best_dist = dist;
///!!!     }
///!!!   }
///!!!   
///!!!   return best;
///!!! }
///!!! 

///!!! int ColorMapping::HueComparer(const void *A, const void *B)
///!!! {
///!!!   Hue_t C = *((const Hue_t *) A);
///!!!   Hue_t D = *((const Hue_t *) B);
///!!! 
///!!!   HsvCol E(compare_cmap->c_array[C]);
///!!!   HsvCol F(compare_cmap->c_array[D]);
///!!! 
///!!!   E.h = (E.h + 30) % 360;
///!!!   F.h = (F.h + 30) % 360;
///!!!    
///!!!   int dh = E.h - F.h;
///!!!   int ds = E.s - F.s;
///!!!   int dv = E.v - F.v;
///!!! 
///!!!   dh = SGN(dh) * dh * dh;
///!!!   ds = SGN(ds) * ds * ds;
///!!!   dv = SGN(dv) * dv * dv;
///!!! 
///!!!   return dh + ds / 1000 + dv / 10000;
///!!! }

Pix_t ColorMapping::AddColor(const PackCol& col, const char *pair)
{
  ASSERT(pair && strlen(pair) <= 2);

  if (num_cols >= max_cols)
  {
    fprintf(stderr, "Too many colors !\n");
    exit(62);
  }

  M_Color& mcol = m_arr[num_cols];

  mcol.c = col;
  mcol.flags = 0;

  strcpy(mcol.pair, pair);
 
  // Update light and dark colors

  PackCol black(0, 0, 0);
  PackCol white(255, 255, 255);

  if (dark_col == NO_PIX ||
      (col.Dist(black) < m_arr[dark_col].c.Dist(black)))
  {
    dark_col = num_cols;
  } 
 
  if (light_col == NO_PIX ||
      (col.Dist(white) < m_arr[light_col].c.Dist(white)))
  {
    light_col = num_cols;
  } 
 
  num_cols++;

  return num_cols - 1;
}

Pix_t *ColorMapping::ComputeSameMap() const
{
  ASSERT(num_cols > 0);

  Pix_t *pmap = new Pix_t[num_cols];
  Pix_t i;

  // sort based on color

  for (i=0; i < num_cols; i++)
    pmap[i] = i;

  compare_cmap = this;

  qsort(pmap, num_cols, sizeof(Pix_t), & ColorComparer);

  // remove duplicates from mapping table

  int dup_num = 0;

  for (i=0; i < num_cols-1; i++)
  {
    Pix_t C = pmap[i];
    Pix_t D = pmap[i+1];

    // Note that this check handles the transparent color, since the
    // alpha component is checked too.
    
    if (m_arr[C].c != m_arr[D].c)
      continue;

    pmap[i+1] = C;

    dup_num++;
  }
//!!!!
fprintf(stderr, "ComputeSameMap: %d duplicates\n", dup_num);

  return pmap;
}

int ColorMapping::ColorComparer(const void *A, const void *B)
{
  Pix_t C = *((const Pix_t *) A);
  Pix_t D = *((const Pix_t *) B);

  const PackCol& E = compare_cmap->m_arr[C].c;
  const PackCol& F = compare_cmap->m_arr[D].c;

  if (E.r != F.r) return int(E.r) - int(F.r);
  if (E.g != F.g) return int(E.g) - int(F.g);
  if (E.b != F.b) return int(E.b) - int(F.b);

  // we ignore alpha here

  return int(C) - int(D);
}

Pix_t ColorMapping::LookupExact(const PackCol& col) const
{
  // OUCH !  Slow...

  Pix_t i;

  for (i=0; i < num_cols; i++)
  {
    const PackCol& curr = m_arr[i].c;

    if (curr == col)
      return i;
  }

  return NO_PIX;
}
      
Pix_t ColorMapping::LookupClosest(const PackCol& col) const
{
  // OUCH !  Slow...

  Pix_t i;
  Pix_t best = NO_PIX;
  int best_dist = 1 << 30;

  for (i=0; i < num_cols; i++)
  {
    if (i == trans_col)
      continue;

    // assumes alpha is 255 on everything
    int dist = col.Dist(m_arr[i].c);

    if (dist == 0)
      return i;

    if (dist < best_dist)
    {
      best = i;
      best_dist = dist;
    }
  }

  return best;
}
 
//------------------------------------------------------------------------


void ColorMapping::BeginMarking()
{
  for (Pix_t i = 0; i < num_cols; i++)
  {
    m_arr[i].flags = 0;
  }
}

Pix_t * ColorMapping::ConversionMap(const ColorMapping *target) const
{
  Pix_t *conv_map = new Pix_t[num_cols];

  for (Pix_t i = 0; i < num_cols; i++)
  {
    if (i == trans_col && target->trans_col != NO_PIX)
    {
      conv_map[i] = target->trans_col;
      continue;
    }

    conv_map[i] = target->LookupClosest(At(i));

    // check exactness of conversion
    
    const PackCol& local = At(i);
    const PackCol& other = target->At(conv_map[i]);

    if (local.Dist(other) != 0)
    {
      fprintf(stderr, "Warning: conversion-mapping (%d,%d,%d) --> "
          "(%d,%d,%d)\n",
          int(local.r), int(local.g), int(local.b),
          int(other.r), int(other.g), int(other.b));
    }
  }

  return conv_map;
}


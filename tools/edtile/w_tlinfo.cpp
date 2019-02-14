//------------------------------------------------------------------------
//  TILE Info for gui
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

#include "FL/fl_draw.H"

#include "w_tlinfo.h"
#include "w_color.h"


#define MAX_PIX_BUF  16384

void TileExtra::Draw(int xx, int yy, Tileset *set, Tile *tile, int zoom)
{
  DrawPart(xx, yy, set, tile, zoom, 
      0, 0, tile->width - 1, tile->height - 1);
}

void TileExtra::DrawPart(int xx, int yy, Tileset *set, Tile *tile, 
      int zoom, int x1, int y1, int x2, int y2)
{
  static byte pixel_buf[MAX_PIX_BUF];

  int ww = x2-x1+1;
  int hh = y2-y1+1;

  int chunk_size = ww * zoom * zoom * 3;
  ASSERT(chunk_size <= MAX_PIX_BUF);
  
  int chunk_y = MAX_PIX_BUF / chunk_size;

  int tw = tile->width;

  Pix_t black = set->cmap->LookupClosest(71, 108, 108);
  Pix_t white = set->cmap->LookupClosest(255, 255, 255);
 
  ASSERT(black != NO_PIX);
  ASSERT(white != NO_PIX);

  for (; hh > 0; hh -= chunk_y, y1 += chunk_y)
  {
    int dx, dy;

    for (dy=0; dy < MIN(hh, chunk_y); dy++)
    for (dx=0; dx < ww; dx++)
    {
      Pix_t pix = tile->pixels[(y1+dy) * tw + (x1+dx)];

      // handle transparency here

      if (pix == set->cmap->trans_col)
      {
        pix = black;

        if (set->tile_w == 48)
        {
          if ( (y1+dy == 63 && x1+dx >=  2 && x1+dx < 34) ||
               (y1+dy == 32 && x1+dx >= 16 && x1+dx < 50) ||
               (y1+dy > 32 && x1+dx == 32 - (y1+dy)/2)   ||
               (y1+dy > 32 && x1+dx == 64 - (y1+dy)/2) )
          {
            pix = white;
          }
        }
      }

      const PackCol& col = set->cmap->At(pix);
       
      byte r = col.r;
      byte g = col.g;
      byte b = col.b;

      int ex, ey;
      byte *dest = 0;

      for (ey=0; ey < zoom; ey++)
      {
        byte *dest = pixel_buf + ((dy * zoom + ey) * tw + dx) * zoom*3;

        for (ex=0; ex < zoom; ex++)
        {
          *dest++ = r;
          *dest++ = g;
          *dest++ = b;
        }
      }

      ASSERT(dest < (pixel_buf + MAX_PIX_BUF));
    }

    fl_draw_image(pixel_buf, xx + x1 * zoom, yy + y1 * zoom, 
        tw * zoom, MIN(hh, chunk_y) * zoom); 
  }
}


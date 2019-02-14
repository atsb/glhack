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

#ifndef __W_TLINFO_H__
#define __W_TLINFO_H__

#include "t_tiles.h"
#include "z_rgb.h"


struct TileExtra
{
  /* contains extra info (GUI-specific) for each tile in a tileset.
   * Value semantics.
   */
  
  bool tagged;
  bool hidden;

  TileExtra() : tagged(false), hidden(false) { }

  static void Draw(int xx, int yy, Tileset *set, Tile *tile, int zoom);

  static void DrawPart(int xx, int yy, Tileset *set, Tile *tile, 
      int zoom, int x1, int y1, int x2, int y2);
};


class TilesetExtra
{
  public:
    Tileset *base;

    TileExtra extras[TL_MAX_TILES];

    PackCol fill_col;

    TilesetExtra() : base(0), fill_col(128, 128, 128) { }
};


#endif  /* __W_TLINFO_H__ */

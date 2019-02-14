//----------------------------------------------------------------------------
//  IMPORT Program (3D tileset)
//----------------------------------------------------------------------------
//
//  (C) 2001-2002 Andrew Apted
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

#include "z_args.h"
#include "z_string.h"

#include "t_colmap.h"
#include "t_hue.h"
#include "t_tiles.h"


#define SKEW_MONSTER_MODE  1


static Tileset *mon32aja;
static Tileset *obj32aja;
static Tileset *oth32aja;

static int obj_start;
static int oth_start;

static Tileset *mon64aja;
static Tileset *obj64aja;
static Tileset *oth64aja;


static void Do3dImports(Tileset *src, const Tileset *dest)
{
  int index;

  for (index = 0; index < src->total; index++)
  {
    Tile *t = src->tiles + index;

    if (! t->name)
      continue;
    
    int orig_idx = dest->LookupName(t->name);

    if (orig_idx < 0)
    {
      fprintf(stderr, "Pruning (%s)\n", t->name);
      free((void *) t->name);
      t->name = 0;

      continue;
    }

    t->tile_id = dest->tiles[orig_idx].tile_id;
  }

  // check for missing stuff
 
  for (index = 0; index < dest->total; index++)
  {
    const Tile *t = dest->tiles + index;

    if (! t->name)
      continue;
    
    int new_idx = src->LookupName(t->name);

    if (new_idx < 0)
    {
      fprintf(stderr, "MISSING TILE (%s)\n", t->name);
      continue;
    }
  }
}


//------------------------------------------------------------------------

void DetemineBounds(const Tileset *set, const Tile *T, 
    int& x1, int& y1, int& x2, int& y2)
{
  Pix_t trans = set->cmap->trans_col;

  x1 = x2 = T->width  / 2;
  y1 = y2 = T->height / 2;

  int x, y;

  for (y = 0; y < T->height; y++)
  for (x = 0; x < T->width;  x++)
  {
    Pix_t p = T->pixels[y * T->width + x];

    if (p == NO_PIX || p == trans)
      continue;

    if (x < x1) x1 = x;
    if (y < y1) y1 = y;
    if (x > x2) x2 = x;
    if (y > y2) y2 = y;
  }
}
 
void CopyRawBox(const Tileset *src, const Tile *S, 
    int sx, int sy, int w, int h,
    Tile *D, int dx, int dy)
{
  Pix_t trans = src->cmap->trans_col;

  int x, y;
  
  for (y=0; y < h; y++)
  for (x=0; x < w; x++)
  {
    ASSERT(0 <= (sx+x) && (sx+x) < S->width);
    ASSERT(0 <= (sy+y) && (sy+y) < S->height);

    ASSERT(0 <= (dx+x) && (dx+x) < D->width);
    ASSERT(0 <= (dy+y) && (dy+y) < D->height);

    Pix_t p = S->pixels[(sy + y) * S->width + (sx + x)];

    if (p == NO_PIX || p == trans)
      continue;

    D->pixels[(dy + y) * D->width + (dx + x)] = p;
  }
}
    
void SkewTileset(Tileset *dest, const Tileset *src)
{
  for (int idx = 0; idx < src->total; idx++)
  {
    const Tile *S = src->tiles + idx;

    if (! S->name)
      continue;

    int d_idx = dest->LookupName(S->name);

    if (d_idx < 0)
    {
      fprintf(stderr, "Couldn't find (%s) to SKEW\n", S->name);
      continue;
    }

    Tile *D = dest->tiles + d_idx;

    D->Clear(dest->cmap->trans_col);

    int x1, y1;
    int x2, y2;
    
    DetemineBounds(src, S, x1, y1, x2, y2);

    int ww = x2 - x1 + 1;
    int hh = y2 - y1 + 1;

    int dest_x = 24 - ww / 2;
    int dest_y = 64 - 17 - hh;

    CopyRawBox(src, S, x1, y1, ww, hh, D, dest_x, dest_y);
  }
}


//------------------------------------------------------------------------

static void ShowInfo()
{
  fprintf(stdout,
      "\n**** Import3D 0.2 ****\n\n"
      "Copyright (C) 2001-2002 Andrew Apted.\n"
      "Under the GNU General Public License (GPL).\n\n"
      "USAGE: import ajadir slashdir\n\n"
  );

  fflush(stdout);
}

int main(int argc, const char **argv, const char **envp)
{
  if (argc > 1 &&
      (Z_String::CaseCmp(argv[1], "/?") == 0 ||
       Z_String::CaseCmp(argv[1], "-h") == 0 ||
       Z_String::CaseCmp(argv[1], "-help") == 0 ||
       Z_String::CaseCmp(argv[1], "--help") == 0))
  {
    ShowInfo();
    exit(0);
  }

  if (argc != 2)
  {
    fprintf(stderr, "USAGE: import3D ajadir\n");
    exit(2);
  }
 
  Z_Args::AddMany(argc - 1, argv + 1);
 
  char threedbuf[256];

  sprintf(threedbuf, "%s/THREE_D/", argv[1]);

  Filename ajdir(argv[1]);
  Filename threeddir(threedbuf);


  /* ---- colormaps ---- */

  T_Colmap::cmap16 = SharedColmap::Load(
      Filename("colors16.list").SetPath(ajdir));

  T_Colmap::cmap32 = SharedColmap::Load(
      Filename("colors32.list").SetPath(ajdir));


  /* ---- aja tiles ---- */
  
  mon32aja = Tileset::Load(Filename("mon32aja.txt").SetPath(ajdir),
      T_Colmap::cmap32);
  obj32aja = Tileset::Load(Filename("obj32aja.txt").SetPath(ajdir),
      T_Colmap::cmap32);
  oth32aja = Tileset::Load(Filename("oth32aja.txt").SetPath(ajdir),
      T_Colmap::cmap32);

  obj_start = mon32aja->total;
  oth_start = obj_start + obj32aja->total;

 
  /* ---- 3D tiles ---- */
  
#if (SKEW_MONSTER_MODE)
  mon64aja = Tileset::Load(Filename("mon64aja.txt").SetPath(ajdir),
      T_Colmap::cmap32);
  obj64aja = Tileset::Load(Filename("obj64aja.txt").SetPath(ajdir),
      T_Colmap::cmap32);
  oth64aja = Tileset::Load(Filename("oth64aja.txt").SetPath(ajdir),
      T_Colmap::cmap32);

  SkewTileset(mon64aja, mon32aja);
  SkewTileset(obj64aja, obj32aja);

#else

  mon64aja = Tileset::Load(Filename("mon3d.txt").SetPath(threeddir),
      T_Colmap::cmap32);
  obj64aja = Tileset::Load(Filename("obj3d.txt").SetPath(threeddir),
      T_Colmap::cmap32);
  oth64aja = Tileset::Load(Filename("oth3d.txt").SetPath(threeddir),
      T_Colmap::cmap32);
   
  mon64aja->SkipEmptyNames(1);
  obj64aja->SkipEmptyNames(1);
  oth64aja->SkipEmptyNames(1);

  mon64aja->ImportPairNames(mon32aja);
  obj64aja->ImportPairNames(obj32aja);
  oth64aja->ImportPairNames(oth32aja);

  Do3dImports(mon64aja, mon32aja);
  Do3dImports(obj64aja, obj32aja);
  Do3dImports(oth64aja, oth32aja);

#endif  /* SKEW_MONSTER_MODE */


  /* ---- save it all, baby ---- */

  fprintf(stderr, "\n");

  mon64aja->Save("mon64new.txt");
  obj64aja->Save("obj64new.txt");
  oth64aja->Save("oth64new.txt");
  
  return 0;
}

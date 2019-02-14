//----------------------------------------------------------------------------
//  Explosion modifier
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


#define SKEW_EXPL_MODE  1

#define WarpCol  WetWarpCol


PackCol DarkWarpCol(int r, int g, int b)
{
  int H = MAX(MAX(r, b), g);
  int L = MIN(MIN(r, b), g);

  int J = (H - L);

  return PackCol(
      /* RED */   MAX(0, (J * 4 - r)) / 7
  ,   /* GREEN */ MAX(0, (J * 4 - g)) / 7
  ,   /* BLUE */  MAX(0, (J * 4 + b)) / 7
  );
}

PackCol NoxiousWarpCol(int r, int g, int b)
{
  return PackCol(
      /* RED */   (g+b)/2
  ,   /* GREEN */ r
  ,   /* BLUE */  b/4
  );
}

PackCol MuddyWarpCol(int r, int g, int b)
{
  return PackCol(
      /* RED */   g*7/10
  ,   /* GREEN */ b*3/5
  ,   /* BLUE */  g*g/1600
  );
}

PackCol WetWarpCol(int r, int g, int b)
{
  int H = MAX(MAX(r, b), g);
  int L = MIN(MIN(r, b), g);

  int J = (H - L);
  int K = MAX(0, g - b);

  return PackCol(
      /* RED */   MAX(0, K*4/5 - b*b/1000)
  ,   /* GREEN */ MAX(0, J*4/5 - b*b/500)
  ,   /* BLUE */  r*4/5
  );
}

PackCol MagicWarpCol(int r, int g, int b)
{
  return PackCol(
      /* RED */   g
  ,   /* GREEN */ b
  ,   /* BLUE */  r
  );
}

PackCol FrostyWarpCol(int r, int g, int b)
{
  return PackCol(
      /* RED */   b
  ,   /* GREEN */ g
  ,   /* BLUE */  r
  );
}

void ModifyColors(Tileset *set, Tile *T)
{
  int x, y;

  for (y = 0; y < T->width;  y++)
  for (x = 0; x < T->height; x++)
  {
    Pix_t p = T->pixels[y * T->width + x];

    if (p == NO_PIX || p == set->cmap->trans_col)
      continue;

    const PackCol& orig = set->cmap->c_array[p];

    PackCol newcol = WarpCol(orig.r, orig.g, orig.b);

    T->pixels[y * T->width + x] = set->cmap->LookupClosest(newcol);
  }
}


//------------------------------------------------------------------------

const char *expl_names[] = 
{
  "dark", "noxious", "muddy", "wet", "magical",
  "fiery", "frosty" 
};

int corner_coords[18] =
{
  0, 0,   1, 0,   2, 0,
  0, 1,   1, 1,   2, 1,
  0, 2,   1, 2,   2, 2
};

void store_pixel(Tile **D, int abs_x, int abs_y, Pix_t p)
{
  int row, col;

  row = abs_y / 32;

  if (row == 0 && abs_x < 8)
  {
    if (abs_y < 16)
      return;

    row++;
  }

  if (row == 2 && abs_x >= 88)
  {
    if (abs_y >= 80)
      return;

    row--;
  }

  col = (abs_x + (row - 1) * 8) / 32;

  ASSERT(row >= 0 && row <= 2);
  ASSERT(col >= 0 && col <= 2);

  int base_x = 8 + col * 32 - row * 16;
  int base_y = row * 32 - 16;

  int x = abs_x - base_x;
  int y = abs_y - base_y;

#if 0  // debug
fprintf(stderr, "abs=(%d,%d) tile=(%d,%d) base=(%d,%d) pos=(%d,%d)\n",
abs_x, abs_y, col, row, base_x, base_y, x, y);
#endif

  assert(0 <= x && x < 48);
  assert(0 <= y && y < 64);
  
  int corner = row * 3 + col;
  
  D[corner]->pixels[y * 48 + x] = p;
}

void handle_one_explosion(Tileset *src, Tileset *dest, const char *name)
{
  Tile *S[9];  // source tiles making up the explosion
  Tile *D[9];  // destination tiles

  int S_num = 0;
  int D_num = 0;

  // find all the tiles in the explosion

  int corner;

  for (corner = 0; corner < 9; corner++)
  {
    char namebuf[256];

    sprintf(namebuf, "explosion %s %d", name, corner);

    int S_idx = src->LookupName(namebuf);

    if (S_idx < 0)
    {
      fprintf(stderr, "ERROR: source tile (%s) not found\n", namebuf);
      continue;
    }

    S[corner] = src->tiles + S_idx;
    S_num++;

    int D_idx = dest->LookupName(namebuf);

    if (D_idx < 0)
    {
      fprintf(stderr, "ERROR: dest tile (%s) not found\n", namebuf);
      continue;
    }

    D[corner] = dest->tiles + D_idx;
    D_num++;
  }

  if (S_num < 9 || D_num < 9)
    return;

  // clear destination tiles
   
  for (corner = 0; corner < 9; corner++)
    D[corner]->Clear(dest->cmap->trans_col);

  // transfer the pixels

  for (corner = 0; corner < 9; corner++)
  {
    int cx = corner_coords[corner * 2 + 0];
    int cy = corner_coords[corner * 2 + 1];

    int dest_x = cx * 32;
    int dest_y = cy * 32;

    int x, y;

    for (y=0; y < 32; y++)
    for (x=0; x < 32; x++)
    {
      Pix_t p = S[corner]->pixels[y * 32 + x];

      if (p == NO_PIX || p == src->cmap->trans_col)
        continue;

      store_pixel(D, dest_x + x, dest_y + y, p);
    }
  }
}
    
void skew_explosions(Filename& path)
{
  Tileset *src = Tileset::Load("expl32.txt", T_Colmap::cmap32);

  Tileset *dest = Tileset::Load("expl64.txt", T_Colmap::cmap32);

  for (int exp_idx = 0; exp_idx < 7; exp_idx++)
  {
    handle_one_explosion(src, dest, expl_names[exp_idx]);
  }

  fprintf(stderr, "\n");

  dest->Save("expl64_new.txt");
}


//------------------------------------------------------------------------

void ShowInfo()
{
  fprintf(stdout,
      "\n**** Explode 0.4 ****\n\n"
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

  Filename ajdir("../../win/share/");


  /* ---- colormaps ---- */

  T_Colmap::cmap16 = SharedColmap::Load(
      Filename("colors16.list").SetPath(ajdir));

  T_Colmap::cmap32 = SharedColmap::Load(
      Filename("colors32.list").SetPath(ajdir));


#if (SKEW_EXPL_MODE)
  skew_explosions(ajdir);
#else

  /* ---- aja tiles ---- */
  
  Tileset *expl = Tileset::Load("ex_fiery.txt", T_Colmap::cmap32);

  int idx;

  for (idx = 0; idx < expl->total; idx++)
  {
    ModifyColors(expl, expl->tiles + idx);
  }


  /* ---- save it all, baby ---- */

  fprintf(stderr, "\n");

  expl->Save("explo_new.txt");

#endif /* SKEW_EXPL_MODE */
  
  return 0;
}

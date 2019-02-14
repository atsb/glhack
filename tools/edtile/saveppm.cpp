//------------------------------------------------------------------------
//  SAVE PPM Program
//------------------------------------------------------------------------
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


#define BH_PADDING  0


static Tileset *set;


static int save_ppm_file(const char *filename, 
    const unsigned char *pixels, int image_w, int image_h)
{
  FILE *fp;
  int x, y;

  fp = fopen(filename, "wb");

  if (! fp)
    return 0;

  /* PPM header */
  fprintf(fp, "P6\n%d %d\n255\n", image_w, image_h);

  for (y = 0; y < image_h; y++)
  {
    const unsigned char *p = pixels + y * image_w * 3;

    for (x = 0; x < image_w; x++, p += 3)
    {
      fputc(p[0], fp);
      fputc(p[1], fp);
      fputc(p[2], fp);
    }
  }

  fclose(fp);
  return 1;
}

static void InstallTile(unsigned char *rgba, int width, int height,
    int bx, int by, Tileset *set, Tile *tile)
{
  int x, y;

  ASSERT(bx >= 0 && bx < width);
  ASSERT(by >= 0 && by < height);
 
  ASSERT(tile->pixels);

  for (y=0; y < tile->height; y++)
  for (x=0; x < tile->width; x++)
  {
    Pix_t pix = tile->pixels[y*tile->width + x];

#if (BH_PADDING > 0)
    if (pix == set->cmap->trans_col)
      pix = set->tiles[0].pixels[y*tile->width + x];
#endif
 
    const PackCol& col = set->cmap->At(pix);

    unsigned char *dst = rgba + (by + y) * width * 3 + (bx + x) * 3;

    dst[0] = col.r;
    dst[1] = col.g;
    dst[2] = col.b;
  }
}

static void InstallSet(unsigned char *rgba, int width, int height,
    int across, int down, Tileset *set, int& pos)
{
  int i;

  for (i=0; i < set->total; i++, pos++)
  {
    int bx = (pos % across) * set->tile_w;
    int by = (pos / across) * set->tile_h;

    InstallTile(rgba, width, height, bx, by, set, set->tiles + i);
  }
}

static void SaveAsPPM(const char *filename, int across, int down)
{
  int width  = across * set->tile_w;
  int height = down   * set->tile_h;

  unsigned char *rgba = new unsigned char [width * height * 3];

  memset(rgba, 0, width * height * 3);

  int pos = 0;

  InstallSet(rgba, width, height, across, down, set, pos);

  fprintf(stderr, "Saving %s...\n", filename);
  
  save_ppm_file(filename, rgba, width, height);
  fprintf(stderr, "\n");

  delete[] rgba;
}


//------------------------------------------------------------------------

static int integer_sqrt(int val)
{
  int i = 0;

  while (i * i < val)
    i++;

  return i;
}

static void ShowInfo()
{
  fprintf(stdout,
      "\n**** SavePPM 0.6 ****\n\n"
      "Copyright (C) 2001-2002 Andrew Apted.\n"
      "Under the GNU General Public License (GPL).\n\n"
      "USAGE: saveppm input.txt output.ppm\n\n"
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

  if (argc != 3)
  {
    fprintf(stderr, "USAGE: saveppm input.txt output.ppm\n");
    exit(2);
  }
 

  set = Tileset::Load(Filename(argv[1]));

#if (BH_PADDING > 0)
  set->SetTrans(TRANS_COL);

  int num_across = 16;
#else
  int num_across = integer_sqrt(set->total);
#endif
  int num_down = (set->total + num_across - 1) / num_across;

  SaveAsPPM(argv[2], num_across, num_down);
 
  return 0;
}

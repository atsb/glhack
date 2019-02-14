//------------------------------------------------------------------------
//  MAKESET Program
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


#define SLASHEM  0

#if (SLASHEM)
#define TOTAL_NUM   (1401 + 6)

#define MON16  "monsters.txt"
#define OBJ16  "objects.txt"
#define OTH16  "other.txt"

#define MON32  "mon32.txt"
#define OBJ32  "obj32.txt"
#define OTH32  "oth32.txt"

#define MON3D  "mon3d.txt"
#define OBJ3D  "obj3d.txt"
#define OTH3D  "oth3d.txt"

#else  /* NetHack 3.4.0 */

#define TOTAL_NUM   (1057 + 8)

#define MON16  "mon16aja.txt"
#define OBJ16  "obj16aja.txt"
#define OTH16  "oth16aja.txt"

#define MON32  "mon32aja.txt"
#define OBJ32  "obj32aja.txt"
#define OTH32  "oth32aja.txt"

#define MON64  "mon64aja.txt"
#define OBJ64  "obj64aja.txt"
#define OTH64  "oth64aja.txt"
#endif


#define NUM_ACROSS  32
#define NUM_DOWN    ((TOTAL_NUM + NUM_ACROSS - 1) / NUM_ACROSS)


static Tileset *mon16;
static Tileset *obj16;
static Tileset *oth16;

static Tileset *mon32;
static Tileset *obj32;
static Tileset *oth32;

static Tileset *mon3d;
static Tileset *obj3d;
static Tileset *oth3d;

static Tileset *font8;
static Tileset *font14;
static Tileset *font20;
static Tileset *font22;


static int save_ppm_file(const char *filename, 
    const unsigned char *pixels, int image_w, int image_h)
{
  FILE *fp;
  int x, y;

  fp = fopen(filename, "wb");

  if (! fp)
    return 0;

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

static void SaveAsPPM(const char *filename, int across, int down,
    Tileset *s1, Tileset *s2, Tileset *s3)
{
  int width  = across * s1->tile_w;
  int height = down   * s1->tile_h;

  unsigned char *rgba = new unsigned char [width * height * 3];

  memset(rgba, 0, width * height * 3);

  int pos = 0;

  InstallSet(rgba, width, height, across, down, s1, pos);

  if (s2)
    InstallSet(rgba, width, height, across, down, s2, pos);

  if (s3)
    InstallSet(rgba, width, height, across, down, s3, pos);

  fprintf(stderr, "Saving %s...\n", filename);
  
  save_ppm_file(filename, rgba, width, height);
  fprintf(stderr, "\n");

  delete[] rgba;
}


//------------------------------------------------------------------------

static void TilesToColumn(FILE *fp, Tileset *set, int pos)
{
  int i;

  for (i=0; i < set->total; i++, pos++)
  {
    int x, y;

    Tile *tile = set->tiles + i;

    ASSERT(tile->pixels);

    for (y=0; y < tile->height; y++)
    for (x=0; x < tile->width; x++)
    {
      Pix_t pix = tile->pixels[y*tile->width + x];

      fputc(pix, fp);
    }
  }
}

static void SaveAsRawColumn(const char *filename, int across, int down,
    Tileset *s1, Tileset *s2, Tileset *s3)
{
  int pos = 0;

  FILE *fp;

  fp = fopen(filename, "wb");

  if (! fp)
    exit(200);

  fprintf(stderr, "Saving %s...\n", filename);
  
  TilesToColumn(fp, s1, pos);

  if (s2)
    TilesToColumn(fp, s2, pos);

  if (s3)
    TilesToColumn(fp, s3, pos);

  fclose(fp);

  fprintf(stderr, "\n");
}


//------------------------------------------------------------------------

static void ShowInfo()
{
  fprintf(stdout,
      "\n**** MakeSet 0.4 ****\n\n"
      "Copyright (C) 2001-2002 Andrew Apted.\n"
      "Under the GNU General Public License (GPL).\n\n"
      "USAGE: makeset txtdir\n\n"
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
    fprintf(stderr, "USAGE: makeset txtdir\n");
    exit(2);
  }
 
  Z_Args::AddMany(argc - 1, argv + 1);
   
  const char *tdir = argv[1];


  /* ---- colormaps ---- */

  T_Colmap::cmap16 = ColorMapping::Load(
      Filename("colors16.list").SetPath(tdir));

  T_Colmap::cmap32 = ColorMapping::Load(
      Filename("colors32.list").SetPath(tdir));


  /* ---- small tiles ---- */

  mon16 = Tileset::Load(Filename(MON16).SetPath(tdir));
  obj16 = Tileset::Load(Filename(OBJ16).SetPath(tdir));
  oth16 = Tileset::Load(Filename(OTH16).SetPath(tdir));

  SaveAsPPM("gltile16.ppm", NUM_ACROSS, NUM_DOWN, mon16, obj16, oth16);
  SaveAsRawColumn("btile16.raw", NUM_ACROSS, NUM_DOWN, mon16, obj16, oth16);
   

  /* ---- big tiles ---- */
  
  mon32 = Tileset::Load(Filename(MON32).SetPath(tdir));
  obj32 = Tileset::Load(Filename(OBJ32).SetPath(tdir));
  oth32 = Tileset::Load(Filename(OTH32).SetPath(tdir));

  SaveAsPPM("gltile32.ppm", NUM_ACROSS, NUM_DOWN, mon32, obj32, oth32);
  SaveAsRawColumn("btile32.raw", NUM_ACROSS, NUM_DOWN, mon32, obj32, oth32);
 

  /* ---- pseudo 3D tiles ---- */
  
#if (SLASHEM)
  mon3d = Tileset::Load(Filename(MON3D).SetPath(tdir));
  obj3d = Tileset::Load(Filename(OBJ3D).SetPath(tdir));
  oth3d = Tileset::Load(Filename(OTH3D).SetPath(tdir));

  SaveAsPPM("gltile3d.ppm", NUM_ACROSS, NUM_DOWN, mon3d, obj3d, oth3d);
#else
  mon3d = Tileset::Load(Filename(MON64).SetPath(tdir));
  obj3d = Tileset::Load(Filename(OBJ64).SetPath(tdir));
  oth3d = Tileset::Load(Filename(OTH64).SetPath(tdir));

  SaveAsPPM("gltile64.ppm", NUM_ACROSS, NUM_DOWN, mon3d, obj3d, oth3d);
#endif
 

  /* ---- fonts ---- */
  
  ColorMapping *cmap_font = new ColorMapping(true);

  font8  = Tileset::Load(Filename("font8.txt").SetPath(tdir));
  font14 = Tileset::Load(Filename("font14.txt").SetPath(tdir));
  font20 = Tileset::Load(Filename("font20.txt").SetPath(tdir));
  font22 = Tileset::Load(Filename("font22.txt").SetPath(tdir));

  SaveAsPPM("glfont8.ppm",  16, 16, font8,  0, 0);
  SaveAsPPM("glfont14.ppm", 16, 16, font14, 0, 0);
  SaveAsPPM("glfont20.ppm", 16, 16, font20, 0, 0);
  SaveAsPPM("glfont22.ppm", 16, 16, font22, 0, 0);

  return 0;
}

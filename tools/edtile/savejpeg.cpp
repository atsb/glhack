//------------------------------------------------------------------------
//  SAVE JPEG Program
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


extern "C" 
{
#include "jpeglib.h"
}

#define QUALITY  90


static Tileset *set;


static int save_jpeg_file(const char *filename, 
    const unsigned char *pixels, int image_w, int image_h)
{
  FILE *fp;

  fp = fopen(filename, "wb");

  if (! fp)
    return 0;

  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;

  JSAMPROW row_pointer[1];

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);

  jpeg_stdio_dest(&cinfo, fp);

  cinfo.image_width  = image_w;
  cinfo.image_height = image_h;
  cinfo.input_components = 3;
  cinfo.in_color_space = JCS_RGB;

  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, QUALITY, TRUE);

  jpeg_start_compress(&cinfo, TRUE);

  while (cinfo.next_scanline < cinfo.image_height)
  {
    row_pointer[0] = (JSAMPROW) (pixels + 
        (cinfo.next_scanline * image_w * 3));
    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  jpeg_finish_compress(&cinfo);
 
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

static void SaveAsJPEG(const char *filename, int across, int down)
{
  int width  = across * set->tile_w;
  int height = down   * set->tile_h;

  unsigned char *rgba = new unsigned char [width * height * 3];

  memset(rgba, 0, width * height * 3);

  int pos = 0;

  InstallSet(rgba, width, height, across, down, set, pos);

  fprintf(stderr, "Saving %s...\n", filename);
  
  save_jpeg_file(filename, rgba, width, height);
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
      "\n**** SaveJPEG 0.3 ****\n\n"
      "Copyright (C) 2001-2002 Andrew Apted.\n"
      "Under the GNU General Public License (GPL).\n\n"
      "USAGE: savejpeg input.txt output.jpg\n\n"
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
    fprintf(stderr, "USAGE: savejpeg input.txt output.jpg\n");
    exit(2);
  }
 

  set = Tileset::Load(Filename(argv[1]));

  int num_across = integer_sqrt(set->total);
  int num_down = (set->total + num_across - 1) / num_across;

  SaveAsJPEG(argv[2], num_across, num_down);
 
  return 0;
}

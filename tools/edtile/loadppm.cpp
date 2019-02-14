//------------------------------------------------------------------------
//  LOAD PPM Program (convert to .TXT)
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


static Tileset *set;
static ColorMapping *cmap;


static int ppm_width, ppm_height;
static uint8_g *ppm_pixels;


static int skip_to_eol(FILE *fp)
{
  int c;

  while (((c = fgetc(fp)) != EOF) && (c != '\n')) 
  { /* nothing here */ }

  return c;
}

static void skip_comments(FILE *fp)
{
  int c;

  while (((c = fgetc(fp)) != EOF) && ((c == '#') || isspace(c)))
  {
    if (c == '#') 
    {
      if (skip_to_eol(fp) == EOF)
        return;
    }
  }

  ungetc(c, fp);
}

static int load_PPM(const char *filename)
{
  int x, y, maxval;

  FILE *fp;

  uint8_g *p;

  /* open input file */

  if ((fp = fopen(filename, "r")) < 0)
  {
    fprintf(stderr, "Unknown file '%s'.\n", filename);
    return -1;
  }

  /* read in header information */

  if ((fgetc(fp) != 'P') || (fgetc(fp) != '6'))
  {
    fprintf(stderr, "Not a PPM file '%s'.\n", filename);
    fclose(fp);
    return -1;
  }

  skip_comments(fp);

  if ((fscanf(fp, "%d", &ppm_width) != 1) || (ppm_width < 1))
  {
    fprintf(stderr, "Bad ppm file '%s'.\n", filename);
    fclose(fp);
    return -1;
  }

  skip_comments(fp);

  if ((fscanf(fp, "%d", &ppm_height) != 1) || (ppm_height < 1))
  {
    fprintf(stderr, "Bad ppm file '%s'.\n", filename);
    fclose(fp);
    return -1;
  }

  skip_comments(fp);

  if (fscanf(fp, "%d", &maxval) != 1)
  {
    fprintf(stderr, "Bad ppm file '%s'.\n", filename);
    fclose(fp);
    return -1;
  }

  skip_to_eol(fp);


  /* show information */

  fprintf(stderr, "PPM file '%s': %dx%d.\n", filename,
      ppm_width, ppm_height);


  /* read image pixels */

  ppm_pixels = p = (uint8_g *) malloc(ppm_width * ppm_height * 3);

  ASSERT(ppm_pixels);

  for (y=0; y < ppm_height; y++)
  {
    for (x=0; x < ppm_width;  x++) 
    {
      int r, g, b;

      r = fgetc(fp);
      g = fgetc(fp);
      b = fgetc(fp);

      if ((r == EOF) || (g == EOF) || (b == EOF)) 
      {
        fprintf(stderr, "Incomplete data in file"
            " '%s'.\n", filename);
        free(ppm_pixels);
        fclose(fp);
        return -1;
      }

      p[0] = r;
      p[1] = g;
      p[2] = b;

      p += 3;
    }

    fprintf(stderr, "LOADING: %03d%%\r", (y+1) * 100 / ppm_height);
  }

  fprintf(stderr, "DONE              \n");
  fclose(fp);

  return 0;
}

static void free_PPM(void)
{
  ASSERT(ppm_pixels);

  free(ppm_pixels);

  ppm_pixels = 0;
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
  fprintf(stderr,
      "\n**** LoadPPM 0.5.0 ****\n\n"
      "Copyright (C) 2001-2002 Andrew Apted.\n"
      "Under the GNU General Public License (GPL).\n\n"
      "USAGE: loadppm 32x32 ref.txt input.ppm > output.txt\n\n"
  );

  fflush(stderr);
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

  if (argc != 4)
  {
    fprintf(stderr, "USAGE: loadppm 32x32 ref.txt input.ppm > output.txt\n");
    exit(2);
  }

  int t_w, t_h;
  
  Filename ref_file(argv[2]);
  const char *input_file(argv[3]);


  if (sscanf(argv[1], " %dx%d ", &t_w, &t_h) != 2 ||
      t_w <= 0 || t_h <= 0)
  {
    fprintf(stderr, "Bad tile size: %s (should be WWxHH)\n", argv[1]);
    exit(4);
  }

  if (load_PPM(input_file) < 0)
  {
    fprintf(stderr, "Failed to load PPM file.\n");
    exit(3);
  }

  
  int across = (ppm_width  + t_w - 1) / t_w;
  int down   = (ppm_height + t_h - 1) / t_h;

  ASSERT(across > 0 && down > 0);

  fprintf(stderr, "Tile size: %dx%d   Tiles in image: %dx%d\n",
      t_w, t_h, across, down);
  
  if (ppm_width  != across * t_w || ppm_height != down * t_h)
  {
    fprintf(stderr, "** Warning: Image is not a multiple of tile size!\n");
  }


  if (ref_file.Ext() == Filename("lst") ||
      ref_file.Ext() == Filename("list"))
  {
    cmap = ColorMapping::Load(ref_file);
  }
  else
  {
    Tileset * ref_set = Tileset::Load(ref_file);

    cmap = ref_set->cmap;
  }

  set = Tileset::Create(t_w, t_h, cmap);


  int x, y;

  for (y=0; y < down;   y++)
  {
    for (x=0; x < across; x++)
    {
      int stride = ppm_width * 3;

      int cur_w = MIN(t_w, ppm_width  - x * t_w);
      int cur_h = MIN(t_h, ppm_height - y * t_h);

      ASSERT(cur_w > 0 && cur_h > 0);
      
      uint8_g *pix = ppm_pixels + (y * t_h * stride) + (x * t_w * 3);

      set->AddTile(pix, 3, stride, cur_w, cur_h);
    }

    fprintf(stderr, "CONVERTING: %03d%%\r", (y+1) * 100 / down);
  }

  fprintf(stderr, "DONE                      \n");
 
  free_PPM();

  set->Save(stdout);
  
  return 0;
}

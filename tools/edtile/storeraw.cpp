//------------------------------------------------------------------------
//  STORE RAW Program (for BrowserHack)
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


static void save_palette(FILE *fp)
{
  Pix_t pix;

  for (pix=0; pix < 256; pix++)
  {
    if (pix == set->cmap->trans_col || pix >= set->cmap->num_cols)
    {
      fputc(0, fp);
      fputc(0, fp);
      fputc(0, fp);
    }
    else
    {
      const PackCol& col = set->cmap->At(pix);

      fputc(col.r, fp);
      fputc(col.g, fp);
      fputc(col.b, fp);
    }
  }
}

static void save_tile(FILE *fp, Tileset *set, int idx)
{
  Tile *tile = set->tiles + idx;

  ASSERT(tile->pixels);

  /* transparency flag */

  fputc(tile->Contains(set->cmap->trans_col) ? 1 : 0, fp);
 
  int x, y;

  for (y=0; y < tile->height; y++)
  for (x=0; x < tile->width;  x++)
  {
    Pix_t pix = tile->pixels[y * tile->width + x];

    fputc((unsigned char) pix, fp);
  }
}

static void SaveAsRAW(const char *filename)
{
  fprintf(stderr, "Saving %s...\n", filename);

  FILE *fp = fopen(filename, "wb");

  if (! fp)
  {
    fprintf(stderr, "COULDN'T CREATE FILE: %s\n", filename);
    return;
  }

  save_palette(fp);

  for (int i=0; i < set->total; i++)
  {
    save_tile(fp, set, i);    
  }
 
  fclose(fp);

  fprintf(stderr, "\n");
}


//------------------------------------------------------------------------

/* returns -1 if name should be commented out */
static int ConvertName(const char *name, char *buf, int tile)
{
  char *result = buf;
  char last;
  int len;
    
  if (! name || ! name[0])
  {
    sprintf(buf, "UNKNOWN_%d", tile);
    return 0;
  }

  while (*name)
  {
    if (isspace(*name))
    {
      name++;
      
      while (isspace(*name))
        name++;
      
      *buf++ = '_';
      continue;
    }

    if (*name == '-')
    {
      *buf++ = '_';
      name++;
      continue;
    }
    
    if (! isalnum(*name))
    {
      fprintf(stderr, "\nBAD TILE NAME: `%s'\n", name);
      sprintf(buf, "BAD_NAME_%d", tile);
      return 0;
    }

    *buf++ = *name++;
  }

  *buf = 0;

  /* --- name checks --- */

  len = strlen(result);

  assert (len != 0);
 
  last = result[len-1];

  if ((strncmp(result, "wall_", 5) == 0) ||
      (strncmp(result, "cave_wall_", 10) == 0) ||
      (strncmp(result, "outer_", 6) == 0) ||
      (strncmp(result, "zap_", 4) == 0) ||
      (strncmp(result, "explosion_", 10) == 0) ||
      (strncmp(result, "swallow_", 8) == 0))
  {
    if (last >= '1')
      return -1;
  }

  return 0;
}
 
static void SaveJavaNames(const char *filename)
{
  int i;
  
  char name_buf[256];

  FILE *fp = fopen(filename, "wb");

  if (! fp)
  {
    fprintf(stderr, "ERROR CREATING: %s\n", filename);
    return;
  }

  fprintf(fp, "//\n");
  fprintf(fp, "// Tile Names\n");
  fprintf(fp, "//\n");
  fprintf(fp, "// (This file is automatically generated. Do not edit)\n");
  fprintf(fp, "//\n");

  fprintf(fp, "package gnu.browserhack.ui;\n");
  fprintf(fp, "\n");

  fprintf(fp, "public class TileName\n");
  fprintf(fp, "{\n");

  fprintf(fp, "   public static final int total = %d;\n", set->total);
  fprintf(fp, "\n");

  for (i=0; i < set->total; i++)
  {
    if (ConvertName(set->tiles[i].name, name_buf, i) < 0)
    {
      fprintf(fp, "// ");
    }
    else
    {
      fprintf(fp, "   ");
    }

    fprintf(fp, "public static final int %s = %d;\n", name_buf, i);
  }
   
  fprintf(fp, "}\n");

  fclose(fp);
}


//------------------------------------------------------------------------

static void ShowInfo()
{
  fprintf(stdout,
      "\n**** StoreRAW 0.8 ****\n\n"
      "Copyright (C) 2001-2002 Andrew Apted.\n"
      "Under the GNU General Public License (GPL).\n\n"
      "USAGE: storeraw input.txt output.raw out.java\n\n"
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

  if (argc != 4)
  {
    fprintf(stderr, "USAGE: storeraw input.txt output.raw out.java\n");
    exit(2);
  }
 

  set = Tileset::Load(Filename(argv[1]));

  set->SetTrans(TRANS_COL);

///  int num_across = 16;
///  int num_down = (set->total + num_across - 1) / num_across;

  SaveAsRAW(argv[2]);

  SaveJavaNames(argv[3]);
 
  return 0;
}

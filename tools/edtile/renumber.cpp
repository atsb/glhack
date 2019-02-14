//----------------------------------------------------------------------------
//  RENUMBER Program
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


//------------------------------------------------------------------------

static void ShowUsage(FILE *fp)
{
  fprintf(fp, "USAGE: renumber  startat  infile.txt  outfile.txt\n");
}

static void ShowInfo()
{
  fprintf(stdout,
      "\n**** Renumber 0.2 ****\n\n"
      "Copyright (C) 2001-2002 Andrew Apted.\n"
      "Under the GNU General Public License (GPL).\n\n");

  ShowUsage(stdout);
  
  fprintf(stdout, "\n");
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
    ShowUsage(stderr);
    exit(2);
  }

  Z_Args::AddMany(argc - 1, argv + 1);
   
  Filename aj_dir("/home/apps/glhack/win/share");

  Filename in_name(argv[2]);
  Filename out_name(argv[3]);

  int start_at = atoi(argv[1]);


  /* ---- colormaps ---- */

  Tileset *set = Tileset::Load(in_name);

  for (int i = 0; i < set->total; i++)
    set->tiles[i].tile_id = start_at + i;

  set->Save(out_name);

  return 0;
}

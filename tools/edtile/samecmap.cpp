//----------------------------------------------------------------------------
//  SAME CMAP utility
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


static Tileset *ref;
static Tileset *dest;


//------------------------------------------------------------------------

static void ShowInfo()
{
  fprintf(stdout,
      "\n**** SameCMap 1.1 ****\n\n"
      "Copyright (C) 2001-2002 Andrew Apted.\n"
      "Under the GNU General Public License (GPL).\n\n"
      "USAGE: samecmap ref.txt input.txt output.txt\n\n"
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
    fprintf(stderr, "USAGE: samecmap ref.txt input.txt output.txt\n");
    exit(2);
  }
 
  ref = Tileset::Load(Filename(argv[1]));

  dest = Tileset::Load(Filename(argv[2]));

  dest->ChangeColormap(ref->cmap);

  fprintf(stderr, "\n");

  dest->Save(argv[3]);
  
  return 0;
}

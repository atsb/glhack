//----------------------------------------------------------------------------
//  IMPORT Program
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


#if 0
static int flip_list[] =
{
  104, 105, 106, -1
};

static int alleg2_list[] =
{
  342, 343, -1
};

static int orig_mon_list[] =
{
  118, 277, 284, 285,   // 254,
  336, 339, 348, -1
};

static int orig_obj_list[] =
{
  749,
  786, 787, 788, 789,
  790, 791, 792, 793, 794, 795, 796, 797, 798, 799,
  800, 801, 802, 803, 804, 805, 806, 807, 808, 809,
  810, 811, 812, 813, 814, 815, 816,
  -1
};

static int orig_oth_list[] =
{
  890, 891, 892, 893, -1
};
#endif


static const char * nh34_oth_list[] =
{
  "explosion dark 0",
  "explosion dark 1",
  "explosion dark 2",
  "explosion dark 3",
  "explosion dark 4",
  "explosion dark 5",
  "explosion dark 6",
  "explosion dark 7",
  "explosion dark 8",
  "explosion noxious 0",
  "explosion noxious 1",
  "explosion noxious 2",
  "explosion noxious 3",
  "explosion noxious 4",
  "explosion noxious 5",
  "explosion noxious 6",
  "explosion noxious 7",
  "explosion noxious 8",
  "explosion muddy 0",
  "explosion muddy 1",
  "explosion muddy 2",
  "explosion muddy 3",
  "explosion muddy 4",
  "explosion muddy 5",
  "explosion muddy 6",
  "explosion muddy 7",
  "explosion muddy 8",
  "explosion wet 0",
  "explosion wet 1",
  "explosion wet 2",
  "explosion wet 3",
  "explosion wet 4",
  "explosion wet 5",
  "explosion wet 6",
  "explosion wet 7",
  "explosion wet 8",
  "explosion magical 0",
  "explosion magical 1",
  "explosion magical 2",
  "explosion magical 3",
  "explosion magical 4",
  "explosion magical 5",
  "explosion magical 6",
  "explosion magical 7",
  "explosion magical 8",
  "explosion frosty 0",
  "explosion frosty 1",
  "explosion frosty 2",
  "explosion frosty 3",
  "explosion frosty 4",
  "explosion frosty 5",
  "explosion frosty 6",
  "explosion frosty 7",
  "explosion frosty 8",
  NULL
};


static Tileset *mon32aja;
static Tileset *obj32aja;
static Tileset *oth32aja;

static int obj_start;
static int oth_start;

static Tileset *mon32ba;
static Tileset *obj32ba;
static Tileset *oth32ba;

static Tileset *mon32al2;

static Tileset *mon16nh34;
static Tileset *obj16nh34;
static Tileset *oth16nh34;


static Tileset * FindSet(int glob_idx, int *index)
{
  ASSERT(glob_idx >= 0);
  
  if (glob_idx >= oth_start)
  {
    (*index) = glob_idx - oth_start;
    ASSERT((*index) < oth32aja->total);

    return oth32aja;
  }

  if (glob_idx >= obj_start)
  {
    (*index) = glob_idx - obj_start;
    ASSERT((*index) < obj32aja->total);
    
    return obj32aja;
  }

  (*index) = glob_idx;
  ASSERT((*index) < mon32aja->total);

  return mon32aja;
}


static void DoImports(const int *list, const Tileset *src_set)
{
  for (; *list >= 0; list++)
  {
    int index;
    Tileset *set = FindSet(*list, &index);

    Tile *tile = set->tiles + index;

    int dest = src_set->LookupName(tile->name);

    if (dest < 0)
    {
      fprintf(stderr, "Warning: tile %d (%s) not found\n",
          *list, tile->name);
      continue;
    }
    
    set->ImportTile(index, src_set, dest);
  }
}


//------------------------------------------------------------------------

static Tileset * FindSetChar(const char *name, int *index)
{
  (*index) = mon32aja->LookupName(name);
  
  if (*index >= 0)
    return mon32aja;

  (*index) = obj32aja->LookupName(name);
  
  if (*index >= 0)
    return obj32aja;

  (*index) = oth32aja->LookupName(name);
  
  if (*index >= 0)
    return oth32aja;

  return NULL;
}

static void DoCharImports(const char **list, const Tileset *src_set)
{
  for (; *list; list++)
  {
    int index;
    Tileset *set = FindSetChar(*list, &index);

    if (! set)
    {
      fprintf(stderr, "Warning: tile (%s) not found in dest\n", *list);
      continue;
    }

    int dest = src_set->LookupName(*list);

    if (dest < 0)
    {
      fprintf(stderr, "Warning: tile (%s) not found in source\n", *list);
      continue;
    }
    
    set->ImportTile(index, src_set, dest);
  }
}


//------------------------------------------------------------------------

static void ShowInfo()
{
  fprintf(stdout,
      "\n**** Import 0.1 ****\n\n"
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

  if (argc != 3)
  {
    fprintf(stderr, "USAGE: import ajadir slashdir\n");
    exit(2);
  }
 
  Z_Args::AddMany(argc - 1, argv + 1);
   
  Filename ajdir(argv[1]);
  Filename sedir(argv[2]);


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

 
  /* ---- orig tiles ---- */
  
  mon16nh34 = Tileset::Load(Filename("monsters.txt").SetPath(sedir),
      T_Colmap::cmap16);
  obj16nh34 = Tileset::Load(Filename("objects.txt").SetPath(sedir),
      T_Colmap::cmap16);
  oth16nh34 = Tileset::Load(Filename("other.txt").SetPath(sedir),
      T_Colmap::cmap16);
   
  // make sure transparent color is converted properly
  // (bit of a hack).

  T_Colmap::cmap16->trans_col = T_Colmap::cmap16->LookupExact(
      PackCol(71, 108, 108));

#if 0
  mon32ba = Tileset::Load(Filename("mon32ba.txt").SetPath(ajdir),
      T_Colmap::cmap32);
  obj32ba = Tileset::Load(Filename("obj32ba.txt").SetPath(ajdir),
      T_Colmap::cmap32);
  oth32ba = Tileset::Load(Filename("oth32ba.txt").SetPath(ajdir),
      T_Colmap::cmap32);


  /* ---- slashem tiles ---- */
  
  mon32al2 = Tileset::Load(Filename("mon32al2.txt").SetPath(sedir),
      T_Colmap::cmap32);

  mon32al2->ConvertTrans(PackCol(255, 0, 255));


  /* ---- do flipping ---- */
  
  fprintf(stderr, "\nFLIPPING...\n");

  int i;
  
  for (i=0; flip_list[i] >= 0; i++)
  {
    int index;
    Tileset *set = FindSet(flip_list[i], &index);

    set->tiles[index].Flip();
  }
#endif

  
  /* ---- do imports ---- */
  
  DoCharImports(nh34_oth_list, oth16nh34);

#if 0
  fprintf(stderr, "ALLEG...\n");

  DoImports(alleg2_list, mon32al2);


  fprintf(stderr, "MONSTERS...\n");

  DoImports(orig_mon_list, mon32ba);


  fprintf(stderr, "OBJECTS...\n");

  DoImports(orig_obj_list, obj32ba);


  fprintf(stderr, "OTHER...\n");

  DoImports(orig_oth_list, oth32ba);
#endif


  /* ---- save it all, baby ---- */

  fprintf(stderr, "\n");

  mon32aja->Save("mon32new.txt");
  obj32aja->Save("obj32new.txt");
  oth32aja->Save("oth32new.txt");
  
  return 0;
}

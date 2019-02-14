//----------------------------------------------------------------------------
//  PAIR handling
//----------------------------------------------------------------------------
//
//  EdTile (C) 2001-2002 Andrew Apted
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

#include "t_pair.h"
#include "t_alias.h"


#define MAX_LINE  800


PairSet::PairSet() : num_pairs(0)
{
  /* nothing else needed */
}

PairSet::~PairSet()
{
  int i;

  for (i=0; i < max_pairs; i++)
  {
    p_array[i].name = "";
  }
}

void PairSet::AddRaw(const char *line)
{
  if (num_pairs >= max_pairs)
  {
    fprintf(stderr, "Too many aliases !\n");
    exit(72);
  }

  Pair_t *p = p_array + num_pairs;
  num_pairs++;

  int i;

  for (i=1; line[i] && line[i] != ' '; i++)
  { /* empty block */ }
  
  ASSERT(i <= 4);
      
  strncpy(p->pair, line, i);
  p->pair[i] = 0;

  line = line + i + 1;

  p->name = string(line);

#if 1  /* DEBUGGING */
  fprintf(stderr, "PAIR `%s' is (%s)\n", p->pair, p->name.c_str());
#endif
}
    
PairSet * PairSet::Load(const Filename& fname)
{
  FILE *fp = Z_File::Open(fname, "r");

  if (! fp)
  {
    fprintf(stderr, "Failed to open: %s\n", fname.str.c_str());
    exit(70);
  }

  fprintf(stderr, "Loading: %s\n", fname.str.c_str());

  char LineBuf[MAX_LINE];

  PairSet *set = new PairSet();

  for (;;)
  {
    if (! fgets(LineBuf, MAX_LINE-2, fp))
      break;

    if (strlen(LineBuf) == 0)
      continue;

    set->AddRaw(LineBuf);
  }

  Z_File::Close(fp);

  return set;
}


/* ---------------------------------------------------------------- */

/* 
 * returns either the given string or alias-stored string.
 */
string PairSet::Lookup(const string& tile_name)
{
  int i;

  for (i=0; i < num_pairs; i++)
  {
    if (T_Alias::FuzzyCmp(tile_name, p_array[i].name) == 0)
    {
      return string(p_array[i].pair);
    }
  }

  return "";
}


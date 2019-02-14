//----------------------------------------------------------------------------
//  MODEL Specs
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

#ifndef __T_SPECS_H__
#define __T_SPECS_H__

#include "z_file.h"

#include "t_alias.h"


#define TL_MAX_SPECS  4096


struct ModelSpec
{
  int spec_id;
  const char *name;

  /* link in hash chain, -1 if none */
  int hash_next;

  char model[64];
  char skin[64];
  char material[64];
  char light[64];

  float scale;
  float rotate;

  int flags;  
};


class SpecGroup
{
  public:
    SpecGroup();
    ~SpecGroup();

    ModelSpec specs[TL_MAX_SPECS];
    int total;

    static SpecGroup * Load(const Filename& fname);
    // load the tiles from a standard TSP file.

    int LookupName(const char *name) const;
    // find the given tilename.  Returns -1 if not found.

    void Save(const Filename& fname);
    // save the tileset into a standard TSP file.
     
    void SkipEmptyNames(int value) { skip_empties = value; }
    // when value is 1, causes empty names to be skipped when saving
    // the tileset.  Default is zero.

  private:
    int hash_tab[T_Alias::hash_size];
    // hash table, using tile indexes.

    int skip_empties; 

    /* data needed only while loading/saving.
     */
    int got_spec, got_id;
    const char *got_name;

    ModelSpec tmp_spec;
      
    /* low-level loading routines.
     */
    void DoTag(char *line);
    void AddToHashTable(int index);
    void StartTile();
    void EndTile();
    void TileDatum(char *line);
    void HandleLine(char *line);

    /* low-level saving routines
     */
    void WriteSpec(FILE *fp, ModelSpec *spec, int id);
};


/* --------- IMPLEMENTATION --------- */


#endif  /* __T_SPECS_H__ */

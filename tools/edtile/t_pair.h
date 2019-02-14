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

#ifndef __T_PAIRS_H__
#define __T_PAIRS_H__

#include "z_file.h"

#include <string>


class PairSet
{
  public:
    PairSet();
    ~PairSet();

    static PairSet *Load(const Filename& fname);

    string Lookup(const string& tile_name);

  private:
    static const int max_pairs = 2048;

    int num_pairs;

    typedef struct
    {
      char pair[6];
      string name;
    }
    Pair_t;

    Pair_t p_array[max_pairs];

    void AddRaw(const char *line);
};


#endif  /* __T_PAIRS_H__ */

//----------------------------------------------------------------------------
//  ALIAS handling
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

#ifndef __T_ALIAS_H__
#define __T_ALIAS_H__

#include "z_file.h"

#include <string>


namespace T_Alias
{
  const int hash_size = 229;
  const int hash_mul  = 37;

  void Load(const Filename& fname);
  void Free(void);

  string Lookup(const string& name);
  int FuzzyCmp(const string& A, const string& B);
  int Hash(const string& name);
}


#endif  /* __T_ALIAS_H__ */

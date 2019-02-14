//------------------------------------------------------------------------
//  FILE utilities
//------------------------------------------------------------------------
//
//  Z C++ Lib (C) 2001-2002 Andrew Apted
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

#ifndef __Z_FILE_H__
#define __Z_FILE_H__

#include <string>

#include "z_string.h"


struct Filename
{
  /* This is a simple wrapper class for handling filenames */

  string str;
   
  // platform specifics:
  static const char DirSep;
  static const bool AllowDrives;
  static const bool CaseSensitive;
  
  Filename(const string& fname);
  Filename(const char *fname);

  Filename(const Filename& fname);
  Filename& operator= (const Filename& fname);

  bool operator< (const Filename& src) const;
  bool operator== (const Filename& src) const;
 
  Filename Drive() const;
  Filename Path() const;
  Filename File() const;
  Filename Ext() const;
  // these routines return the drive/path/file/extension of the
  // filename, possibly the empty string if none.  Note that Path()
  // will include any drive, and File() will include any extension.

  Filename& RemovePath();
  Filename& RemoveExt();
  // these remove any path/extension from the filename.

  Filename& SetPath(const Filename& path);
  Filename& SetExt(const Filename& ext);
  // these change the path/extension to the one given.  The empty
  // string is allowed.
 
  Filename& MakeCanonical();
  // checks whether the filename follows the platform-specific
  // conventions.  If yes, returns true, otherwise the filename is
  // converted and false is returned.
};

inline Filename::Filename(const string& fname) : str(fname) 
{ }

inline Filename::Filename(const char *fname) : str(fname)
{ }

inline Filename::Filename(const Filename& src) : str(src.str)
{ }

inline Filename& Filename::operator= (const Filename &src)
{
  CHECK_SELF_ASSIGN(src);
  str = src.str;
  return *this;
}


//------------------------------------------------------------------------
//
//  UTILITIES
// 

namespace Z_File
{
  bool Exists(const Filename& fname);
  // check if the given file exists.

  FILE *Open(const Filename& fname, const char *mode);
  // opens the given file.  Returns 0 if unsuccessful, otherwise a
  // normal stdio file handle -- use FileClose() when done.

  void Close(FILE *fp);
  // mainly here for completeness.
  
  bool Delete(const Filename& fname);
  // deletes the given file.  Returns true if successful.
  
  bool Rename(const Filename& src, const Filename& dest);
  // renames the given file.  Returns true if successful.  Mainly here
  // for completeness. 
 
  bool Copy(const Filename& src, const Filename& dest);
  // make a copy of the source file into the destination file.  The
  // destination will be overwritten if it already exists.  Returns
  // true if successful.
  
  /* ---- test/debug routines ---- */
   
  void TestPlatform();
  // print the platform specifics to stdout.

  void TestNames();
  // test the filename checking & modifying routines.
}


#endif /* __Z_FILE_H__ */

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

using namespace std;

#include "z_config.h"
#include "z_file.h"

#include <iostream>


#if defined(WIN32)

const char Filename::DirSep = '\\';
const bool Filename::AllowDrives = true;
const bool Filename::CaseSensitive = false;

#else

const char Filename::DirSep = '/';
const bool Filename::AllowDrives = false;
const bool Filename::CaseSensitive = true;

#endif


bool Filename::operator< (const Filename& src) const
{
#if defined(WIN32)
  return Z_String::CaseCmp(str, src.str) < 0;
#else
  return str < src.str;
#endif
}

bool Filename::operator== (const Filename& src) const
{
#if defined(WIN32)
  return Z_String::CaseCmp(str, src.str) == 0;
#else
  return str == src.str;
#endif
}


Filename Filename::Drive() const
{
  if (AllowDrives && str.size() >= 2 && str[1] == ':')
  {
    return string(str, 0, 2);
  }

  return string();
}

Filename Filename::Path() const
{
  string::const_reverse_iterator p = str.rbegin();

  for (; p != str.rend(); p++)
  {
    if (*p == DirSep || (AllowDrives && *p == ':'))
      break;
  }

  if (p == str.rend())
    return string();

  // don't leave a trailing directory char
  if (*p == DirSep)
    p++;

  return string(str.begin(), p.base());
}

Filename Filename::File() const
{
  string::const_reverse_iterator p = str.rbegin();

  for (; p != str.rend(); p++)
  {
    if (*p == DirSep || (AllowDrives && *p == ':'))
      break;
  }

  if (p == str.rend())
    return str;

  return string(p.base(), str.end());
}

Filename Filename::Ext() const
{
  string::const_reverse_iterator p = str.rbegin();

  for (; p != str.rend(); p++)
  {
    if (*p == DirSep || (AllowDrives && *p == ':'))
      break;

    if (*p == '.')
      return string(p.base(), str.end());
  }

  return string();
}


Filename& Filename::RemovePath()
{
  string::reverse_iterator p = str.rbegin();

  for (; p != str.rend(); p++)
  {
    if (*p == DirSep || (AllowDrives && *p == ':'))
    {
      str.erase(str.begin(), p.base());
      break;
    }
  }

  return *this;
}

Filename& Filename::RemoveExt()
{
  string::reverse_iterator p = str.rbegin();

  for (; p != str.rend(); p++)
  {
    if (*p == DirSep || (AllowDrives && *p == ':'))
      break;

    if (*p == '.')
    {
      str.erase(p.base() - 1, str.end());
      break;
    }
  }

  return *this;
}

Filename& Filename::SetPath(const Filename& path)
{
  RemovePath();

  if (path.str.size() > 0)
  {
    str = (path.str + string(1, DirSep) + str);
  }

  return *this;
}

Filename& Filename::SetExt(const Filename& ext)
{
  RemoveExt();

  if (ext.str.size() > 0)
  {
    str += '.';
    str += ext.str;
  }

  return *this;
}


Filename& Filename::MakeCanonical()
{
  int drives = 0;
  int total_sep = 0;
  int good_sep = 0;
  
  if (str.size() >= 2 && isalpha(str[0]) && str[1] == ':')
  {
    drives++;
  }

  string::const_iterator p;

  for (p=str.begin(); p != str.end(); p++)
  {
    if (*p == '/' || *p == '\\')
      total_sep++;

    if (*p == DirSep)
      good_sep++;
  }

  if (! AllowDrives && drives > 0 && good_sep == 0)
  {
    /* not canonical */
  }
  else if (total_sep == 0)
  {
    return *this;
  }
  else if (good_sep * 2 >= total_sep)
  {
    return *this;
  }

  // --- must be opposite convention ---

  string::iterator q;

  for (q=str.begin(); q != str.end(); q++)
  {
    if (*q == '/' || *q == '\\')
      *q = DirSep;
  }

  if (! AllowDrives && drives > 0)
  {
    // convert "X:"        -> "/x"
    // convert "X:abc..."  -> "/x/abc..."
    // convert "X:\abc..." -> "/x/abc..."
    
    str[0] = tolower(str[0]);
    str.erase(1, 1);
    str.insert(0, string(1, DirSep));

    if (str.size() >= 3 && str[2] != DirSep)
    {
      str.insert(2, string(1, DirSep));
    }
  }

  return *this;
}


//------------------------------------------------------------------------


bool Z_File::Exists(const Filename& fname)
{
  FILE *fp = Z_File::Open(fname, "rb");

  if (fp)
  {
    fclose(fp);
    return true;
  }

  return false;
}

FILE * Z_File::Open(const Filename& fname, const char *mode)
{
  ARG_CHECK("mode", mode);
  return fopen(fname.str.c_str(), mode);
}

void Z_File::Close(FILE *fp)
{
  ARG_CHECK("fp", fp);
  fclose(fp);
}

bool Z_File::Delete(const Filename& fname)
{
  return (remove(fname.str.c_str()) == 0);
}
 
bool Z_File::Rename(const Filename& src, const Filename& dest)
{
  return (rename(src.str.c_str(), dest.str.c_str()) == 0);
}

bool Z_File::Copy(const Filename& src, const Filename& dest)
{
  FILE *s_fp = Z_File::Open(src, "rb");

  if (! s_fp)
    return false;

  FILE *d_fp = Z_File::Open(dest, "wb");

  if (! d_fp)
  {
    Z_File::Close(s_fp);
    return false;
  }

  char buffer[256];
  int len = 0;

  for (;;)
  {
    len = fread(buffer, 1, 256, s_fp);

    if (len <= 0)
      break;

    ASSERT(len <= 256);

    fwrite(buffer, 1, len, d_fp);

    if (len < 256)
      break;
  }

  fflush(d_fp);

  Z_File::Close(s_fp);
  Z_File::Close(d_fp);

  return (len >= 0);
}


//------------------------------------------------------------------------
//
//  TEST ROUTINES
//

#ifndef NDEBUG

namespace Z_File
{
  const char *test_file_names[] =
  {
    "foo", "bar.wad",
    "simple/test.dud",
    "/home/fred/super.fry",
    "../qdoom/qdoom.wad",
    "jimbo.", "/bubbly/",
    "/", "..", "../.",
    "\\jimmy\\blar\\whiterock.tex",
    "C:\\JIMMY\\BLAR\\WHITESTONE",
    "D:", "a:\\", "b:..",
    "D:Krackle.nokob",
    "Z:/Pommle/Krackle.nokob",
  };
}

void Z_File::TestPlatform()
{
  cout << "\n---FILE-TEST-PLATFORM----------\n";

  cout << "DirSep = `"       << Filename::DirSep << "'\n";
  cout << "AllowDrives = "   << Filename::AllowDrives << "\n";
  cout << "CaseSensitive = " << Filename::CaseSensitive << "\n";

  cout << "-------------------------------\n\n";
}

void Z_File::TestNames()
{
  cout << "\n---FILE-TEST-NAMES-------------\n";

  int num_names = int(sizeof(test_file_names) / sizeof(char *));

  for (int i=0; i < num_names; i++)
  {
    Filename curname(test_file_names[i]);
    
    cout << "Filename: " << curname.str << "\n\n";

    cout << "  Drive = `" << curname.Drive().str << "'\n";
    cout << "  Path = `"  << curname.Path().str  << "'\n";
    cout << "  File = `"  << curname.File().str  << "'\n";
    cout << "  Ext = `"   << curname.Ext().str   << "'\n";
     
    cout << "  Extension:  ";
    cout << "wad = " << (curname.Ext() == "wad") << "  ";
    cout << "WAD = " << (curname.Ext() == "WAD") << "  ";
    cout << "nokob = " << (curname.Ext() == "nokob") << "  ";
    cout << "`' = " << (curname.Ext() == "");
    cout << "\n";

    Filename noext = curname;
    noext.RemoveExt();
    cout << "  Without extension = `" << noext.str << "'\n";

    Filename nopath = curname;
    nopath.RemovePath();
    cout << "  Without path = `" << nopath.str << "'\n";

    Filename canon = curname;
    canon.MakeCanonical();
    cout << "  Canonical = `" << canon.str << "'\n";

    cout << "\n";
  }

  cout << "-------------------------------\n\n";
}

#endif  // NDEBUG


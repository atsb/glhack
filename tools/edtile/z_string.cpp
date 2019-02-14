//------------------------------------------------------------------------
//  STRING utilities
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
#include "z_string.h"

#include <iostream>


void Z_String::MakeLower(char *str)
{
  for (; *str; str++)
    *str = tolower(*str);
}

void Z_String::MakeUpper(char *str)
{
  for (; *str; str++)
    *str = toupper(*str);
}

void Z_String::MakeLower(string& str)
{
  string::iterator p;

  for (p=str.begin(); p != str.end(); p++)
    *p = tolower(*p);
}

void Z_String::MakeUpper(string& str)
{
  string::iterator p;

  for (p=str.begin(); p != str.end(); p++)
    *p = toupper(*p);
}


int Z_String::CaseCmp(const char *A, const char *B)
{
  for (; *A && *B; A++, B++)
  {
    if (toupper(*A) != toupper(*B))
      return (toupper(*A) - toupper(*B));
  }

  return (*A) ? 1 : (*B) ? -1 : 0;
}

// CaseCmpLen is like the above routine, but compares no more than
// `len' characters at the start of each string.
//
int Z_String::CaseCmpLen(const char *A, const char *B, int len)
{
  for (; (*A && *B) && (len > 0); A++, B++, len--)
  {
    if (toupper(*A) != toupper(*B))
      return (toupper(*A) - toupper(*B));
  }

  if (len == 0)
    return 0;

  return (*A) ? 1 : (*B) ? -1 : 0;
}


int Z_String::CaseCmp(const string& A, const string& B)
{
  string::const_iterator C, D;
  
  for (C = A.begin(), D = B.begin();
       C != A.end() && D != B.end();
       C++, D++)
  {
    if (toupper(*C) != toupper(*D))
      return (toupper(*C) - toupper(*D));
  }

  return (C != A.end()) ? 1 : (D != B.end()) ? -1 : 0;
}

int Z_String::CaseCmpLen(const string& A, const string& B, int len)
{
  string::const_iterator C, D;
  
  for (C = A.begin(), D = B.begin();
       C != A.end() && D != B.end() && (len > 0);
       C++, D++, len--)
  {
    if (toupper(*C) != toupper(*D))
      return (toupper(*C) - toupper(*D));
  }

  if (len == 0)
    return 0;

  return (C != A.end()) ? 1 : (D != B.end()) ? -1 : 0;
}


void Z_String::WriteSafeStr(const string& str)
{
  string::const_iterator p;

  for (p = str.begin(); p != str.end(); p++)
  {
    char ch = *p;

    if (isprint(ch))
    {
      cout << ch;
      continue;
    }

    cout << "<Ch=" << int(ch) << ">";
  }
}

void Z_String::WriteSafeStr(const char *start, const char *end)
{
  for (; start != end; start++)
  {
    char ch = *start;

    if (isprint(ch))
    {
      cout << ch;
      continue;
    }

    cout << "<Ch=" << int(ch) << ">";
  }
}


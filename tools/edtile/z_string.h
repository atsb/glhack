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

#ifndef __Z_STRING_H__
#define __Z_STRING_H__

#include <string>


namespace Z_String
{
  void MakeLower(char *str);
  void MakeUpper(char *str);
  
  void MakeLower(string& str);
  void MakeUpper(string& str);

  void WriteSafeStr(const string& str);
  void WriteSafeStr(const char *start, const char *end);
  // writes the string "safely", outputting control characters using
  // the format <Ch=##>.
 
  // --- case insensitive comparisons ---
 
  int CaseCmp(const char *A, const char *B);
  int CaseCmpLen(const char *A, const char *B, int len);

  int CaseCmp(const string& A, const string& B);
  int CaseCmpLen(const string& A, const string& B, int len);
}


#endif /* __Z_STRING_H__ */

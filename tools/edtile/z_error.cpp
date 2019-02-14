//----------------------------------------------------------------------------
//  ERROR and debugging
//----------------------------------------------------------------------------
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
#include "z_error.h"

//
// Fatal
//
// Terminates the program, reporting the error.
//
void Z_Error::Fatal(const char *str, ...)
{
  char buffer[2048];

  // create message
  va_list args;

  va_start(args, str);
  vsprintf(buffer, str, args);
  va_end(args);

  if (buffer[0] == 0 || buffer[strlen(buffer)-1] != '\n')
    strcat(buffer, "\n");

  fprintf(stderr, "\n\n*** FATAL ERROR ***\n\n%s\n", buffer);
      
  exit(10);
}

//
// Debug
//
// FIXME: this code is platform specific (and hack-ish).
//
void Z_Error::Debug(const char *str, ...)
{
#ifndef NDEBUG
  static FILE *debug_fp = 0;

  if (! debug_fp)
  {
    debug_fp = fopen("/tmp/debug.txt", "w");

    if (! debug_fp)
    {
      Z_Error::Fatal("Unable to open: /tmp/debug.txt\n");
    }
  }

  char buffer[2048];

  // create message
  va_list args;

  va_start(args, str);
  vsprintf(buffer, str, args);
  va_end(args);

  fprintf(debug_fp, "%s", buffer);
#else
  (void) str;
#endif
}

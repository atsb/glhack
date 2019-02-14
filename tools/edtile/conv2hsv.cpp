//----------------------------------------------------------------------------
//  RGB --> HSV Converter
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

#include "z_string.h"
#include "z_rgb.h"
#include "z_hsv.h"


static void ShowInfo()
{
  fprintf(stdout,
      "\n**** Conv2HSV 1.0 ****\n\n"
      "Copyright (C) 2001-2002 Andrew Apted.\n"
      "Under the GNU General Public License (GPL).\n\n"
      "USAGE: conv2hsv < input.rgb > output.hsv\n\n"
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

  for (;;)
  {
    int r, g, b;

    if (scanf(" %d %d %d ", &r, &g, &b) != 3)
      break;

    PackCol rgbcol(r, g, b);
    
    HsvCol hsvcol(rgbcol);

    printf("%3d %3d %3d\n", hsvcol.h, hsvcol.s, hsvcol.v);
  }
 
  return 0;
}

//------------------------------------------------------------------------
//  Web (Highlight) button
//------------------------------------------------------------------------
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

#ifndef __W_WEBBUT_H__
#define __W_WEBBUT_H__

#include "FL/Fl_Widget.H"


class WebButton : public Fl_Widget
{
  public:
    WebButton(int X, int Y, int W, int H, const char *label = 0);
    ~WebButton();

    void value(int new_v);
    int value() const { return value_; }

  public:
    int handle(int event);
    // FLTK virtual method for handling input events.

  private:
    void draw();
    // FLTK virtual method for drawing.
   
  private:
    int value_;
    int high_;
};


#endif  /* __W_WEBBUT_H__ */

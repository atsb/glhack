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

using namespace std;

#include "z_config.h"

#include "FL/Fl_Window.H"
#include "FL/fl_draw.H"

#include "w_webbut.h"
#include "w_color.h"
#include "w_main.h"


WebButton::WebButton(int X, int Y, int W, int H, const char *label) : 
    Fl_Widget(X, Y, W, H, label),
    value_(0), high_(0)
{
  box(FL_FLAT_BOX);
  color(FL_WHITE);
  labelcolor(FL_BLACK);
}

WebButton::~WebButton()
{
  // nothing needed
}

void WebButton::value(int new_v)
{
  if (value_ != new_v)
  {
    value_ = new_v;
    redraw();
  }
}

void WebButton::draw()
{
  labelcolor(value_ ? FL_RED : FL_BLACK);
   
  draw_box(box(), color());
  draw_label();

  if (Fl::focus() == this) 
    draw_focus();
}

int WebButton::handle(int event)
{
  /* put here to work, but very hackish */
  if (high_ && !value_)
    MainWin::appwin->ChangeCursor(FL_CURSOR_HAND);

  switch (event)
  {
    case FL_ENTER:
      high_ = 1;
      redraw();
      return 1;

    case FL_LEAVE:
      MainWin::appwin->ChangeCursor(FL_CURSOR_DEFAULT);
      high_ = 0;
      redraw();
      return 1;

    case FL_FOCUS:
    case FL_UNFOCUS:
      if (Fl::visible_focus())
      {
        redraw();
        return 1;
      }
      return 0;
     
    case FL_PUSH:
      if (! value_)
      {
//        value_ = 1;
//        redraw();
        do_callback();
      }
      return 1;
     
    case FL_DRAG:
    case FL_RELEASE:
      // these are currently ignored.
      return 1;

    default:
      break;
  }

  return 0;
}


//------------------------------------------------------------------------
//  BASE Window class
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

#include "w_base.h"

#ifndef WIN32
#include <unistd.h>
#endif


WindowBase::WindowBase(int X, int Y, int W, int H, const char *title) :
    Fl_Window(X, Y, W, H, title),
    want_quit(false),
    init_x(-999), init_y(-999),
    init_w(-999), init_h(-999)
{
  // turn off FLTK's "auto-add-widget" mode
  Fl_Group::current(0);
}

WindowBase::~WindowBase()
{
  /* nothing to do */
}


void WindowBase::CaptureInitialPos()
{
  // read initial pos, giving approx 1/5th of a second for the WM to
  // adjust our window's position (naughty WM...)
  SmallDelay();

  init_x = x();
  init_y = y();
  init_w = w();
  init_h = h();
}

bool WindowBase::HasBeenMoved() const
{
  return (x() != init_x || y() != init_y);
}

bool WindowBase::HasBeenResized() const
{
  return (w() != init_w || h() != init_h);
}


void WindowBase::RunModal()
{
  while (! CheckQuit())
  {
    Fl::wait();
  }
}


//
// SmallDelay
//
// This routine is meant to delay a short time to allow the window
// manager to move our windows around.
//
void WindowBase::SmallDelay() const
{
#ifndef WIN32
  Fl::wait(0);  usleep(100 * 1000);
  Fl::wait(0);  usleep(100 * 1000);
#endif

  Fl::wait(0);
}


void WindowBase::quit_callback(Fl_Widget *w, void *)
{
  ASSERT(w);

  WindowBase *base = static_cast<WindowBase *> (w);

  base->SetQuit();
}


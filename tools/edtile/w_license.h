//------------------------------------------------------------------------
//  LICENSE Window
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

#ifndef __W_LICENSE_H__
#define __W_LICENSE_H__

#include <FL/Fl_Browser.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Window.H>

#include "w_base.h"


class LicenseWin : public WindowBase
{
  /* Simple class that shows the license text (GPL) */

  public:
    LicenseWin();
    virtual ~LicenseWin();

  private:
    Fl_Group *group;
    Fl_Button *quit;
    Fl_Browser *browser;

    // minimum window size
    static const int min_w = 500;
    static const int min_h = 240;

    static void quit_but_callback(Fl_Widget *w, void *data);
    // callback for quit button -- data must be a pointer to the
    // active license window which contains the button.

    static const char *license_text[];
};


#endif  /* __W_LICENSE_H__ */

//------------------------------------------------------------------------
//  TAB of Main window
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

#ifndef __W_TAB_H__
#define __W_TAB_H__

#include "FL/Fl_Group.H"


class BaseTab : public Fl_Group
{
  /* provides a single "Tab" of the main window.  Only one tab is
   * active at a time, and the user can switch between them.  This
   * class is an abstract base class for the actual tabs.
   */

  public:
    BaseTab();
    virtual ~BaseTab();

    virtual const char *Name() const = 0;
    // return the name for this Tab.

  protected:
    virtual int handle_key(int key) { return 0; }
    // handle a keyboard event.

  public:
    int handle(int event);
    // FLTK virtual event handling function.
};


#endif  /* __W_TAB_H__ */

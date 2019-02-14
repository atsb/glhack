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

using namespace std;

#include "z_config.h"

#include "w_tab.h"

    
BaseTab::BaseTab() : Fl_Group(0, 0, 30, 30)
{ 
  Fl_Group::current(0);
}

BaseTab::~BaseTab()
{
  /* nothing to do */
}

int BaseTab::handle(int event)
{
  if (Fl_Group::handle(event))
    return 1;

  // make sure we keep the focus
  if (event == FL_FOCUS || event == FL_UNFOCUS)
    return 1;
  
  if (event == FL_KEYDOWN)
  {
    if (handle_key(Fl::event_key()))
      return 1;

    // eat everything but ALT key combinations
    if (! Fl::event_state(FL_ALT) && ! Fl::event_state(FL_META))
      return 1;
  }

  return 0;
}

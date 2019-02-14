//------------------------------------------------------------------------
//  PANEL of Main window
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

#ifndef __W_PANEL_H__
#define __W_PANEL_H__

#include "FL/Fl_Box.H"
#include "FL/Fl_Group.H"

#include <vector>

#include "w_tab.h"
#include "w_webbut.h"


class MainPanel : public Fl_Group
{
  /* provides a side panel on the left of the main window, showing the
   * tab names so that the user can select them.
   */

  public:
    MainPanel(int X, int Y, int W, int H, const char *label = 0);
    virtual ~MainPanel();

    void AddTab(BaseTab *tab);
    // adds a new tab to the panel.

    void ChangeTab(int t_num);
    void ChangeTab(BaseTab *tab);
    // change the tab to the one given.

  private:
    vector <BaseTab *> tabs;
    
    vector <WebButton *> names;
    
    int cur_tab;

    static void panel_do_change(Fl_Widget *w, void *data);
};


#endif  /* __W_PANEL_H__ */

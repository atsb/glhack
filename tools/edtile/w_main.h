//------------------------------------------------------------------------
//  MAIN Window
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

#ifndef __W_MAIN_H__
#define __W_MAIN_H__

#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Window.H>

#include <vector>

#include "w_base.h"
#include "w_tab.h"
#include "w_panel.h"


class MainWin : public WindowBase
{
  /* The main application window.  This class is meant to be pretty
   * simple, basically a focus point for controlling the main window,
   * including the menu bar.
   */

  friend class MainPanel;  /* access to `panel' member */

  public:
    // minimum main window size
    static const int min_w = 600;
    static const int min_h = 440;
    
    static MainWin *appwin;
    // global pointer to the main window.  Set it in main().
     
    MainWin(int X, int Y, int W, int H, const char *title);
    virtual ~MainWin();

    void AddTab(BaseTab *tab);
    // adds a new tab to the main window.
    
    void ChangeTab(BaseTab *tab);
    // changes the current tab to the one given.  Should only be
    // called by the panel.
  
    static const Fl_Color bg_gray;

    void ChangeCursor(Fl_Cursor new_c);
    // routine for changing the cursor.  Can be called repeatedly
    // (e.g. on FL_DRAG events) since only changes will get sent to
    // the system.
   
  private:
    BaseTab *cur_tab;
    // current tab, possibly zero.  All tabs remain hidden except for
    // the current one.

    static const int menubar_h = 30;

    Fl_Menu_Bar *menu_bar;
    // menu bar at top of window.

    static Fl_Menu_Item menu_items[];

    Fl_Cursor curr_curs;
    // currently active cursor

    static const int panel_w = 100;

    MainPanel *panel;
    // panel at left of window, for selecting tabs.

    Fl_Box *resize_box;

  private:
    static void menu_do_nothing(Fl_Widget *w, void * data);
    static void menu_do_exit(Fl_Widget *w, void * data);
    static void menu_do_save(Fl_Widget *w, void * data);
};

inline void MainWin::ChangeCursor(Fl_Cursor new_c)
{
  if (new_c != curr_curs)
  {
    curr_curs = new_c;
    cursor(curr_curs);
  }
}


#endif  /* __W_MAIN_H__ */

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

using namespace std;

#include "z_config.h"

#include "w_main.h"
#include "w_tab.h"
#include "w_panel.h"
#include "w_color.h"

 
MainWin *MainWin::appwin = 0;

const Fl_Color MainWin::bg_gray = P_GREY(25);


MainWin::MainWin(int X, int Y, int W, int H, const char *title) :
    WindowBase(X, Y, W, H, title),
    curr_curs(FL_CURSOR_DEFAULT)
{
  size_range(min_w, min_h);

  color(bg_gray, bg_gray);

  callback(WindowBase::quit_callback);

  cur_tab = 0;

  menu_bar = new Fl_Menu_Bar(0, 0, w(), menubar_h);
  menu_bar->menu(menu_items);
  add(menu_bar);


  panel = new MainPanel(0, menubar_h, panel_w, h() - menubar_h);
  add(panel);


  resize_box = new Fl_Box(panel_w, menubar_h, W - panel_w, H - menubar_h);
  add(resize_box);
  resizable(resize_box);


  //...

  // show window (pass some dummy arguments)
  int argc = 1;
  char *argv[] = { PACKAGE, NULL };

  show(argc, argv);

  CaptureInitialPos();
}

MainWin::~MainWin()
{
  resizable(0);
    
  //...

  remove(panel);
  delete panel;  panel = 0;

  remove(menu_bar);
  delete menu_bar;  menu_bar = 0;
}


void MainWin::AddTab(BaseTab *tab)
{
  panel->AddTab(tab);

  if (! cur_tab)
  {
    cur_tab = tab;
    cur_tab->take_focus();
  }
  else
    tab->hide();

  add(tab);

  redraw();
}

void MainWin::ChangeTab(BaseTab *tab)
{
  if (cur_tab)
  {
    cur_tab->hide();
  }

  cur_tab = tab;

  cur_tab->resize(panel_w, menubar_h, w() - panel_w, h() - menubar_h);
  cur_tab->show();

  panel->ChangeTab(tab);

  redraw();

  cur_tab->take_focus();
}


//------------------------------------------------------------------------

void MainWin::menu_do_nothing(Fl_Widget *w, void * data)
{
  /* nothing here */
}

void MainWin::menu_do_exit(Fl_Widget *w, void * data)
{
  if (appwin)
  {
    appwin->SetQuit();
  }
}

extern void DoSave();

void MainWin::menu_do_save(Fl_Widget *w, void * data)
{
  if (! appwin)
    return;

  DoSave();
}


#undef FCAL
#define FCAL(func)  MainWin::func

Fl_Menu_Item MainWin::menu_items[] =
{
  { "&File", 0, 0, 0, FL_SUBMENU },
    { "&Save...",           0, FCAL(menu_do_save) },
    { "&Preferences...",    0, FCAL(menu_do_nothing) },
    { "E&xit",   FL_ALT + 'q', FCAL(menu_do_exit) },
    { 0 },

  { "&Help", 0, 0, 0, FL_SUBMENU },
    { "&About...",         0,  FCAL(menu_do_nothing) },
    { "&License...",       0,  FCAL(menu_do_nothing) },
    { "&Manual...",   FL_F+1,  FCAL(menu_do_nothing) },
    { 0 },

  { 0 }
};

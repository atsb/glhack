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

using namespace std;

#include "z_config.h"

#include "w_panel.h"
#include "w_color.h"
#include "w_main.h"

    
MainPanel::MainPanel(int X, int Y, int W, int H, const char *label) :
    Fl_Group(X, Y, W, H, label)
{ 
  Fl_Group::current(0);

  box(FL_DOWN_BOX);

  color(P_GREY(90), P_GREY(90));

  cur_tab = -1;

  //...
}

MainPanel::~MainPanel()
{
  //... delete tabs[]
}


void MainPanel::AddTab(BaseTab *tab)
{
  int cur_num = tabs.size();

  tabs.push_back(tab);

  WebButton *webbut = new WebButton(x() + 10, y() + 6 + cur_num * 32, 
      80, 30, tab->Name());
  webbut->color(P_GREY(90));
  webbut->labelfont(FL_HELVETICA | FL_BOLD);
  webbut->callback(panel_do_change);

  if (cur_tab < 0)
    webbut->value(1);

  names.push_back(webbut);

  add(webbut);

  if (cur_tab < 0)
    cur_tab = cur_num;

  redraw();
}

void MainPanel::ChangeTab(int t_num)
{
  if (cur_tab >= 0)
    names[cur_tab]->value(0);
   
  cur_tab = t_num;

  if (cur_tab >= 0)
  {
    names[cur_tab]->value(1);
  }

  redraw();
}

void MainPanel::ChangeTab(BaseTab *tab)
{
  for (int i=0; i < int(tabs.size()); i++)
  {
    if (tab == tabs[i])
    {
      ChangeTab(i);
      break;
    }
  }
}

void MainPanel::panel_do_change(Fl_Widget *w, void *data)
{
  if (!w || !MainWin::appwin)
    return;

  MainPanel *pan = MainWin::appwin->panel;
    
  for (int i=0; i < int(pan->names.size()); i++)
  {
    if (w == (Fl_Widget *) pan->names[i])
    {
      MainWin::appwin->ChangeTab(pan->tabs[i]);
      break;
    }
  }
}

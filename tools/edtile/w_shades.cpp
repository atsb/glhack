//------------------------------------------------------------------------
//  SHADE Browsing Tab
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

#include "FL/fl_draw.H"

#include "w_shades.h"
#include "w_color.h"
#include "w_tlinfo.h"


#define FILL_COL  P_GREY(50)


ShadeBrowser::ShadeBrowser(int X, int Y, int W, int H, const char *label) : 
        Fl_Widget(X, Y, W, H, label),
        set(0), size(24)
{
  //...
}

ShadeBrowser::~ShadeBrowser()
{
  //...
}

void ShadeBrowser::ChangeMap(HueSet *hueset)
{
  set = hueset;

  Fl_Widget::resize(x(), y(), 32 * (size + 2), (size + 4) * set->num_grp);
 
  redraw();
}

void ShadeBrowser::DrawOneGroup(int xx, int yy, int grp)
{
  if (fl_not_clipped(xx, yy, 32 * (size + 2), size + 4) == 0)
    return;

  fl_color(FILL_COL);
  fl_rectf(xx, yy, 32 * (size + 2), size + 4);

  int sub;

  for (sub=0; sub < set->grp_len[grp]; sub++)
  {
    Pix_t pix = set->pix_arr[set->grp_start[grp] + sub];

///!!!!    const PackCol& col = set->cmap->c_array[pix];
    
///!!!!    fl_rectf(xx + 2 + sub * (size+2), yy + 2, size, size,
///!!!!        col.r, col.g, col.b);
  }
}

void ShadeBrowser::draw()
{
  if (! set)
  {
    fl_color(FL_BLACK);
    fl_rectf(x(), y(), w(), h());
    return;
  }

///  if (fl_not_clipped(0, y(), w(), 4))
///  {
///    fl_color(FILL_COL);
///    fl_rectf(0, y(), w(), 4);
///  }

  int grp;

  for (grp=0; grp < set->num_grp; grp++)
  {
    DrawOneGroup(x(), y() + grp * (size + 4), grp);
  }

#if 0
  /* show selected tile */

  if (selected > 0)
  {
    int R = selected / cols;
    int C = selected % cols;

    int xx = x() + C * each_w;
    int yy = y() + R * each_h;

    fl_color(FL_GREEN);

    fl_rect(xx + 2, yy - 1, each_w - 1, each_h - 1);
  }
#endif
}

void ShadeBrowser::resize(int X, int Y, int W, int H)
{
  Fl_Widget::resize(X, Y, W, H);
}
 
int ShadeBrowser::handle(int event)
{
  switch (event)
  {
    case FL_PUSH:
//      R = (Fl::event_y() - y()) / each_h;
//      C = (Fl::event_x() - x()) / each_w;
//      selected = R * cols + C;
      redraw();
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


//------------------------------------------------------------------------


ShadeTab::ShadeTab() : BaseTab()
{ 
  scroll = new Fl_Scroll(x(), y(), w(), h() - 0);
  scroll->end();
  scroll->color(FILL_COL, FILL_COL);

  s_brow = new ShadeBrowser(0, 0, 40, 40);
  scroll->add(s_brow);

  add(scroll);
//   resizable(scroll);
}

ShadeTab::~ShadeTab()
{
  /* nothing to do */
}


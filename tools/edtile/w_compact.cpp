//------------------------------------------------------------------------
//  COMPACT Browsing Tab
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

#include "w_compact.h"
#include "w_color.h"
#include "w_tlinfo.h"


#define FILL_COL  P_GREY(50)


CompactBrowser::CompactBrowser(int X, int Y, int W, int H, const char *label) : 
        Fl_Widget(X, Y, W, H, label),
        set(0), 
        each_w(16), each_h(16),
        cols(1), rows(1),
        zoom(1),
        selected(3)   /// -1
{
  //...
}

CompactBrowser::~CompactBrowser()
{
  //...
}

void CompactBrowser::ChangeTiles(Tileset *new_set)
{
  set = new_set;

  each_w = 4 + MAX(16, set->tile_w) * zoom;
  each_h = 4 + MAX(16, set->tile_h) * zoom;

  cols = 16;  // !!!! MAX(1, 360 / each_w);
  rows = (set->total + cols - 1) / cols;
   
  Fl_Widget::resize(x(), y(), 52 + cols * each_w, rows * each_h);
   
  redraw();
}

void CompactBrowser::ChangeZoom(int new_zoom)
{
  ASSERT(1 <= zoom && zoom <= 4);

  zoom = new_zoom;

  each_w = 4 + MAX(16, set->tile_w) * zoom;
  each_h = 4 + MAX(16, set->tile_h) * zoom;
    
  // !!! FIXME: duplicated code
  cols = 16;  // !!!! MAX(1, 360 / each_w);
  rows = (set->total + cols - 1) / cols;
   
  Fl_Widget::resize(x(), y(), 52 + cols * each_w, rows * each_h);
   
  redraw();
}


void CompactBrowser::DrawOneTile(int idx, Tile *tile)
{
  int R = idx / cols;
  int C = idx % cols;

  int xx = x() + C * each_w;
  int yy = y() + R * each_h;

  if (fl_not_clipped(xx, yy, each_w, each_h) == 0)
    return;

  if (C == 0)
  {
    fl_color(FILL_COL);
    fl_rectf(xx, yy, 52, each_h);

    char buf[20];
    sprintf(buf, "%4d", idx);
  
    int text_y = yy + each_h-4 - (each_h-4 - fl_height()) / 2 - fl_descent();

    fl_color(FL_WHITE);
    fl_draw(buf, xx+6, text_y);
  }
 
  xx += 52;

  fl_color(FILL_COL);
  fl_rectf(xx, yy, each_w, each_h);

  /* draw tile image */

  TileExtra::Draw(xx, yy, set, tile, zoom);
}

void CompactBrowser::draw()
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

  fl_font(FL_COURIER, 16);

  int idx;

  for (idx=0; idx < set->total; idx++)
  {
    DrawOneTile(idx, set->tiles + idx);
  }

  /* show selected tile */

  if (selected >= 0)
  {
    int R = selected / cols;
    int C = selected % cols;

    int xx = x() + C * each_w;
    int yy = y() + R * each_h;

    fl_color(FL_GREEN);
    fl_rect(xx + 52 - 2, yy - 2, each_w, each_h);
    fl_rect(xx + 52 - 3, yy - 3, each_w+2, each_h+2);
  }
}

void CompactBrowser::resize(int X, int Y, int W, int H)
{
  Fl_Widget::resize(X, Y, W, H);
}
 
int CompactBrowser::handle(int event)
{
  int R, C;

  switch (event)
  {
    case FL_PUSH:
      R = (Fl::event_y() - y()) / each_h;
      C = (Fl::event_x() - x() - 52) / each_w;

      if (0 <= R && R < rows && 0 <= C && C < cols)
        selected = R * cols + C;
      else
        selected = -1;
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

int CompactBrowser::handle_key(int key)
{
  Tile *cur = CurrentTile();
  if (! cur)
    return 0;

  switch (key)
  {
    case 'u': cur->Wrap(0, -1); break;
    case 'd': cur->Wrap(0, +1); break;
    case 'l': cur->Wrap(-1, 0); break;
    case 'r': cur->Wrap(+1, 0); break;

    default: return 0;
  }

  redraw();

  return 1;
}


//------------------------------------------------------------------------


CompactTab::CompactTab() : BaseTab()
{ 
  scroll = new Fl_Scroll(x(), y(), w(), h() - 0);
  scroll->end();
  scroll->color(FILL_COL, FILL_COL);

  t_brow = new CompactBrowser(0, 0, 40, 40);
  scroll->add(t_brow);

  add(scroll);
//   resizable(scroll);
}

CompactTab::~CompactTab()
{
  /* nothing to do */
}


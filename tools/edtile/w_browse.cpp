//------------------------------------------------------------------------
//  BROWSING Tab
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

#include "w_browse.h"
#include "w_color.h"
#include "w_tlinfo.h"


#define FILL_COL  P_GREY(50)


TileBrowser::TileBrowser(int X, int Y, int W, int H, const char *label) : 
        Fl_Widget(X, Y, W, H, label),
        set(0), 
        each_h(16),
        zoom(1),
        selected(2)   /// -1
{
  //...
}

TileBrowser::~TileBrowser()
{
  //...
}

void TileBrowser::ChangeTiles(Tileset *new_set)
{
  set = new_set;

  // !!! FIXME: duplicate code
  each_h = 4 + MAX(16, set->tile_h) * zoom;
   
  Fl_Widget::resize(x(), y(), 800, each_h * set->total);
   
  redraw();
}

void TileBrowser::ChangeZoom(int new_zoom)
{
  ASSERT(1 <= zoom && zoom <= 4);

  zoom = new_zoom;

  each_h = 4 + MAX(16, set->tile_h) * zoom;
    
  Fl_Widget::resize(x(), y(), 800, each_h * set->total);
   
  redraw();
}


void TileBrowser::DrawOneTile(int idx, Tile *tile)
{
  int xx = x();
  int yy = y() + idx * each_h;

  if (fl_not_clipped(xx, yy, w(), each_h) == 0)
    return;

  fl_color(FILL_COL);
  fl_rectf(0, yy, w(), each_h);

  /* draw text */

  xx += 6;

  int text_y = yy + each_h-4 - (each_h-4 - fl_height()) / 2 - fl_descent();
   
  fl_color(FL_WHITE);

  if (tile->tile_id >= 0)
  {
    char buf[20];
    sprintf(buf, "%4d", tile->tile_id);
  
    fl_draw(buf, xx, text_y);
  }
 
  xx += 46;

  if (tile->name)
    fl_draw(tile->name, xx + set->tile_w * zoom + 8, text_y);

  /* draw tile image */

  TileExtra::Draw(xx, yy, set, tile, zoom);
}

void TileBrowser::draw()
{
  if (! set)
  {
    fl_color(FL_BLACK);
    fl_rectf(x(), y(), w(), h());
    return;
  }

  if (fl_not_clipped(0, y(), w(), 4))
  {
    fl_color(FILL_COL);
    fl_rectf(0, y(), w(), 4);
  }

  fl_font(FL_COURIER, 16);

  int idx;

  for (idx=0; idx < set->total; idx++)
  {
    DrawOneTile(idx, set->tiles + idx);
  }

  /* show selected tile */

  if (selected >= 0)
  {
    int xx = x();
    int yy = y() + selected * each_h;

    fl_color(FL_GREEN);
    fl_rect(xx + 4, yy - 2, 300, each_h);
    fl_rect(xx + 3, yy - 3, 302, each_h+2);
  }
}

void TileBrowser::resize(int X, int Y, int W, int H)
{
  Fl_Widget::resize(X, Y, W, H);
}
 
int TileBrowser::handle(int event)
{
  int R;

  switch (event)
  {
    case FL_PUSH:
      R = (Fl::event_y() - y()) / each_h;
      if (0 <= R && R < set->total)
        selected = R;
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


//------------------------------------------------------------------------

BrowserTab::BrowserTab() : BaseTab()
{ 
  scroll = new Fl_Scroll(x(), y(), w(), h() - 0);
  scroll->end();

  t_brow = new TileBrowser(0, 0, 40, 40);
  scroll->add(t_brow);

  add(scroll);
//   resizable(scroll);
}

BrowserTab::~BrowserTab()
{
  /* nothing to do */
}


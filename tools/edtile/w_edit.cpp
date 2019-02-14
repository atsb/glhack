//------------------------------------------------------------------------
//  EDITING Tab
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

#include "w_edit.h"
#include "w_color.h"
#include "w_tlinfo.h"


#define FILL_COL  P_GREY(50)


TileEditor::TileEditor(int X, int Y, int W, int H, const char *label) : 
        Fl_Widget(X, Y, W, H, label),
        tile(0), 
        zoom(8)
{
  //...
}

TileEditor::~TileEditor()
{
  //...
}

void TileEditor::ChangeTile(Tileset *new_set, Tile *new_tile)
{
  set  = new_set;
  tile = new_tile;

  Fl_Widget::resize(x(), y(), tile->width * zoom, tile->height * zoom);
   
  redraw();
}

void TileEditor::ChangeZoom(int new_zoom)
{
  ASSERT(1 <= zoom);

  zoom = new_zoom;

  Fl_Widget::resize(x(), y(), tile->width * zoom, tile->height * zoom);
   
  redraw();
}


void TileEditor::draw()
{
  if (! tile)
  {
    fl_color(FL_BLACK);
    fl_rectf(x(), y(), w(), h());
    return;
  }

  int xx = x();
  int yy = y();

  TileExtra::Draw(xx, yy, set, tile, zoom);
}

void TileEditor::resize(int X, int Y, int W, int H)
{
  Fl_Widget::resize(X, Y, W, H);
}
 
int TileEditor::handle(int event)
{
  switch (event)
  {
    case FL_PUSH:
    case FL_DRAG:
      // FIXME: change some pixels...
      return 1;
     
    case FL_RELEASE:
      // currently ignored.
      return 1;

    default:
      break;
  }

  return 0;
}


//------------------------------------------------------------------------


EditorTab::EditorTab() : BaseTab()
{ 
  box(FL_FLAT_BOX);
  color(FILL_COL, FILL_COL);

  t_edit = new TileEditor(4, 4, 8, 8);
  add(t_edit);

  resizable(0);
}

EditorTab::~EditorTab()
{
  /* nothing to do */
}


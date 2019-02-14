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

#ifndef __W_EDIT_H__
#define __W_EDIT_H__

#include "FL/Fl_Widget.H"
#include "FL/Fl_Group.H"

#include "w_tab.h"

#include "t_tiles.h"


class TileEditor : public Fl_Widget
{
  public:
    TileEditor(int X, int Y, int W, int H, const char *label = 0);
    ~TileEditor();

    void ChangeTile(Tileset *new_set, Tile *new_tile);
    // changes which tile to edit.
     
    void ChangeZoom(int new_zoom);
    // changes the current zoom factor.
    
  public:
    int handle(int event);
    // FLTK virtual method for handling input events.

    void resize(int X, int Y, int W, int H);
    // FLTK virtual method for resizing.
   
  private:
    void draw();
    // FLTK virtual method for drawing.
   
  private:
    Tileset *set;
    Tile *tile;

    int zoom;
    // zoom factor (number of on-screen pixels for each tile pixel).

};


class EditorTab : public BaseTab
{
  public:
    EditorTab();
    virtual ~EditorTab();

    const char *Name() const { return "Edit"; }
   
    void ChangeTile(Tileset *new_set, Tile *new_tile)
    {
      ASSERT(t_edit);
      t_edit->ChangeTile(new_set, new_tile);
    }

    void ChangeZoom(int new_zoom)
    {
      ASSERT(t_edit);
      t_edit->ChangeZoom(new_zoom);
    }
    
  private:
    TileEditor *t_edit;
};


#endif  /* __W_EDIT_H__ */

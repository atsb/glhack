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

#ifndef __W_COMPACT_H__
#define __W_COMPACT_H__

#include "FL/Fl_Group.H"
#include "FL/Fl_Scroll.H"

#include "w_tab.h"

#include "t_tiles.h"


class CompactBrowser : public Fl_Widget
{
  public:
    CompactBrowser(int X, int Y, int W, int H, const char *label = 0);
    ~CompactBrowser();

    void ChangeTiles(Tileset *set);
    // changes the tileset to view.
        
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
   
  public:
    int handle_key(int key);

  private:
    Tileset *set;

    int each_w, each_h;
    // pixels per tile

    int cols, rows;
    // number of rows and columns.
      
    int zoom;
    // zoom factor: 1, 2 or 4.

    int selected;
    // currently selected tile, or -1

    void DrawOneTile(int idx, Tile *tile);

    Tile *CurrentTile() const
    {
      if (selected < 0) 
        return NULL;
      
      if (! set)
        return NULL;
      
      return set->tiles + selected;
    }
};


class CompactTab : public BaseTab
{
  public:
    CompactTab();
    virtual ~CompactTab();

    const char *Name() const { return "Compact"; }
   
    void ChangeTiles(Tileset *new_set)
    {
      ASSERT(t_brow);
      t_brow->ChangeTiles(new_set);
    }

    void ChangeZoom(int new_zoom)
    {
      ASSERT(t_brow);
      t_brow->ChangeZoom(new_zoom);
    }
   
  protected:
    int handle_key(int key)
    {
      if (! t_brow)
        return 0;

      return t_brow->handle_key(key);
    }
   
  private:
    CompactBrowser *t_brow;
 
    Fl_Scroll *scroll;
};


#endif  /* __W_COMPACT_H__ */

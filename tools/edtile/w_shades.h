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

#ifndef __W_SHADES_H__
#define __W_SHADES_H__

#include "FL/Fl_Group.H"
#include "FL/Fl_Scroll.H"

#include "w_tab.h"

#include "t_hue.h"


class ShadeBrowser : public Fl_Widget
{
  public:
    ShadeBrowser(int X, int Y, int W, int H, const char *label = 0);
    ~ShadeBrowser();

    void ChangeMap(HueSet *hueset);
    // changes the hue-set to view.
        
//    void ChangeZoom(int new_zoom);
//    // changes the current zoom factor.
    
  public:
    int handle(int event);
    // FLTK virtual method for handling input events.

    void resize(int X, int Y, int W, int H);
    // FLTK virtual method for resizing.
   
  private:
    void draw();
    // FLTK virtual method for drawing.
   
  private:
    HueSet *set;

    int size;
    // pixels per square

    void DrawOneGroup(int xx, int yy, int grp);
};


class ShadeTab : public BaseTab
{
  public:
    ShadeTab();
    virtual ~ShadeTab();

    const char *Name() const { return "Colors"; }
   
    void ChangeMap(HueSet *hueset)
    {
      ASSERT(s_brow);
      s_brow->ChangeMap(hueset);
    }

//    void ChangeZoom(int new_zoom)
//    {
//      ASSERT(s_brow);
//      s_brow->ChangeZoom(new_zoom);
//    }
   
  private:
    ShadeBrowser *s_brow;
    
    Fl_Scroll *scroll;
};


#endif  /* __W_SHADES_H__ */

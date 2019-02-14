//------------------------------------------------------------------------
//  BASE Window class
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

#ifndef __W_BASE_H__
#define __W_BASE_H__

#include <FL/Fl_Window.H>


class WindowBase : public Fl_Window
{
  /* This class is meant to be a wrapper around Fl_Window which
   * provides a small number of useful services, such as handling the
   * problem with some window managers reporting a different position
   * than the window was opened at.  This class is meant to be
   * subclassed, especially for the main app window.
   */
  
  public:
    WindowBase(int X, int Y, int W, int H, const char *title);
    // this constructor will call end(), the Fl_Group method, turning
    // off FLTK's rather dubious (IMO) "auto-add" feature.

    virtual ~WindowBase();

    void SetQuit();
    // call this when the user wants to close the window.

    bool CheckQuit() const;
    // checks if the user wants to quit (i.e. SetQuit was called).
    
    void RunModal();
    // runs the window until the user closes it (or otherwise signals
    // to quit).  Only useful for (a) the main app window and (b)
    // modal windows.

    bool HasBeenMoved() const;
    bool HasBeenResized() const;
    // returns true if the window has been moved/resized by the user,
    // i.e. the current position doesn't match the captured initial
    // position.

    bool MovedOrResized() const
    {
      return HasBeenMoved() || HasBeenResized();
    }
    
  protected:
    void CaptureInitialPos();
    // capture the initial window position.  Call this straight after
    // calling show() on the window (in the constructor).

    static void quit_callback(Fl_Widget *w, void *data);
    // callback function for Fl_Window, which calls SetQuit().
    // Designed to be used in derived classes.

  private:
    // user wants to quit/close the window
    bool want_quit;
    
    // initial window position, read after the window manager has had
    // a chance to move the window somewhere else.  If the window is
    // still there at capture-state time, then we know the user didn't
    // move the window.
    // 
    int init_x, init_y, init_w, init_h;

    void SmallDelay() const;
};

inline void WindowBase::SetQuit()
{
  want_quit = true;
}

inline bool WindowBase::CheckQuit() const
{
  return want_quit;
}


#endif  /* __W_BASE_H__ */

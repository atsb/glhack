//----------------------------------------------------------------------------
//  COLORMAP for tiles
//----------------------------------------------------------------------------
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

#ifndef __T_COLMAP_H__
#define __T_COLMAP_H__

#include "z_rgb.h"
#include "z_file.h"


/* index into tileset's colormap */
typedef unsigned short Pix_t;

#define NO_PIX  ((Pix_t) 0x7FFF)

#define TRANS_COL     PackCol(0, 255, 255)
#define TRANS_SLAM    PackCol(71, 108, 108)
#define TRANS_ALLEG   PackCol(255, 0, 255)


struct M_Color
{
  PackCol c;

  char pair[3];
  // gives the symbol used in the TXT file.  If <= 62 colors is used,
  // then only character is needed (the second one is NUL), otherwise
  // two characters are used.  New colors, or colors from a different
  // source, have an empty string -- if these need to be written out,
  // a unique pair must be allocated.

  int flags;
};

#define MCOL_F_HUE      0x0001    /* color is a hue */
#define MCOL_F_USED     0x0010    /* color was used within a tile */
#define MCOL_F_SAME_COL   0x0020    /* color is a duplicate of an earlier one */


class ColorMapping
{
  public:
    ColorMapping(bool open_ = false);
    ~ColorMapping();

    static ColorMapping * Load(const Filename& fname);

    const PackCol& At(Pix_t index) const;

    Pix_t LookupExact(const PackCol& col) const;
    Pix_t LookupExact(int r, int g, int b) const;
    // find an exact match for the given color, otherwise return
    // NO_PIX.
      
    Pix_t LookupClosest(const PackCol& col) const;
    Pix_t LookupClosest(int r, int g, int b) const;
    // find the closest color in the colormap.  Returns NO_PIX only if
    // colormap is empty.  The transparent color is ignored.
     
    bool IsHue(Pix_t pix) const;
    // returns true if the pixel is one of the hues.
     
    Pix_t AddColor(const PackCol& col, const char *pair = "");
    Pix_t AddColor(int r, int g, int b, const char *pair = "");
    // used by the tile loading code to add a color.  The color is
    // assumed to be unique.  Returns the new index.

    Pix_t * ConversionMap(const ColorMapping *target) const;
    // returns a mapping table that maps pixels within this colormap
    // to pixels within the target colormap.

// ####     void ImportPairs(const ColorMapping *target);
    // sets the pair names in this map to the ones in the target,
    // using the closest color in the target.  This means that
    // duplicate pair names may exist afterwards.

    void BeginMarking();
    void MarkPix(Pix_t pix);
    // used to mark which colors are used with a tileset.

    Pix_t *ComputeSameMap() const;
    // determines which colors are the same.  Returns a mapping table
    // that is indexed by Pix_t, giving the lowest Pix_t value with
    // the exact same color.  When table[nn] != nn, the color is
    // redundant and doesn't need to be written into the tile file.
    //
    // Use delete[] when finished with the table.
     
  public:
    static const int max_cols = 4096;

    int num_cols;
    M_Color m_arr[max_cols];

    Pix_t trans_col;
    // transparent index, or NO_PIX if none.

    Pix_t dark_col;
    // black, or the closest color to it.

    Pix_t light_col;
    // white, or the closest color to it.

    bool open;
    // when true, colors which are not present in the colormap when
    // loading a tileset are allowed to be added.
   
  private:
    static const class ColorMapping *compare_cmap;
    // kludge to pass cmap pointer to sort routines.

    void AddRaw(const char *line);

    static int ColorComparer(const void *A, const void *B);
    // compare two colors, for sorting.
};

inline const PackCol& ColorMapping::At(Pix_t index) const
{
  ASSERT(index != NO_PIX);
  ASSERT(index < num_cols);
  
  return m_arr[index].c;
}

inline Pix_t ColorMapping::LookupExact(int r, int g, int b) const
{
  return LookupExact(PackCol(r, g, b));
}

inline Pix_t ColorMapping::LookupClosest(int r, int g, int b) const
{
  return LookupClosest(PackCol(r, g, b));
}

inline Pix_t ColorMapping::AddColor(int r, int g, int b, const char *pair)
{
  return AddColor(PackCol(r, g, b), pair);
}

inline bool ColorMapping::IsHue(Pix_t pix) const
{
  ASSERT(pix != NO_PIX);

  return (m_arr[pix].flags & MCOL_F_HUE) ? true : false;
}

inline void ColorMapping::MarkPix(Pix_t pix)
{
  if (pix != NO_PIX)
  {
    ASSERT(pix < num_cols);

    m_arr[pix].flags |= MCOL_F_USED;
  }
}


namespace T_Colmap
{
  extern ColorMapping *cmap16;
  extern ColorMapping *cmap32;
}


#endif  /* __T_COLMAP_H__ */

//------------------------------------------------------------------------
//  TILE Sets
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

#ifndef __T_TILES_H__
#define __T_TILES_H__

#include "z_file.h"

#include "t_alias.h"
#include "t_colmap.h"


#define TL_MAX_TILES  4096

#define TL_MAX_COLS   4096


struct Tile
{
  int tile_id;
  const char *name;

  int width;
  int height;

  Pix_t *pixels;

  /* link in hash chain, -1 if none */
  int hash_next;

  void Create(int w, int h, int id);
  // create the tile.
      
  bool Contains(Pix_t what) const;
  // returns true if at least one of the pixels matches `what'.
  // Useful for transparency checking.

  void Flip();
  // flip horizontally.

  void Clear(Pix_t p);
  // clears the tile.

  void Replace(Pix_t src, Pix_t dst);
  // change all occurances of `src' pixels to `dst'.

  void Wrap(int dx, int dy);
  // moves the tile in the given direction.  Pixels that cross an edge
  // will wrap around to the other side.

  void ApplyConversion(const Pix_t *mapping, int size);
  // convert all pixels according to the given mapping.

  void StoreRGB(ColorMapping *cmap, uint8_g *rgba, 
      int stride, int rgb_w, int rgb_h);
  void StoreRGBA(ColorMapping *cmap, uint8_g *rgba, 
      int stride, int rgb_w, int rgb_h);
  // copy image data into this tile.
};


class Tileset
{
  public:
    Tileset();
    ~Tileset();

    Tile tiles[TL_MAX_TILES];
    int total;

    ColorMapping *cmap;

    int tile_w, tile_h;

    /* true if these tiles don't have any names (only happens when not
     * loaded from the TXT format, e.g. from an image).
     */
    int no_names;

    static Tileset * Create(int width, int height,
        ColorMapping *colmap);
    // create a new (empty) tileset.
     
    int AddTile(Pix_t fill_pix, int t_width = -1, 
        int t_height = -1);
    int AddTile(uint8_g *rgba, int depth, int stride,
        int t_width = -1, int t_height = -1);
    //
    // add a new tile (either blank or from RGB data) and returns the
    // index number.  The `depth' parameter is either 3 or 4, and
    // `stride' gives the number of bytes between rows.  If the width
    // and height paramters are omitted, they default to tile_w and
    // tile_h.  If `fill_pix' is NO_PIX, the new tile is NOT cleared.
 
    static Tileset * Load(const Filename& fname);
    // load the tiles from a standard TXT file.

    void SetTrans(const PackCol& col = TRANS_COL);
    // set the transparent color after loading.
 
    int LookupName(const char *name) const;
    // find the given tilename.  Returns -1 if not found.

    void Save(const Filename& fname);
    void Save(FILE *fp);
    // save the tileset into a standard TXT file.
     
    static char bysx2char(int i);
    static int char2bysx(char c);
    // low-level character conversions.

    bool ConvertTrans(const PackCol& col);
    // Used for handling "foreign" tilesets.  Converts all pixels that
    // match the given color to transparent.  Returns true, unless
    // something went wrong (e.g. given color didn't exist).
    
    void ImportTile(int dst, const Tileset *src_set, int src);
    // import the source tile into the destination tile within this
    // set.  If the colormaps are not the same, a conversion will take
    // place.  If the sizes are not the same, the tile will be scaled
    // to fit (enlarged or shrunk).

    void SkipEmptyNames(int value) { skip_empties = value; }
    // when value is 1, causes empty names to be skipped when saving
    // the tileset.  Default is zero.

    /// bool SwapPair(const char *src, const char *dest);
    // looks for both pairs in the color info array, if found then
    // they are swapped and true is returned.  Returns false if either
    // pair didn't exist.
   
    Pix_t FindPair(const char *pair) const;
    void SwapPair(Pix_t src, Pix_t dest);
    
    void ImportPairNames(const Tileset *src_set);

    void ChangeColormap(ColorMapping *new_map);
    // changes the colormap to the one specified, updating all pixel
    // values, using the closest colors if an exact match does not
    // exist.  If the original colormap has a transparent color, and
    // the target map does not, then it gets converted to the closest
    // opaque color.

  private:
    int hash_tab[T_Alias::hash_size];
    // hash table, using tile indexes.

    int skip_empties; 

    /* data needed only while loading/saving.
     */
    int color_size, got_tile;
    int got_id, got_col_num;
    const char *got_name;

    PackCol *parse_cols;
    Pix_t *parse_map;

    byte *used_pairs;
    
    /* low-level loading routines.
     */
    void DoColor(char *line);
    void DoTag(char *line);
    void AddToHashTable(int index);
    void StartTile();
    void EndTile();
    void TileRow(char *line);
    void HandleLine(char *line);

    /* low-level saving routines
     */
    void FindUsedPairs();
    void AllocatePair(char *pair);
    void WriteColors(FILE *fp);
    void WriteTile(FILE *fp, Tile *tile, int id);

///---     static int InfoComparer(const void *A, const void *B);
///---     // compare two TileColInfo structs, for sorting.
};


/* --------- IMPLEMENTATION --------- */

inline void Tileset::SetTrans(const PackCol& col)
{
  cmap->trans_col = cmap->LookupExact(col);
}


#endif  /* __T_TILES_H__ */

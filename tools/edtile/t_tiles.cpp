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

using namespace std;

#include "z_config.h"

#include "t_tiles.h"
#include "t_colmap.h"
#include "t_alias.h"

#define MAX_LINE  1200
#define MAX_SIZE  512


// FIXME: put into TileSet (dynamic alloc).
static Pix_t PixelBuf[MAX_SIZE][MAX_SIZE];
static int pix_w, pix_h;


void Tile::Create(int w, int h, int id)
{
  tile_id = id;
  width   = w;
  height  = h;

  name = NULL;

  pixels = (Pix_t *) malloc(width * height * sizeof(Pix_t));
  ASSERT(pixels);
}

bool Tile::Contains(Pix_t what) const
{
  for (int y=0; y < height; y++)
  for (int x=0; x < width;  x++)
  {
    if (pixels[y*width + x] == what)
      return true;
  }

  return false;
}

void Tile::Flip()
{
  for (int y=0; y < height;    y++)
  for (int x=0; x < width / 2; x++)
  {
    int x2 = width - 1 - x;

    Pix_t tmp            = pixels[y*width + x];
    pixels[y*width + x]  = pixels[y*width + x2];
    pixels[y*width + x2] = tmp;
  }
}

void Tile::Clear(Pix_t p)
{
  for (int y=0; y < height; y++)
  for (int x=0; x < width;  x++)
  {
    pixels[y*width + x] = p;
  }
}

void Tile::Replace(Pix_t src, Pix_t dst)
{
  for (int y=0; y < height; y++)
  for (int x=0; x < width;  x++)
  {
    if (pixels[y*width + x] == src)
      pixels[y*width + x] = dst;
  }
}

void Tile::Wrap(int dx, int dy)
{
  if (dx == 0 && dy == 0)
    return;

  /* Algorithm: copy to pixel buffer, then copy back */

  int x, y; 

  for (y=0; y < height; y++)
  for (x=0; x < width;  x++)
  {
    PixelBuf[y][x] = pixels[y*width + x];
  }
  
  for (y=0; y < height; y++)
  for (x=0; x < width;  x++)
  {
    int sx = (x - dx + width  * 8) % width;
    int sy = (y - dy + height * 8) % height;
    
    pixels[y*width + x] = PixelBuf[sy][sx];
  }
}

void Tile::ApplyConversion(const Pix_t *mapping, int size)
{
  for (int y=0; y < height; y++)
  for (int x=0; x < width;  x++)
  {
    Pix_t p = pixels[y*width + x];

    if ((int) p >= size)
    {
      fprintf(stderr, "Warning: Bad PIXEL %u at (%d,%d)\n", p, x, y);
      p = 0;
    }
    else
    {
      p = mapping[p];
    }

    pixels[y*width + x] = p;
  }
}

void Tile::StoreRGB(ColorMapping *cmap, uint8_g *rgb, 
      int stride, int rgb_w, int rgb_h)
{
  if (rgb_w > width)
    rgb_w = width;

  if (rgb_h > height)
    rgb_h = height;

  int x, y;

  for (y = 0; y < rgb_h; y++, rgb += stride)
  for (x = 0; x < rgb_w; x++)
  {
    uint8_g r = rgb[x * 3 + 0];
    uint8_g g = rgb[x * 3 + 1];
    uint8_g b = rgb[x * 3 + 2];

    pixels[y * width + x] = cmap->LookupClosest(r, g, b);
  }
}

void Tile::StoreRGBA(ColorMapping *cmap, uint8_g *rgba, 
      int stride, int rgb_w, int rgb_h)
{
  if (rgb_w > width)
    rgb_w = width;

  if (rgb_h > height)
    rgb_h = height;

  int x, y;

  for (y = 0; y < rgb_h; y++, rgba += stride)
  for (x = 0; x < rgb_w; x++)
  {
    uint8_g r = rgba[x * 4 + 0];
    uint8_g g = rgba[x * 4 + 1];
    uint8_g b = rgba[x * 4 + 2];
    uint8_g a = rgba[x * 4 + 3];

    Pix_t p;
    
    if (a < 128 && cmap->trans_col != NO_PIX)
      p = cmap->trans_col;
    else
      p = cmap->LookupClosest(r, g, b);
    
    pixels[y * width + x] = p;
  }
}


/* ---------------------------------------------------------------- */

int Tileset::char2bysx(char c)
{
  if (c == '_') return 0;
  if (c >= 'A' && c <= 'Z') return c - 'A' + 1;
  if (c >= 'a' && c <= 'z') return c - 'a' + 27;
  if (c >= '0' && c <= '9') return c - '0' + 53;
  if (c == '$') return 63;

  return -1;
}

char Tileset::bysx2char(int i)
{
  if (!i) return '_';
  if (i < 27) return 'A' + i - 1;
  if (i < 53) return 'a' + i - 27;
  if (i < 63) return '0' + i - 53;

  return '$';
}


Tileset::Tileset() : total(0), cmap(0), 
    tile_w(0), tile_h(0), no_names(false), skip_empties(0),
    color_size(1), got_tile(0), got_id(-1), got_col_num(0), got_name(0),
    parse_cols(0), parse_map(0), used_pairs(0)
{
  memset(tiles, 0, sizeof(tiles));

  // clear hash table
  for (int i=0; i < T_Alias::hash_size; i++)
    hash_tab[i] = -1;
}

Tileset::~Tileset()
{
  for (int t=0; t < total; t++)
  {
    if (tiles[t].pixels)
      free(tiles[t].pixels);
  }

  memset(tiles, 0, sizeof(tiles));
}


/* ---------------------------------------------------------------- */

void Tileset::DoColor(char *line)
{
  char pair[3];

  pair[0] = *line++;
  pair[1] = 0;
  pair[2] = 0;

  int val = char2bysx(pair[0]);

  if (char2bysx(*line) >= 0)
  {
    pair[1] = *line++;

    val = val * 64 + char2bysx(pair[1]);
    color_size = 2;
  }

  if (val < 0 || val >= TL_MAX_COLS)
  {
    fprintf(stderr, "Illegal color index: %d\n", val);
    exit(23);
  }

  line = strchr(line, '=');

  if (! line)
  {
    fprintf(stderr, "Bad color line (missing =)\n");
    exit(24);
  }

  line++;

  int cr, cg, cb;

  if (3 != sscanf(line, " ( %i , %i , %i ) ", &cr, &cg, &cb))
  {
    fprintf(stderr, "Bad color spec: `%s'\n", line);
    exit(25);
  }

  parse_cols[val] = PackCol(cr, cg, cb);

  parse_map[val] = cmap->LookupExact(cr, cg, cb);

  if (parse_map[val] == NO_PIX)
  {
    if (cmap->open)
    {
      parse_map[val] = cmap->AddColor(cr, cg, cb, pair);

//      fprintf(stderr, "Warning: Unknown color:%s.  Adding it.\n", line);
    }
    else
    {
      parse_map[val] = cmap->LookupClosest(cr, cg, cb);

      const PackCol& col = cmap->At(parse_map[val]);

      fprintf(stderr, "Warning: Unknown color:%s.  Using (%d,%d,%d).\n", 
          line, int(col.r), int(col.g), int(col.b));
    }
  }

///---  Pix_t c_idx = parse_map[val];
///---
///---  if (color_infos[c_idx].pos == NO_PIX)
///---  {
///---    TileColInfo& info = color_infos[c_idx];
///---
///---    info.pos = Pix_t(got_col_num);
///---    info.pair[0] = pair[0];
///---    info.pair[1] = pair[1];
///---  }

  got_col_num++;
}

void Tileset::DoTag(char *line)
{
  char *pos1, *pos2;

  if (sscanf(line+1, " tile %d ", &got_id) != 1)
  {
    fprintf(stderr, "Bad tag ID:\n`%s'\n", line);
    exit(31);
  }

  pos1 = strchr(line,  '(');
  pos2 = strrchr(line, ')');

  if (! pos1 || ! pos2 || pos1 > pos2)
  {
    fprintf(stderr, "Bad tag name:\n`%s'\n", line);
    exit(31);
  }

  /* extract portion between `(' and `)' */

  *(pos2--) = 0;

  while (isspace(*pos2))
    *(pos2--) = 0;
  
  pos1++;

  while (isspace(*pos1))
    pos1++;

  ASSERT(strlen(pos1) > 0);

  got_name = strdup(pos1);
}

void Tileset::AddToHashTable(int index)
{
  Tile *tile = tiles + index;
  int hash;

  ASSERT(tile->name);
 
  tile->hash_next = -1;

  hash = T_Alias::Hash(string(tile->name));

  if (hash_tab[hash] >= 0)
    tile->hash_next = hash_tab[hash];

  hash_tab[hash] = index;
}

void Tileset::StartTile()
{
  if (got_tile != 0)
  {
    fprintf(stderr, "Error: unexpected '{' found.\n");
    exit(26);
  }

  got_tile++;

  pix_w = pix_h = 0;
}

void Tileset::EndTile()
{
  Tile *tile;
  int x, y;

  got_tile--;

  if (got_tile != 0)
  {
    fprintf(stderr, "Error: unexpected '}' found.\n");
    exit(32);
  }

  if (got_id < 0)
  {
    fprintf(stderr, "Missing tile ID !\n");
    exit(33);
  }

  if (!got_name)
  {
    fprintf(stderr, "Missing tile name !\n");
    exit(33);
  }

  /* copy data into Tile structure */

  tile = tiles + total;
  total++;

  if (total >= TL_MAX_TILES)
  {
    fprintf(stderr, "Too many tiles !\n");
    exit(39);
  }
 
  tile->Create(pix_w, pix_h, got_id);

  tile->name = got_name;
  ASSERT(tile->name);

  got_name = NULL;
  got_id = -1;

  for (y=0; y < tile->height; y++)
  for (x=0; x < tile->width;  x++)
  {
    Pix_t pix = PixelBuf[y][x];

    tile->pixels[y * tile->width + x] = pix;
  }

  AddToHashTable(total-1);
}

void Tileset::TileRow(char *line)
{
  int x;
  int len;
  int width;
  
  while (*line && isspace(*line))
    line++;

  len = strlen(line);

  if (len == 0)
    return;

  width = len / color_size;

  if (width > tile_w)
    tile_w = width;

  if (width > pix_w)
    pix_w = width;

  ASSERT(pix_w <= MAX_SIZE);
  ASSERT(pix_h <= MAX_SIZE);
   
  for (x=0; x < len / color_size; x++)
  {
    int val = char2bysx(*line);
    int val2;

    if (val < 0)
    {
      fprintf(stderr, "Bad pixel `%c' found.\n", *line);
      exit(35);
    }
    
    if (color_size == 2)
    {
      line++;

      val2 = char2bysx(*line);

      if (val2 < 0)
      {
        fprintf(stderr, "Bad pixel2 `%c' found.\n", *line);
        exit(36);
      }

      val = val * 64 + val2;
    }

    line++;

    Pix_t pix = Pix_t(val);

    ASSERT(pix < TL_MAX_COLS);

    PixelBuf[pix_h][x] = parse_map[pix];
  }

  pix_h++;

  if (pix_h > tile_h)
    tile_h = pix_h;
}

void Tileset::HandleLine(char *line)
{
  char ch = line[0];
  int len = strlen(line);

  while (len > 0 && isspace(line[len-1]))
  {
    line[--len] = 0;
  }
   
  if (len == 0)
    return;

  if (ch == '#')
  {
    DoTag(line);
  }
  else if (ch == '{')
  {
    StartTile();
  }
  else if (ch == '}')
  {
    EndTile();
  }
  else if (ch == ' ')
  {
    TileRow(line);
  }
  else if (char2bysx(ch) >= 0)
  {
    DoColor(line);
  }
  else
  {
    fprintf(stderr, "Unrecognised line:\n`%s'\n", line);
    exit(38);
  }
}


/* ---------------------------------------------------------------- */

Tileset * Tileset::Create(int width, int height,
        ColorMapping *colmap)
{
  Tileset *set = new Tileset();

  set->cmap = colmap;

  if (colmap->num_cols > 62)
    set->color_size = 2;
  else
    set->color_size = 1;

  set->tile_w = width;
  set->tile_h = height;

  set->no_names = 1;

  return set;
}

int Tileset::AddTile(Pix_t fill_pix, int t_width, int t_height)
{
  int idx = total;

  total++;
  ASSERT(total <= TL_MAX_TILES);

  if (t_width < 0)
    t_width = tile_w;
    
  if (t_height < 0)
    t_height = tile_h;
   
  Tile *tile = tiles + idx;

  tile->Create(t_width, t_height, idx);

  if (fill_pix != NO_PIX)
    tile->Clear(fill_pix);

  return idx;
}

int Tileset::AddTile(uint8_g *rgba, int depth, int stride,
          int t_width, int t_height)
{
  int idx = AddTile(NO_PIX, t_width, t_height);

  Tile *tile = tiles + idx;

  ASSERT(depth == 3 || depth == 4);

  if (depth == 3)
    tile->StoreRGB(cmap, rgba, stride, tile->width, tile->height);
  else
    tile->StoreRGBA(cmap, rgba, stride, tile->width, tile->height);

  return idx;
}
 
Tileset * Tileset::Load(const Filename& fname)
{
  Tileset *set = new Tileset();

  set->cmap = new ColorMapping(true);

  FILE *fp = Z_File::Open(fname, "r");

  if (! fp)
  {
    fprintf(stderr, "Unable to open: %s\n", fname.str.c_str());
    exit(21);
  }

  fprintf(stderr, "Loading: %s\n", fname.str.c_str());

  set->parse_cols = new PackCol [TL_MAX_COLS];
  set->parse_map  = new Pix_t   [TL_MAX_COLS];

  for (int i=0; i < TL_MAX_COLS; i++)
    set->parse_map[i] = 1;

  for (;;)
  {
    char LineBuf[MAX_LINE];

    if (! fgets(LineBuf, MAX_LINE-2, fp))
      break;

    set->HandleLine(LineBuf);
  }

  Z_File::Close(fp);

  delete[] set->parse_cols;
  set->parse_cols = 0;

  delete[] set->parse_map;
  set->parse_map = 0;

  return set;
}

void Tileset::ImportTile(int dst, const Tileset *src_set, int src)
{
  ASSERT(0 <= dst && dst < total);
  ASSERT(0 <= src && src < src_set->total);

  Tile *D = tiles + dst;
  const Tile *S = src_set->tiles + src;

  D->Clear(cmap->dark_col);

  for (int y=0; y < D->height; y++)
  for (int x=0; x < D->width;  x++)
  {
    int sx = x * S->width  / D->width;
    int sy = y * S->height / D->height;

    Pix_t cur = S->pixels[sy * S->width + sx];
    Pix_t& dp = D->pixels[y * D->width + x];

    ASSERT(cur != NO_PIX);

    if (cmap == src_set->cmap)
    {
      dp = cur;
    }
    else if (cur == src_set->cmap->trans_col && 
             cmap->trans_col != NO_PIX)
    {
      dp = cmap->trans_col;
    }
    else
    {
      const PackCol& col = src_set->cmap->At(cur);

      // compute new color without any alpha part
      PackCol want_col(int(col.r), int(col.g), int(col.b));
      
      dp = cmap->LookupClosest(want_col);
    }
  }
}


/* ---------------------------------------------------------------- */

/*
 * returns tile number that matches name, otherwise -1.  The given
 * name should already have been alias converted.
 */
int Tileset::LookupName(const char *name) const
{
  int hash;
  int index;

  if (! name)
    return -1;

  if (no_names)
    return -1;
  
  hash = T_Alias::Hash(string(name));

  index = hash_tab[hash];

  for (; index >= 0; index = tiles[index].hash_next)
  {
    ASSERT(index < total);
    
    // FIXME: this is not very clean, as tiles who names are set to
    //        NULL (or changed) should update the hash table.
    if (! tiles[index].name)
      continue;

    if (T_Alias::FuzzyCmp(string(tiles[index].name), string(name)) == 0)
    {
      return index;
    }
  }  

  /* not found */
  return -1;
}

bool Tileset::ConvertTrans(const PackCol& col)
{
  Pix_t src = cmap->LookupExact(col);

  if (src == NO_PIX || cmap->trans_col == NO_PIX)
    return false;

  for (int t=0; t < total; t++)
  {
    tiles[t].Replace(src, cmap->trans_col);
  }

  return true;
}


//----------------------------------------------------------------------------

/* !!! FIXME: move to Colormapping class */
void Tileset::FindUsedPairs()
{
  int maxp = (color_size == 1) ? 62 : 4096;

  used_pairs = new byte [maxp];

  memset(used_pairs, 0, maxp * sizeof(byte));
 
  for (Pix_t i=0; i < cmap->num_cols; i++)
  {
    const char *pair = cmap->m_arr[i].pair;

    if (! pair[0])
      continue;

    int val = char2bysx(pair[0]);

    if (pair[1])
      val = val * 64 + char2bysx(pair[1]);
    
    ASSERT(0 <= val && val < maxp);

    used_pairs[val] = 1;
  }
}

void Tileset::AllocatePair(char *pair)
{
  int i;
  int maxp = (color_size == 1) ? 62 : 4096;

  for (i=0; i < maxp; i++)
    if (! used_pairs[i])
      break;

  if (i >= maxp)
  {
    fprintf(stderr, "AllocatePair: none left !!\n");
    exit(79);
  }

  used_pairs[i] = 1;

  if (color_size == 1)
  {
    pair[0] = bysx2char(i+1);
    pair[1] = 0;
  }
  else
  {
    pair[0] = bysx2char(i / 64);
    pair[1] = bysx2char(i % 64);
    pair[2] = 0;
  }
}

/* !!! FIXME: move to Colormapping class */
Pix_t Tileset::FindPair(const char *pair) const
{
  Pix_t i;

  for (i=0; i < cmap->num_cols; i++)
  {
    const char *cmap_pair = cmap->m_arr[i].pair;

    if (memcmp((const void *) cmap_pair, (const void *) pair,
          color_size) == 0)
      return i;
  }

  return NO_PIX;
}
 
//?? void Tileset::SwapPair(Pix_t src, Pix_t dest)
//?? {
//??   ASSERT(src != NO_PIX);
//??   ASSERT(dest != NO_PIX);
//?? 
//??   if (src == dest)
//??     return;
//?? 
//??   char tmp[2];
//?? 
//??   tmp[0] = color_infos[src].pair[0];
//??   tmp[1] = color_infos[src].pair[1];
//?? 
//??   color_infos[src].pair[0] = color_infos[dest].pair[0];
//??   color_infos[src].pair[1] = color_infos[dest].pair[1];
//??   
//??   color_infos[dest].pair[0] = tmp[0];
//??   color_infos[dest].pair[1] = tmp[1];
//?? }

/// OLD OLD OLD OLD OLD VERSION
/// bool Tileset::SwapPair(const char *src, const char *dest)
/// {
///   Pix_t sp = FindPair(src);
///   Pix_t dp = FindPair(dest);
/// 
///   if (sp == NO_PIX || dp == NO_PIX)
///     return false;
/// 
///   if (sp == dp)
///     return true;
/// 
///   char tmp[2];
/// 
///   tmp[0] = color_infos[sp].pair[0];
///   tmp[1] = color_infos[sp].pair[1];
/// 
///   color_infos[sp].pair[0] = color_infos[dp].pair[0];
///   color_infos[sp].pair[1] = color_infos[dp].pair[1];
///   
///   color_infos[dp].pair[0] = tmp[0];
///   color_infos[dp].pair[1] = tmp[1];
///   
///   return true;
/// }

//?? void Tileset::ImportPairNames(const Tileset *src_set)
//?? {
//??   Pix_t i;
//?? 
//??   ASSERT(src_set->color_size == color_size);
//?? 
//??   for (i = 0; i < cmap->num_cols; i++)
//??   {
//??     const PackCol& col = cmap->At(i);
//?? 
//??     TileColInfo& info = color_infos[i];
//?? 
//??     Pix_t same_col_p = src_set->cmap->LookupExact(col);
//?? 
//??     if (same_col_p == NO_PIX)
//??     {
//??       fprintf(stderr, "ImportPair color not found (%d,%d,%d)\n",
//??           col.r, col.g, col.b);
//??       continue;
//??     }
//?? 
//??     const TileColInfo& src_info = src_set->color_infos[same_col_p];
//?? 
//??     Pix_t same_name_p = FindPair(src_info.pair);
//?? 
//?? #define XC(c)  (c ? c : ' ')
//?? 
//?? fprintf(stderr, "Index %d [%c%c]  SAME COL %d [%c%c]  SAME NAME %d\n",
//?? i, XC(info.pair[0]), XC(info.pair[1]),
//?? same_col_p, XC(src_info.pair[0]), XC(src_info.pair[1]),
//?? (same_name_p == NO_PIX) ? -1 : same_name_p);
//??     
//??     if (same_name_p == NO_PIX || same_name_p >= i)
//??     {
//??       /* Ah good, no clashes with this name */
//?? 
//??       info.pair[0] = src_info.pair[0];
//??       info.pair[1] = src_info.pair[1];
//??     }
//??     else
//??     {
//??       fprintf(stderr, "Warning: trouble importing name %c%c\n",
//??           src_info.pair[0],
//??           src_info.pair[1] ? src_info.pair[1] : ' ');
//?? 
//??       SwapPair(same_name_p, i);
//??     }
//??   }
//?? 
//?? #if 0  // OLD (BROKEN) IMPLEMENTATION
//??   Pix_t i;
//?? 
//??   ASSERT(src_set->color_size == color_size);
//?? 
//??   for (i = 0; i < src_set->cmap->num_cols; i++)
//??   {
//??     const PackCol& col = src_set->cmap->At(i);
//??     const TileColInfo& info = src_set->color_infos[i];
//?? 
//??     Pix_t same_col_p = cmap->LookupExact(col);
//?? 
//??     if (same_col_p == NO_PIX)
//??     {
//??       fprintf(stderr, "ImportPair color not found (%d,%d,%d)\n",
//??           col.r, col.g, col.b);
//??       continue;
//??     }
//?? 
//??     Pix_t same_name_p = FindPair(info.pair);
//?? 
//??     if (same_name_p == NO_PIX)
//??     {
//??       fprintf(stderr, "ImportPair name not found: %c%c\n",
//??           info.pair[0], info.pair[1] ? info.pair[1] : ' ');
//??       continue;
//??     }
//?? 
//??     SwapPair(same_name_p, same_col_p);
//??   }
//?? #endif
//?? }


/* ---------------------------------------------------------------- */

void Tileset::WriteColors(FILE *fp)
{
  ASSERT(cmap->num_cols > 0);

  /* first, make sure all colors have names */

  Pix_t i;

  /* !!! FIXME: put into colormap code */
  for (i=0; i < cmap->num_cols; i++)
  {
    char *pair = cmap->m_arr[i].pair;

    if (! pair[0])
      AllocatePair(pair);
  }

#if 0  //!!! FIXME: new routine SortByPair in colormap code
  /* second, create table sorted by the `pair' */

  TileColInfo *infomap = new TileColInfo [cmap->num_cols];

  for (i=0; i < cmap->num_cols; i++)
  {
    infomap[i].pos = i;
    infomap[i].pair[0] = color_infos[i].pair[0];
    infomap[i].pair[1] = color_infos[i].pair[1];
  }
   
  qsort(infomap, cmap->num_cols, sizeof(TileColInfo), & InfoComparer);
#endif

  /* finally, write out colors */

/* !!! FIXME: new routine `WriteOneColor' in Colormapping class */
  for (i=0; i < cmap->num_cols; i++)
  {
    const M_Color& info = cmap->m_arr[i];

///---    Pix_t j = infomap[i].pos;
///---
///---    const PackCol& col = cmap->At(j);
///---
///---    TileColInfo& info = color_infos[j];

    fputc(info.pair[0], fp);

    if (info.pair[1])
      fputc(info.pair[1], fp);

    fprintf(fp, " = (%d, %d, %d)\n", 
        int(info.c.r), int(info.c.g), int(info.c.b));
  }

///---  delete[] infomap;

  fflush(fp);
}

void Tileset::WriteTile(FILE *fp, Tile *tile, int id)
{
  ASSERT(tile);
  ASSERT(tile->pixels);

  // handle the (rare) case of tiles without ID and/or NAMES

  if (tile->tile_id >= 0)
    id = tile->tile_id;

  const char *name = "unknown";

  if (tile->name)
    name = tile->name;
  
  fprintf(fp, "# tile %d (%s)\n", id, name);
  fprintf(fp, "{\n");

  int dx, dy;

  for (dy=0; dy < tile->height; dy++)
  {
    fprintf(fp, "  ");

    for (dx=0; dx < tile->width;  dx++)
    {
      Pix_t pix = tile->pixels[dy * tile->width + dx];

      ASSERT(pix != NO_PIX);
      ASSERT(pix < cmap->num_cols);

      const char *pair = cmap->m_arr[pix].pair;

      ASSERT(pair[0]);

      fputc(pair[0], fp);

      if (pair[1])
        fputc(pair[1], fp);
    }

    fprintf(fp, "\n");
  }
 
  fprintf(fp, "}\n");
  fflush(fp);
}

void Tileset::Save(FILE *fp)
{
  FindUsedPairs();
  
  WriteColors(fp);

  for (int t=0; t < total; t++)
  {
    if (skip_empties && ! tiles[t].name)
      continue;

    WriteTile(fp, tiles + t, t);
  }

  delete[] used_pairs;
  used_pairs = 0;
}

void Tileset::Save(const Filename& fname)
{
  FILE *fp = Z_File::Open(fname, "w");

  if (! fp)
  {
    fprintf(stderr, "Unable to create: %s\n", fname.str.c_str());
    exit(81);
  }

  fprintf(stderr, "Saving: %s\n", fname.str.c_str());

  Save(fp);
 
  Z_File::Close(fp);
}

///--- int Tileset::InfoComparer(const void *A, const void *B)
///--- {
///---   const TileColInfo *C = (const TileColInfo *) A;
///---   const TileColInfo *D = (const TileColInfo *) B;
///--- 
///---   int E = char2bysx(C->pair[0]);
///---   int F = char2bysx(D->pair[0]);
///--- 
///---   if (C->pair[1])
///---     E = E * 64 + char2bysx(C->pair[1]);
///--- 
///---   if (D->pair[1])
///---     F = F * 64 + char2bysx(D->pair[1]);
///--- 
///---   return E - F;
///--- }

void Tileset::ChangeColormap(ColorMapping *new_map)
{
  ASSERT(new_map);

  if (cmap == new_map)
    return;
  
  Pix_t *conv_mapping = cmap->ConversionMap(new_map);

  /* FIXME: check for inexact matches, and warn about 'em.
   */
  
  for (int index = 0; index < total; index++)
  {
    Tile *T = tiles + index;

    T->ApplyConversion(conv_mapping, cmap->num_cols);
  }

  delete[] conv_mapping;

  cmap = new_map;

  /* !!! FIXME: should delete or return the original cmap */
}

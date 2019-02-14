/*
 * Q2 STRUCTS
 *
 * Based on "qfiles.h" from the GPL'd quake 2 source release.
 * Copyright (C) 1997-2001 Id Software, Inc.
 */

#ifndef __Q2_STRUCTS_H__
#define __Q2_STRUCTS_H__


/* 
 * Pack files
 */

#define PACK_IDENT  "PACK"

typedef struct
{
  char ident[4];

  Sint32 dir_offset;
  Sint32 dir_size;
}
pack_header_t;

typedef struct
{
  char filename[56];

  Sint32 offset;
  Sint32 size;
}
pack_entry_t;


/* 
 * MD2 model format
 */

#define MD2_IDENT  "IDP2"

typedef struct
{
  char ident[4];
  Sint32 version;

  Sint32 skin_width;
  Sint32 skin_height;
  Sint32 frame_size;

  Sint32 num_skins;
  Sint32 num_xyz;
  Sint32 num_st;
  Sint32 num_tris;
  Sint32 num_glcmds;
  Sint32 num_frames;

  Sint32 ofs_skins;
  Sint32 ofs_st;
  Sint32 ofs_tris;
  Sint32 ofs_frames;
  Sint32 ofs_glcmds;  
  Sint32 ofs_end;
} 
md2_header_t;

typedef struct
{
  Uint16 s, t;
} 
md2_texcoord_t;

typedef struct 
{
  Uint16 index_xyz[3];
  Uint16 index_st[3];
} 
md2_triangle_t;

typedef struct
{
  Uint8 v[3];
  Uint8 light_normal;
} 
md2_vertex_t;

typedef struct
{
  float s, t;
  Sint32 vert_index;
}
md2_gl_vertex_t;

typedef struct
{
  float scale[3];
  float translate[3];

  char name[16];

  md2_vertex_t verts[1];  /* variable sized */
} 
md2_frame_t;

typedef struct
{
  char name[64];
}
md2_skin_t;


/* 
 * PCX image structure
 */

typedef struct
{
  Uint8 manufacturer;
  Uint8 version;
  Uint8 encoding;
  Uint8 bits_per_pixel;

  Uint16 xmin, ymin, xmax, ymax;
  Uint16 hres, vres;

  Uint8 palette[48];

  Uint8 reserved;
  Uint8 color_planes;

  Uint16 bytes_per_line;
  Uint16 palette_type;

  Uint8 filler[58];
  Uint8 data[1];      /* variable sized */
} 
pcx_header_t;


#endif /* __Q2_STRUCTS_H__ */

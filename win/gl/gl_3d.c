/* Copyright (C) 2002 Andrew Apted <ajapted@users.sourceforge.net> */
/* NetHack may be freely redistributed.  See license for details.  */

/*
 * SDL/GL window port for NetHack & Slash'EM.
 *
 * Real 3D renderer.
 *
 * Pretty "Alpha" stuff.  Don't look Ethel !
 */

#include "hack.h"
#include "patchlevel.h"
#include "lev.h"

#if defined(GL3D_GRAPHICS)

#define WINGL_INTERNAL
#include "winGL.h"

#include "q2_structs.h"

#undef yn
#include <math.h>
#include <ctype.h>


#ifndef M_SQRT2
#define M_SQRT2  1.41421356237309504880
#endif

#ifndef M_SQRT3
#define M_SQRT3  1.73205080756887729352
#endif


#define MAX_OBJ_TILE  828   /* FIXME: define elsewhere */


static float player_z = 0;
static int player_z_last_ms = -1;


extern light_source *light_base;

#define LSF_NEEDS_FIXUP 0x2


struct LightInfo
{
  /* light name, used for lookups and debugging */
  const char *name;

  float ambient[4];
  float diffuse[4];
  float specular[4];
  float attenuation[3];

  const char *flags;
};

struct MaterialInfo
{
  /* material name, for lookups and debugging */
  const char *name;

  float ambient[4];
  float diffuse[4];
  float specular[4];
  float emission[4];
  float shininess;

  const char *flags;
};

struct ModelSpec
{
  const char *model;
  struct Model3D *loaded_model;
  
  const char *skin;
  GLuint loaded_skin;

  struct MaterialInfo *material;
  struct LightInfo *light;

  float scale;
  float rotate;

  const char *flags;
};

#define MAX_LIGHT_INFO     128
#define MAX_MATERIAL_INFO  128
#define MAX_MODEL_SPECS    2000

static struct LightInfo light_infos[MAX_LIGHT_INFO];
static int num_light_infos = 0;

static struct MaterialInfo material_infos[MAX_MATERIAL_INFO];
static int num_material_infos = 0;

static struct ModelSpec model_specs[MAX_MODEL_SPECS] = { { NULL, }, };
static int num_model_specs = 0;

static struct ModelSpec pet_mark_spec = 
{
  "petmark.md2", NULL, "petmark.png", 0, 
  NULL, NULL, 0.25, 0, "-"
};

static struct ModelSpec ridden_mark_spec =
{
  "petmark.md2", NULL, "ridemark.png", 0, 
  NULL, NULL, 0.25, 0, "-"
};


struct ModelVertex
{
  short x, y, z;
  
  short normal_idx;
};

struct ModelGlVert
{
  short vert_idx;

  float s, t;
};

struct Model3D
{
  /* only one frame supported now */
  struct
  {
    struct ModelVertex *verts;  
    int num_verts;

    float scale[3];
    float translate[3];
  }
  frame;

  struct ModelGlVert *glverts;
  int num_glverts;

  int *glcmds;
  int num_glcmds;

  struct MaterialInfo *material;
};


#define NUM_VERTEX_NORMALS  162

static float md2_normals[NUM_VERTEX_NORMALS][3] = {
#include "q2_norms.h"
};


#define TO_GL_MID_X(map_x)  ((map_x) * 128.0 + 64.0)
#define TO_GL_MID_Y(map_y)  ((22-1-(map_y)) * 128.0 + 64.0)
 

/* ---------------------------------------------------------------- */

#define MAX_LINE_BUF   2048
#define MAX_TOKEN_BUF  64

#define WHITE_SPACE  " \t\r\n"

struct FileKeyword
{
  char buf[MAX_TOKEN_BUF];
};


static void load_keyworded_file(const char *filename,
    int required_words, 
    int (* handle_func)(struct FileKeyword *words, int num_words))
{
  FILE *fp = fopen_datafile(filename, "r", FALSE);

  int line_num = 0;

  if (! fp)
  {
    sdlgl_error("Failed to open %s !\n", filename);
    return;  /* NOTREACHED */
  }

  for (;;)
  {
    char line_buf[MAX_LINE_BUF];
    char *keyword;

    struct FileKeyword word_list[16];

    int num_words = 0;
      
    if (! fgets(line_buf, MAX_LINE_BUF-2, fp))
      break;

    line_num++;

    for (keyword = line_buf; *keyword && isspace(*keyword); keyword++)
    { /* nothing here */ }

    /* Ignore blank lines and comments */

    if (keyword[0] == 0 || keyword[0] == '/' || keyword[0] == '#')
      continue;
        
    for (keyword = strtok(keyword, WHITE_SPACE);
         keyword != NULL;
         keyword = strtok(NULL, WHITE_SPACE))
    {
      if (strlen(keyword) > MAX_TOKEN_BUF-2)
      {
        sdlgl_warning("Keyword too long ! ('%s')\n", keyword);
        sdlgl_error("Occurred at line %d of %s\n", line_num, filename);
        return;  /* NOT REACHED */
      }

      if (num_words >= SIZE(word_list))
      {
        sdlgl_warning("Too many keywords !\n");
        sdlgl_error("Occurred at line %d of %s\n", line_num, filename);
        return;  /* NOT REACHED */
      }
      
      strcpy(word_list[num_words].buf, keyword);
      num_words++;
    }

    if (num_words != required_words)
    {
      sdlgl_warning("Wrong number of keywords (got %d, wanted %d)\n",
          num_words, required_words);
      sdlgl_error("Occurred at line %d of %s\n", line_num, filename);
      return;  /* NOT REACHED */
    }

    if (0 != (* handle_func)(word_list, num_words))
    {
      sdlgl_error("Occurred at line %d of %s\n", line_num, filename);
      return;  /* NOT REACHED */
    }
  }
}

static int parse_color(char *buf, float *quad, const char *name)
{
  quad[3] = 1.0;
 
  if (buf[0] == '(')
  {
    if (sscanf(buf, " ( %f , %f , %f ) ", quad+0, quad+1, quad+2) != 3)
    {
      sdlgl_warning("Bad VECTOR color entry `%s' for %s\n", buf, name);
      return -1;
    }
    
    return 0;  /* OK */
  }
  
  if (sscanf(buf, " %f ", quad+0) != 1)
  {
    sdlgl_warning("Bad color entry `%s' for %s\n", buf, name);
    return -1;
  }

  quad[1] = quad[0];
  quad[2] = quad[1];

  return 0;  /* OK */
}
    
static int parse_light(struct FileKeyword *words, int num_words)
{
  struct LightInfo *lit = light_infos + num_light_infos;
  
  num_light_infos++;

  if (num_light_infos >= MAX_LIGHT_INFO)
  {
    sdlgl_error("Too many light definitions !\n");
    return -1;  /* NOT REACHED */
  }

  lit->name  = strdup(words[0].buf);
  lit->flags = strdup(words[7].buf);

  if (parse_color(words[1].buf, lit->ambient, "light") != 0 ||
      parse_color(words[2].buf, lit->diffuse, "light") != 0 ||
      parse_color(words[3].buf, lit->specular, "light") != 0)
  {
    return -1;
  }
  
  lit->attenuation[0] = atof(words[4].buf);
  lit->attenuation[1] = atof(words[5].buf);
  lit->attenuation[2] = atof(words[6].buf);

  return 0;  /* OK */
}

static int parse_material(struct FileKeyword *words, int num_words)
{
  struct MaterialInfo *mat = material_infos + num_material_infos;
  
  num_material_infos++;

  if (num_material_infos >= MAX_MATERIAL_INFO)
  {
    sdlgl_error("Too many material definitions !\n");
    return -1;  /* NOT REACHED */
  }

  mat->name  = strdup(words[0].buf);
  mat->flags = strdup(words[6].buf);

  if (parse_color(words[1].buf, mat->ambient, "material") != 0 ||
      parse_color(words[2].buf, mat->diffuse, "material") != 0 ||
      parse_color(words[3].buf, mat->specular, "material") != 0 ||
      parse_color(words[4].buf, mat->emission, "material") != 0)
  {
    return -1;
  }
  
  mat->shininess = atof(words[5].buf);

  return 0;  /* OK */
}

static struct LightInfo *lookup_light(const char *name)
{
  int i;

  for (i=0; i < num_light_infos; i++)
    if (strcmpi(light_infos[i].name, name) == 0)
      return light_infos + i;

  return NULL;
}
 
static struct MaterialInfo *lookup_material(const char *name)
{
  int i;

  for (i=0; i < num_material_infos; i++)
    if (strcmpi(material_infos[i].name, name) == 0)
      return material_infos + i;

  return NULL;
}
 
static int parse_model_spec(struct FileKeyword *words, int num_words)
{
  int tile = num_model_specs;

  struct ModelSpec *mod = model_specs + num_model_specs;
  
  num_model_specs++;

  if (num_model_specs >= MAX_MODEL_SPECS)
  {
    sdlgl_error("Too many model specs !\n");
    return -1;  /* NOT REACHED */
  }

  mod->model = strdup(words[0].buf);
  mod->skin  = strdup(words[1].buf);
  mod->flags = strdup(words[6].buf);

  mod->loaded_model = NULL;
  mod->loaded_skin  = 0;

  /* handle material */
  {
    if (words[2].buf[0] == '-')
    {
      if (tile < NUM_MON_TILES)
        strcpy(words[2].buf, "MonsterDefault");
      else if (tile <= MAX_OBJ_TILE)
        strcpy(words[2].buf, "ObjectDefault");
      else
        strcpy(words[2].buf, "DungeonDefault");
    }

    mod->material = lookup_material(words[2].buf);

    if (! mod->material)
    {
      sdlgl_warning("Material `%s' not found !\n", words[2].buf);
      return -1;
    }
  }
  
  /* handle light */
  {
    if (words[3].buf[0] == '-')
    {
      mod->light = NULL;
    }
    else
    {
      mod->light = lookup_light(words[3].buf);

      if (! mod->light)
      {
        sdlgl_warning("Light `%s' not found !\n", words[3].buf);
        return -1;
      }
    }
  }

  mod->scale  = atof(words[4].buf);
  mod->rotate = atof(words[5].buf);

  if (mod->scale <= 0)
  {
    sdlgl_warning("Bad scale `%1.3f' for model !\n", mod->scale);
    return -1;
  }

  return 0;  /* OK */
}


/* ---------------------------------------------------------------- */

struct Camera3D
{
  float fov;
  float viewx, viewy, viewz;
  float horiz_angle, vert_angle;
  float hf_width, hf_height;
  float near, far;

  /* computed values */
  
  float h_sin, h_cos;  /* from horiz_angle */
  float v_sin, v_cos;  /* from vert_angle */
  
  float h_nx, h_ny;    /* normal, from hf_width */
  float v_nx, v_nz;    /* normal, from hf_height */
};

static struct Camera3D camera = { 78, 0, };
 

static void load_model_vertices(FILE *fp, md2_header_t *header,
    struct Model3D *model)
{
  int v;

  md2_frame_t *frame_buf;


  if (fseek(fp, header->ofs_frames, SEEK_SET) != 0)
  {
    sdlgl_error("Error finding frames in model file.\n");
    return;  /* NOT REACHED */
  }

  frame_buf = (md2_frame_t *) alloc(header->frame_size);
    
  /* only the first frame is used */

  if (fread(frame_buf, header->frame_size, 1, fp) != 1)
  {
    sdlgl_error("Error reading frames in model file.\n");
    return;  /* NOT REACHED */
  }

  model->frame.scale[0] = frame_buf->scale[0];
  model->frame.scale[1] = frame_buf->scale[1];
  model->frame.scale[2] = frame_buf->scale[2];

  model->frame.translate[0] = frame_buf->translate[0];
  model->frame.translate[1] = frame_buf->translate[1];
  model->frame.translate[2] = frame_buf->translate[2];

  for (v=0; v < model->frame.num_verts; v++)
  {
    struct ModelVertex *dest = model->frame.verts + v;

    md2_vertex_t *src = (& frame_buf->verts[0]) + v;

    dest->x = src->v[0];
    dest->y = src->v[1];
    dest->z = src->v[2];

    dest->normal_idx = src->light_normal;
  }

  free(frame_buf);
}

static void load_model_glcommands(FILE *fp, md2_header_t *header,
    struct Model3D *model)
{
  Uint32 *cmd_buf;
  Uint32 *cmd_ptr;
  
  if (fseek(fp, header->ofs_glcmds, SEEK_SET) != 0)
  {
    sdlgl_error("Error finding glcmds in model file.\n");
    return;  /* NOT REACHED */
  }
  
  cmd_buf = (Uint32 *) alloc(header->num_glcmds * sizeof(Uint32));

  if (fread(cmd_buf, header->num_glcmds * sizeof(Uint32), 1, fp) != 1)
  {
    sdlgl_error("Error reading glcmds in model file.\n");
    return;  /* NOT REACHED */
  }

  for (cmd_ptr = cmd_buf; *cmd_ptr != 0; )
  {
    int count = *cmd_ptr++;

    md2_gl_vertex_t *src = (md2_gl_vertex_t *) cmd_ptr;

    /* sanity check */
    if (abs(count) > header->num_glcmds)
    {
      sdlgl_error("Corrupted gl commands (count=%d) in model file.\n",
          count);
      return;  /* NOT REACHED */
    }
    
    model->glcmds[model->num_glcmds++] = count;
    
    for (count = abs(count); count > 0; 
         count--, src++, model->num_glverts++)
    {
      struct ModelGlVert *dest = model->glverts + model->num_glverts;

      dest->vert_idx = src->vert_index;
      dest->s = src->s;
      dest->t = src->t;
    }
    
    cmd_ptr = (Uint32 *) src;
  }
}

static struct Model3D *load_md2_model(const char *filename)
{
  md2_header_t header;

  struct Model3D *model;
  
  FILE *fp;

  
  fp = fopen_datafile(filename, "rb", FALSE);

  if (! fp)
  {
    sdlgl_error("Unable to open model file: %s\n", filename);
    return NULL;  /* NOT REACHED */
  }

  if (fread(&header, sizeof(header), 1, fp) != 1)
  {
    sdlgl_error("Error reading model file: %s\n", filename);
    return NULL;  /* NOT REACHED */
  }

  if (strncmp(header.ident, MD2_IDENT, 4) != 0)
  {
    sdlgl_error("Bad header (%c%c%c%c) in model file: %s\n", 
        header.ident[0], header.ident[1], header.ident[2], header.ident[3],
        filename);
    return NULL;  /* NOT REACHED */
  }

  if (header.version != 8)
  {
    sdlgl_error("Bad version (%d) in model file: %s\n", 
        header.version, filename);
    return NULL;  /* NOT REACHED */
  }

  model = (struct Model3D *) alloc(sizeof(struct Model3D));
  memset(model, 0, sizeof(struct Model3D));

  model->frame.num_verts = header.num_xyz;
  model->frame.verts = (struct ModelVertex *) alloc(
      model->frame.num_verts * sizeof(struct ModelVertex));

  /* header.num_glcmds is an upper bound of the number of actual
   * glvertices and glcommands that are needed.  This ain't optimal,
   * but it'll suffice.
   */
  model->glverts = (struct ModelGlVert *) alloc(
      header.num_glcmds * sizeof(struct ModelGlVert));

  model->glcmds = (int *) alloc(header.num_glcmds * sizeof(int));

  load_model_vertices(fp, &header, model);
  load_model_glcommands(fp, &header, model);

  return model;
}

static GH_INLINE int check_flag(int tile, char flag)
{
  if (tile == TILE_EMPTY)
    return 0;

  return strchr(model_specs[tile].flags, flag) != NULL;
}

#define WSIDE_LEFT    0x01
#define WSIDE_RIGHT   0x02
#define WSIDE_BOTTOM  0x04
#define WSIDE_TOP     0x08

static int rotate_sides(int sides, float rotate)
{
  if (rotate < 0)
    rotate += 360.0;

  if (rotate <= 2 || rotate >= 358.0)
    return sides;

  if (rotate >= 88.0 && rotate <= 92.0)
  {
    return ((sides & WSIDE_LEFT)   ? WSIDE_BOTTOM : 0) |
           ((sides & WSIDE_BOTTOM) ? WSIDE_RIGHT  : 0) |
           ((sides & WSIDE_RIGHT)  ? WSIDE_TOP    : 0) |
           ((sides & WSIDE_TOP)    ? WSIDE_LEFT   : 0);
  }
  
  if (rotate >= 178.0 && rotate <= 182.0)
  {
    return ((sides & WSIDE_LEFT)   ? WSIDE_RIGHT  : 0) |
           ((sides & WSIDE_BOTTOM) ? WSIDE_TOP    : 0) |
           ((sides & WSIDE_RIGHT)  ? WSIDE_LEFT   : 0) |
           ((sides & WSIDE_TOP)    ? WSIDE_BOTTOM : 0);
  }

  if (rotate >= 268.0 && rotate <= 272.0)
  {
    return ((sides & WSIDE_LEFT)   ? WSIDE_TOP    : 0) |
           ((sides & WSIDE_BOTTOM) ? WSIDE_LEFT   : 0) |
           ((sides & WSIDE_RIGHT)  ? WSIDE_BOTTOM : 0) |
           ((sides & WSIDE_TOP)    ? WSIDE_RIGHT  : 0);
  }

  /* unsupported rotation -- never hide any sides */
  return 0;
}

static void render_md2_model(struct Model3D *model, GLuint skin,
    struct MaterialInfo *mat, float bx, float by, float bz,
    float scale, float rotate, int hidden_sides)
{
  int c;
  int gv;
 
  /* FIXME: this doesn't seem to work fully */
  if (hidden_sides)
    hidden_sides = rotate_sides(hidden_sides, rotate);
 
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  glTranslatef(bx, by, bz);

  glRotatef(rotate, 0.0, 0.0, 1.0);
 
  glScalef(scale, scale, scale);

  glTranslatef(model->frame.translate[0], model->frame.translate[1],
               model->frame.translate[2]);

  glScalef(model->frame.scale[0], model->frame.scale[1],
           model->frame.scale[2]);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, skin);

  glMaterialfv(GL_FRONT, GL_AMBIENT,  mat->ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,  mat->diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat->specular);
  glMaterialfv(GL_FRONT, GL_EMISSION, mat->emission);

  glMaterialf(GL_FRONT, GL_SHININESS, mat->shininess);
  
  for (c=0, gv=0; c < model->num_glcmds; c++)
  {
    int count = model->glcmds[c];
    int skip = 0;

    /* hidden side removal (for walls) */
    if (hidden_sides != 0)
    {
      struct ModelGlVert *glvert = model->glverts + gv;
      struct ModelVertex *vert = model->frame.verts + glvert->vert_idx;
      
      float *normal = & md2_normals[vert->normal_idx][0];

      if ( ((hidden_sides & WSIDE_LEFT)   && normal[0] < -0.97) ||
           ((hidden_sides & WSIDE_RIGHT)  && normal[0] > +0.97) ||
           ((hidden_sides & WSIDE_TOP)    && normal[1] > +0.97) ||
           ((hidden_sides & WSIDE_BOTTOM) && normal[1] < -0.97))
      {
        skip = 1;
      }
    }

    if (skip)
    {
      gv += abs(count);
      continue;
    }

    glBegin(count > 0 ? GL_TRIANGLE_STRIP : GL_TRIANGLE_FAN);

    for (count = abs(count); count > 0; count--, gv++)
    {
      struct ModelGlVert *glvert = model->glverts + gv;
      struct ModelVertex *vert = model->frame.verts + glvert->vert_idx;

      float *normal = & md2_normals[vert->normal_idx][0];

      glTexCoord2f(glvert->s, glvert->t);
      glNormal3f(normal[0], normal[1], normal[2]);
      glVertex3s(vert->x, vert->y, vert->z);
    }
   
    glEnd();
  }
  
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

static void begin_3d_rendering(struct TileWindow *win)
{
  int mousex, mousey;
  float mx, my;

  glViewport(win->scr_x, win->scr_y, win->scr_w, win->scr_h);

  SDL_GetMouseState(&mousex, &mousey);

  mousex = max(0, min(sdlgl_width,  mousex));
  mousey = max(0, min(sdlgl_height, mousey));
   
  mx = 2.0 * mousex / (float) sdlgl_width - 1.0;
  my = 1.0 - mousey / (float) sdlgl_height;
  
  /* compute camera info */
  {
    camera.vert_angle  = my * -64.0;
    camera.horiz_angle = 90 - mx * 135.0;

    camera.hf_height = win->scr_h / (float) win->scr_w;
    camera.hf_width  = tan((camera.fov / 2.0) * M_PI / 180.0);

    camera.near = 1;
    camera.far  = 8000;

    camera.h_sin = sin(camera.horiz_angle * M_PI / 180.0);
    camera.h_cos = cos(camera.horiz_angle * M_PI / 180.0);

    camera.v_sin = sin(camera.vert_angle * M_PI / 180.0);
    camera.v_cos = cos(camera.vert_angle * M_PI / 180.0);
  }
  
  /* compute camera normals */
  {
    float h_norm_len = sqrt(1.0 + camera.hf_width  * camera.hf_width);
    float v_norm_len = sqrt(1.0 + camera.hf_height * camera.hf_height);

    camera.h_nx = 1.0 / h_norm_len;
    camera.h_ny = camera.hf_width / h_norm_len;

    camera.v_nx = 1.0 / v_norm_len;
    camera.v_nz = camera.hf_height / v_norm_len;
  }
  
  {
    float view_dx = camera.h_cos;
    float view_dy = camera.h_sin;
     
    struct TextWindow *map;

    /* determine the focus tile */
    int fx, fy;

    map = text_wins[sdlgl_map_win];
    assert(sdlgl_map_win != WIN_ERR);
    assert(map);

    fx = map->focus_x;
    fy = win->total_h-1 - map->focus_y;

    camera.viewx = TO_GL_MID_X(fx);
    camera.viewy = TO_GL_MID_Y(fy);
    camera.viewz = 130 + my * 510;
     
    camera.viewx -= view_dx * (290 + (1-my) * 400);
    camera.viewy -= view_dy * (290 + (1-my) * 400);
     
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-camera.hf_width,  +camera.hf_width, 
              -camera.hf_height, +camera.hf_height, 
              camera.near, camera.far);

    glRotatef(270.0 - camera.vert_angle, 1.0, 0.0, 0.0);
    glRotatef(90.0 - camera.horiz_angle, 0.0, 0.0, 1.0);
    glTranslatef(-camera.viewx, -camera.viewy, -camera.viewz);
  }

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  glDisable(GL_NORMALIZE);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glEnable(GL_LIGHTING);

  /* this requires OpenGL 1.2 */
#if 0
  glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
#endif
}

static void finish_3d_rendering(void)
{
  glEnable(GL_NORMALIZE);

  glNormal3f(0.0, 0.0, 1.0);

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glDisable(GL_LIGHTING);

  /* setup the GL matrices for drawing 2D stuff */

  glViewport(0, 0, sdlgl_width, sdlgl_height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, (float)sdlgl_width, 
          0.0, (float)sdlgl_height, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


/* ---------------------------------------------------------------- */

struct ActiveLight
{
  float x, y, z;

  int range;
  
  struct LightInfo *info;  /* NULL if ununsed */
};

#define MAX_ACTIVE_LIGHTS  8

static struct ActiveLight active_lights[MAX_ACTIVE_LIGHTS] =
{ 
  { 0, 0, 0, 0, NULL },
};


static int tile_from_light_source(light_source *ls)
{
  /* Note: this doesn't worry about hallucination.  I figure that
   * hallucination changes the shapes of what you see, but not the
   * lighting you see.
   */
  int glyph;
  
  switch (ls->type)
  {
    case LS_OBJECT:
    {
      struct obj *obj = (struct obj *) ls->id;
      if (! obj || obj->otyp == CORPSE)
        return TILE_EMPTY;

      glyph = GLYPH_OBJ_OFF + (int) obj->otyp;
      assert(glyph >= 0 && glyph < MAX_GLYPH);

      return glyph2tile[glyph];
    }
      
    case LS_MONSTER:
    {
      struct monst *mon = (struct monst *) ls->id;
      if (! mon)
        return TILE_EMPTY;
      
      glyph = GLYPH_MON_OFF + (int) monsndx(mon->data);
      assert(glyph >= 0 && glyph < MAX_GLYPH);
      
      return glyph2tile[glyph];
    }

    default:  /* shouldn't happen */
      return TILE_EMPTY;
  }
}

static struct LightInfo *find_light_source(int lev_x, int lev_y,
    int *range)
{
  light_source *ls;

  light_source *best = NULL;
  int best_range = -1;

  int tile = 0;

  (*range) = 0;

  for (ls = light_base; ls; ls = ls->next)
  {
    if (ls->x != lev_x || ls->y != lev_y)
      continue;

    if (ls->flags & LSF_NEEDS_FIXUP)  /* -AJA- dunno if necessary */
      continue;

    if (ls->range > best_range)
    {
      best_range = ls->range;
      best = ls;
    }
  }

  if (! best)
  {
    if (lev_x == u.ux && lev_y == u.uy)
      return lookup_light("Player");  /* FIXME: lookup once */
     
    return NULL;
  }

  (*range) = best->range;

  tile = tile_from_light_source(best);

  if (tile == TILE_EMPTY)
    return NULL;

  assert(tile >= 0 && tile < SIZE(model_specs));

#if 0  /* DEBUGGING */
  if (! model_specs[tile].light)
  {
    fprintf(stderr, "!!! NO LIGHT for tile %d !\n", tile);
  }
#endif

  return model_specs[tile].light;
}

static void clear_active_lights(void)
{
  int L;

  for (L=0; L < MAX_ACTIVE_LIGHTS; L++)
    active_lights[L].info = NULL;
}

static void add_active_light(float x, float y, float z,
    struct LightInfo *info, int range,
    float base_x, float base_y) 
{
  int L;

  /* find free entry in list */

  for (L=0; L < MAX_ACTIVE_LIGHTS; L++)
  {
    if (! active_lights[L].info)
      break;
  }

  /* if the list is full, try and find a suitable light to replace.
   * It must be further away.
   */
  if (L >= MAX_ACTIVE_LIGHTS)
  {
    float new_dist = (x-base_x) * (x-base_x) + (y-base_y) * (y-base_y);

    int furthest = -1;
    float furth_dist = -1;
    
    /* NOTE: we start at 1, which ensures that the ambient source is
     *       never replaced.  FIXME: this is kludgy.
     */
    for (L=1; L < MAX_ACTIVE_LIGHTS; L++)
    {
      float dx = active_lights[L].x - base_x;
      float dy = active_lights[L].y - base_y;

      float old_dist = dx * dx + dy * dy;
      
      if (old_dist < new_dist + 1)
        continue;

      /* light L is further from new light.  Choose furthest */

      if (old_dist > furth_dist)
      {
        furth_dist = old_dist;
        furthest = L;
      }
    }

    if (furthest < 0)
    {
      /* the new light was further away than all current ones */
      return;
    }

    L = furthest;
  }
   
  active_lights[L].x = x;
  active_lights[L].y = y;
  active_lights[L].z = z;
  active_lights[L].info  = info;
  active_lights[L].range = range;
}

static void build_light_list(float base_x, float base_y, 
    int lev_x, int lev_y, int lev_w, int lev_h)
{
  int L;
  int scan_x, scan_y;
 
  float zero[4] = { 0, 0, 0, 1 };

  /* FIXME: lookup once */
  struct LightInfo *ambi_light = lookup_light("Ambient");

  if (! ambi_light)
  {
    sdlgl_error("Missing standard light `Ambient' !\n");
    return;  /* NOT REACHED */
  }


  clear_active_lights();

  add_active_light(16000, -4000, 1000, ambi_light, 99, base_x, base_y);

  /* FIXME: this is mostly unnecessary (et tres inefficient !) */
  for (scan_y = max(0, lev_y-9); scan_y < max(lev_h,lev_y+10); scan_y++)
  for (scan_x = max(0, lev_x-9); scan_x < max(lev_w,lev_x+10); scan_x++)
  {
    int range;

    struct LightInfo *info = find_light_source(scan_x, scan_y, &range);

    if (info)
    {
      add_active_light(TO_GL_MID_X(scan_x), TO_GL_MID_Y(scan_y), 64.0, 
          info, range, 
          TO_GL_MID_Y(lev_x), TO_GL_MID_Y(lev_y));
    }
  }

  /* set GL parameters */

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, zero);

  for (L=0; L < MAX_ACTIVE_LIGHTS; L++)
  {
    struct ActiveLight *cur = active_lights + L;

    struct LightInfo *info = cur->info;

    int lit_idx = GL_LIGHT0 + L;

    float position[4];

    position[0] = cur->x;
    position[1] = cur->y;
    position[2] = cur->z;
    position[3] = 1;

    if (! info)
    {
      glDisable(lit_idx);
      continue;
    }

    glEnable(lit_idx);

    glLightfv(lit_idx, GL_POSITION, position);

    glLightfv(lit_idx, GL_AMBIENT,  info->ambient);
    glLightfv(lit_idx, GL_DIFFUSE,  info->diffuse);
    glLightfv(lit_idx, GL_SPECULAR, info->specular);
    
    glLightf(lit_idx, GL_CONSTANT_ATTENUATION,  info->attenuation[0]);
    glLightf(lit_idx, GL_LINEAR_ATTENUATION,    info->attenuation[1]);
    glLightf(lit_idx, GL_QUADRATIC_ATTENUATION, info->attenuation[2]);
  }
}
 

/* ---------------------------------------------------------------- */

static float dist_to_normal(float nx, float ny, float px, float py)
{
  /* Normal must be unit length.  Result is negative if point is on
   * left side, positive if on right side.
   *
   *                  ^
   *           LEFT   |   RIGHT
   *                  |
   */

  return px * ny - py * nx;   /* too easy... :) */
}
 
static int sphere_in_camera(float x, float y, float z, float r)
{
  /* translate to view coordinates */

  x -= camera.viewx;
  y -= camera.viewy;
  z -= camera.viewz;

  /* rotate about horizontal angle */
  {
    float ox = x;
    float oy = y;

    x =  camera.h_cos * ox + camera.h_sin * oy;
    y = -camera.h_sin * ox + camera.h_cos * oy;
  }
  
  /* rotate about vertical angle */
  {
    float ox = x;
    float oz = z;

    x =  camera.v_cos * ox + camera.v_sin * oz;
    z = -camera.v_sin * ox + camera.v_cos * oz;
  }

  /* clip against front (near side) of view volume */ 
   
  if (x < -r)
  {
    return 0;
  }
  
  /* view volume is symmetrical, so take advantage of that */

  y = fabs(y);
  z = fabs(z);

  /* clip against left/right sides of view volume */
  {
    if (dist_to_normal(camera.h_nx, camera.h_ny, x, y) < -r)
    {
      return 0;
    }
  }
  
  /* clip against top/bottom sides of view volume */
  {
    if (dist_to_normal(camera.v_nx, camera.v_nz, x, z) < -r)
    {
      return 0;
    }
  }

  return 1;
}

#if 0
static int bbox_in_camera(float x1, float y1, float z1,
    float x2, float y2, float z2)
{
  float mid_x = (x1 + x2) / 2.0;
  float mid_y = (y1 + y2) / 2.0;
  float mid_z = (z1 + z2) / 2.0;

  float ext_x = fabs(x1 - x2) / 2.0;
  float ext_y = fabs(y1 - y2) / 2.0;
  float ext_z = fabs(z1 - z2) / 2.0;

  float ext_max = ext_x > ext_y ?
      (ext_x > ext_z ? ext_x : ext_z) :
      (ext_y > ext_z ? ext_y : ext_z);

  return sphere_in_camera(mid_x, mid_y, mid_z, ext_max * M_SQRT3);
}
#endif

static int tile_in_camera(int tx, int ty)
{
  return sphere_in_camera(tx * 128.0 + 64.0, ty * 128.0 + 64.0,
      /* z */ 64.0, /* radius */ 144.0);
}


/* ---------------------------------------------------------------- */

static int current_ms = 0;


static void load_model_spec(struct ModelSpec *spec, int tile)
{
  const char *model;
  const char *skin;

  char full_model[512];
  char full_skin[512];
    
  if (spec->model[0] == '-')
  {
    if (tile < NUM_MON_TILES)
    {
      model = "x_gelfeet.md2";
      skin  = "x_gelfeet.png";
    }
    else if (tile < 828)
    {
      model = "rubix.md2";
      skin  = "rubix.png";
    }
    else
    {
      model = "x_beartrap.md2";
      skin  = "x_beartrap.png";
    }
  }
  else
  {
    model = spec->model;
    skin  = spec->skin;
  }
  
  /* strcpy(full_model, HACKDIR); */
  strcpy(full_model, "models/");
  strcat(full_model, model);
   
  /* strcpy(full_skin, HACKDIR); */
  strcpy(full_skin, "models/");   /* maybe skins/ ?? */
  strcat(full_skin, skin);
   
  /* first load the skin */
  if (spec->loaded_skin == 0)
  {
    unsigned char *data;

    int width, height;
    
    data = sdlgl_load_png_file(full_skin, &width, &height);

    if (data == NULL)
    {
      sdlgl_error("Image was REQUIRED but missing.\n");
      return;  /* NOT REACHED */
    }

    spec->loaded_skin = sdlgl_send_graphic_RGBA(data, width, height);
  }
 
  /* secondly, load the model */
  if (spec->loaded_model == NULL)
  {
    struct Model3D *data;

    data = load_md2_model(full_model);

    if (data == NULL)
    {
      sdlgl_error("Model was REQUIRED but missing.\n");
      return;  /* NOT REACHED */
    }

    if (model[0] == 'x' && model[1] == '_')
    {
      data->frame.scale[0] *= 3;
      data->frame.scale[1] *= 3;
      data->frame.scale[2] *= 3;

      data->frame.translate[0] *= 3;
      data->frame.translate[1] *= 3;
      data->frame.translate[2] *= 3;
    }
    
    spec->loaded_model = data;
  }
}

static void load_model_for_tile(int tile)
{
  int has_model = 0;
  int has_skin  = 0;

  if (tile == TILE_EMPTY)
    return;

  if (model_specs[tile].loaded_model)
    has_model = 1;

  if (model_specs[tile].loaded_skin != 0)
    has_skin = 1;

  if (has_model && has_skin)
    return;

  load_model_spec(model_specs + tile, tile);

  /* check for other tiles using the same skin */
  if (! has_skin)
  {
    int i;

    for (i=0; i < num_model_specs; i++)
    {
      if (i == tile)
        continue;

      if (model_specs[i].skin[0] == '-')
        continue;

      if (strcmp(model_specs[i].skin, model_specs[tile].skin) == 0)
        model_specs[i].loaded_skin = model_specs[tile].loaded_skin;
    }
  }
 
  /* check for other tiles using the same 3D model */
  if (! has_model)
  {
    int i;

    for (i=0; i < num_model_specs; i++)
    {
      if (i == tile)
        continue;

      if (model_specs[i].model[0] == '-')
        continue;

      if (strcmp(model_specs[i].model, model_specs[tile].model) == 0)
        model_specs[i].loaded_model = model_specs[tile].loaded_model;
    }
  }
}

static void load_extra_graphics(void)
{
  pet_mark_spec.material = ridden_mark_spec.material =
      lookup_material("PetMark");

  if (! pet_mark_spec.material)
  {
    sdlgl_error("Missing standard material `PetMark' !\n");
    return;  /* NOT REACHED */
  }

  load_model_spec(&pet_mark_spec, 700);
  load_model_spec(&ridden_mark_spec, 700);

  assert(pet_mark_spec.loaded_model != NULL);
  assert(ridden_mark_spec.loaded_model != NULL);
}

static GH_INLINE void draw_one_3d_tile(int tile, int bx, int by, int bz, 
    float angle, int hidden_sides)
{
  if (tile == TILE_EMPTY)
    return;

  render_md2_model(model_specs[tile].loaded_model, 
      model_specs[tile].loaded_skin, 
      model_specs[tile].material, 
      bx, by, bz, 
      model_specs[tile].scale,
      angle + model_specs[tile].rotate,
      hidden_sides);
}

static void draw_3d_border(struct TileWindow *win)
{
  float map_w = win->total_w * 128.0;
  float map_h = win->total_h * 128.0;

  glDisable(GL_LIGHTING);

  glColor3f(0, 0, 0.5);

  glBegin(GL_LINE_LOOP);

  glVertex3f(-10, -10, 0);
  glVertex3f(map_w + 10, -10, 0);
  glVertex3f(map_w + 10, map_h + 10, 0);
  glVertex3f(-10, map_h + 10, 0);

  glEnd();

  glEnable(GL_LIGHTING);
}

static void draw_3d_cursor(struct TileWindow *win)
{
  float x = win->curs_x * 128.0;
  float y = win->curs_y * 128.0;
  float z = 0;

  glDisable(GL_LIGHTING);

  glColor3f(1, 1, 1);

  for (z = 10.0; z < 150.0; z += 130)
  {
    glBegin(GL_LINE_LOOP);

    glVertex3f(x -  10, y -  10, z);
    glVertex3f(x + 138, y -  10, z);
    glVertex3f(x + 138, y + 138, z);
    glVertex3f(x -  10, y + 138, z);

    glEnd();
  }

  glBegin(GL_LINES);

  glVertex3f(x -  10, y -  10, 10);
  glVertex3f(x -  10, y -  10, 140);

  glVertex3f(x + 140, y -  10, 10);
  glVertex3f(x + 140, y -  10, 140);

  glVertex3f(x + 140, y + 140, 10);
  glVertex3f(x + 140, y + 140, 140);

  glVertex3f(x -  10, y + 140, 10);
  glVertex3f(x -  10, y + 140, 140);

  glEnd();

  glEnable(GL_LIGHTING);
}

static void draw_3d_extra_shape(struct TileWindow *win,
    struct ExtraShape *shape)
{
  if (! tile_in_camera(shape->x, shape->y))
    return;

  switch (shape->type)
  {
    case SHAPE_None:
      return;

    case SHAPE_Heart:
    case SHAPE_Ridden:
    {
      float x = shape->x * 128.0 + 64.0;
      float y = shape->y * 128.0 + 64.0;
      float z = 116 + 8 * sin((current_ms % 1000) * M_PI * 2.0 / 1000.0);
       
      struct ModelSpec *spec = &pet_mark_spec;

      if (shape->type == SHAPE_Ridden)
        spec = &ridden_mark_spec;
      
      assert(spec->loaded_model);

      render_md2_model(spec->loaded_model, spec->loaded_skin, 
          spec->material, x, y, z, spec->scale,
          (current_ms % 2000) * 360.0 / 2000.0, 0);
    }
    break;
  }
}

#define Pitted  (u.utrap != 0 && u.utraptype == TT_PIT)

static void do_anims(void)
{
  float target_z;

  target_z = Levitation ? 64.0 : Pitted ? -48 : 0.0;

  if (fabs(target_z - player_z) > 0.01)
  {
    if (player_z_last_ms == -1)
      player_z_last_ms = current_ms;
    else if (current_ms > player_z_last_ms)
    {
      int delta = (target_z > player_z) ? +1 : -1;
      
      player_z += delta * (current_ms - player_z_last_ms) / 
        (Levitation ? 30.0 : 10.0);

      if ((delta > 0 && player_z >= target_z) ||
          (delta < 0 && player_z <= target_z))
      {
        player_z = target_z;
        player_z_last_ms = -1;
      }

      player_z_last_ms = current_ms;
    }
  }
  else
  {
    player_z_last_ms = -1;
  }
}

static int get_wall_sides(int tile)
{
  if (tile == TILE_EMPTY)
    return 0;

  /* FIXME: hardcoded tile values are nasty. */

  if (tile >= 830  && tile <= 840)
    tile -= 830;
  else if (tile >= 1013 && tile <= 1023)
    tile -= 1013;
  else if (tile >= 1024 && tile <= 1034)
    tile -= 1024;
  else if (tile >= 1035 && tile <= 1045)
    tile -= 1035;
  else if (tile >= 1046 && tile <= 1056)
    tile -= 1046;
  else
  {
    return 0;
  }

  assert(0 <= tile && tile <= 10);

  switch (tile)
  {
    case 0: return WSIDE_TOP | WSIDE_BOTTOM;
    case 1: return WSIDE_LEFT | WSIDE_RIGHT;
    case 2: return WSIDE_RIGHT | WSIDE_BOTTOM;
    case 3: return WSIDE_LEFT  | WSIDE_BOTTOM;
    case 4: return WSIDE_RIGHT | WSIDE_TOP;
    case 5: return WSIDE_LEFT | WSIDE_TOP;
    case 6: return WSIDE_LEFT | WSIDE_RIGHT | WSIDE_TOP | WSIDE_BOTTOM;
    case 7: return WSIDE_LEFT | WSIDE_RIGHT | WSIDE_TOP;
    case 8: return WSIDE_LEFT | WSIDE_RIGHT | WSIDE_BOTTOM;
    case 9: return WSIDE_LEFT | WSIDE_TOP | WSIDE_BOTTOM;
    case 10: return WSIDE_RIGHT | WSIDE_TOP | WSIDE_BOTTOM;
  }

  return 0; /* NOT REACHED */
}

#define TILE_AT_ANY(win, x, y)  \
  ( ((x) < 0 || (x) >= (win)->total_w ||  \
     (y) < 0 || (y) >= (win)->total_h) ? TILE_EMPTY :  \
    win->tiles[(y) * (win)->total_w + (x)].mg)

/* checks whether the wall at the given location has sides that are
 * hidden by adjacent solid walls.  When we stop drawing those sides,
 * it looks better.  Returns a bitmask of WSIDE_* values, or 0 if
 * there is no wall at that location.
 */
static int check_for_hidden_sides(struct TileWindow *win, int x, int y)
{
  int middle = TILE_AT_ANY(win, x, y);
  int left   = TILE_AT_ANY(win, x-1, y);
  int right  = TILE_AT_ANY(win, x+1, y);
  int top    = TILE_AT_ANY(win, x, y+1);
  int bottom = TILE_AT_ANY(win, x, y-1);

  middle = get_wall_sides(middle);
  left   = get_wall_sides(left);
  right  = get_wall_sides(right);
  top    = get_wall_sides(top);
  bottom = get_wall_sides(bottom);

  left   = (left & WSIDE_RIGHT) ? (middle & WSIDE_LEFT)   : 0;
  right  = (right & WSIDE_LEFT) ? (middle & WSIDE_RIGHT)  : 0;
  top    = (top & WSIDE_BOTTOM) ? (middle & WSIDE_TOP)    : 0;
  bottom = (bottom & WSIDE_TOP) ? (middle & WSIDE_BOTTOM) : 0;
 
  return left | right | top | bottom;
}

static void get_model_bbox(struct Model3D *mod, float *bbox)
{
  /* NOTE: this only handles a single frame */

  /* FIXME: doesn't handle rotation */

  bbox[0] = mod->frame.translate[0];
  bbox[1] = mod->frame.translate[1];
  bbox[2] = mod->frame.translate[2];

  bbox[3] = bbox[0] + 255 * mod->frame.scale[0];
  bbox[4] = bbox[1] + 255 * mod->frame.scale[1];
  bbox[5] = bbox[2] + 255 * mod->frame.scale[2];
}

static int check_fg_overlap_mg(int fg, int mg)
{
  if (fg == TILE_EMPTY || mg == TILE_EMPTY)
    return 0;

  if (check_flag(mg, 'M'))
    return 0;

  /* check heights */
  {
    struct Model3D *fg_mod = model_specs[fg].loaded_model;
    struct Model3D *mg_mod = model_specs[mg].loaded_model;

    float fg_bbox[6];
    float mg_bbox[6];
    
    assert(fg_mod);
    assert(mg_mod);
    
    get_model_bbox(fg_mod, fg_bbox);
    get_model_bbox(mg_mod, mg_bbox);

    if (fg_bbox[2] > mg_bbox[5] - 2.0)
      return 0;
  }

  return 1;
}

int get_monst_angle(struct TilePair *cur, int lev_x, int lev_y)
{
  int dx=0, dy=0;

  struct TextWindow *map;

  map = text_wins[sdlgl_map_win];
  assert(sdlgl_map_win != WIN_ERR);
  assert(map);

  if (cur->fg == TILE_EMPTY)
    return 0;

  if (cur->fg >= NUM_MON_TILES)
    return 0;

  if (lev_x == u.ux && lev_y == u.uy)
  {
    dx = map->player_dx;
    dy = map->player_dy;
  }
  else
  {
    struct monst *mon = m_at(lev_x, lev_y);

    int i;
    
    if (! mon)
      return 0;

    for (i=0; i < MTSZ; i++)
    {
      dx = mon->mx - mon->mtrack[i].x;
      dy = mon->my - mon->mtrack[i].y;

      if (dx != 0 || dy != 0)
        break;
    }
  }

  dx = sgn(dx) + 1;
  dy = sgn(dy) + 1;

  assert(0 <= dx && dx <= 2);
  assert(0 <= dy && dy <= 2);

  switch (dy * 3 + dx)
  {
    case 5: return 0;    /* E  */
    case 2: return 45;   /* NE */
    case 1: return 90;   /* N  */
    case 0: return 135;  /* NW */
    case 3: return 180;  /* W  */
    case 6: return 225;  /* SW */
    case 7: return 270;  /* S  */
    case 8: return 315;  /* SE */

    default: /* center */
      return 270;
  }
}

void sdlgl_3d_draw_map(struct TileWindow *win)
{
  int x, y;
 
  current_ms = sdlgl_get_time();

  if (! youmonst.data)
    return;

  do_anims();
  
  if (pet_mark_spec.loaded_model == NULL)
    load_extra_graphics();

  begin_3d_rendering(win);

  glClear(GL_DEPTH_BUFFER_BIT);

  for (y=win->total_h - 1; y >= 0; y--)
  {
    for (x=0; x < win->total_w; x++)
    {
      struct TilePair *cur = win->tiles + (y * win->total_w + x);

      int bx = x * 128 + 64;
      int by = y * 128 + 64;
      int bz = 0;

      float angle = get_monst_angle(cur, x, win->total_h-1 - y);

      int is_player = 0;
      int no_floor;
      int no_mid;

      if (! tile_in_camera(x, y))
        continue;

      if (x == u.ux && (win->total_h-1 - y) == u.uy)
        is_player = 1;

      /* rotate gems */
      if (check_flag(cur->fg, 'R'))
      {
        angle = (current_ms % 18000) / 50.0;
      }

      build_light_list(bx, by, x, win->total_h-1 - y,
          win->total_w, win->total_h);

      load_model_for_tile(cur->u.bg);
      load_model_for_tile(cur->mg);
      load_model_for_tile(cur->fg);

      no_mid = check_fg_overlap_mg(cur->fg, cur->mg);

      no_floor = (! no_mid && check_flag(cur->mg, 'F')) |
                 check_flag(cur->fg, 'F');
       
      if (! no_floor)
        draw_one_3d_tile(cur->u.bg, bx, by, bz, 0, 0);
 
      if (! no_mid)
        draw_one_3d_tile(cur->mg, bx, by, bz, 0,
          check_for_hidden_sides(win, x, y));

      draw_one_3d_tile(cur->fg, bx, by, 
        is_player ? player_z : bz, angle, 0);
    }
  }
  
  draw_3d_border(win);

  if (win->curs_x >= 0 && ! win->curs_block)
    draw_3d_cursor(win);

  /* draw the extra shapes, like the love heart */
  {
    int n;

    for (n=0; n < win->extra_num; n++)
      draw_3d_extra_shape(win, win->extra_shapes + n);
  }

  finish_3d_rendering();
}

static void free_extra_graphics(void)
{
  /* FIXME: free extras */
}

void sdlgl_3d_startup(void)
{
  load_keyworded_file("gllights.lst", 8, &parse_light);
  load_keyworded_file("glmats.lst",   7, &parse_material);
  load_keyworded_file("glmodels.lst", 7, &parse_model_spec);

  if (num_model_specs != (FLOOR_TILES+8))
  {
    sdlgl_error("Model specs mismatch ! (got %d, wanted %d)\n",
        num_model_specs, FLOOR_TILES+8);
    return;  /* NOT REACHED */
  }
}

void sdlgl_3d_shutdown(void)
{
  free_extra_graphics();

  /* FIXME: free everything */
}


#endif /* GL3D_GRAPHICS */
/*gl_3d.c*/

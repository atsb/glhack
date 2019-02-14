/*
 * SIMPLE Md2 CREATOR
 *
 * By Andrew Apted (c) 2002.  Under the GNU GPL.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#ifndef MAX
#define MAX(a,b)  ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)  ((a) < (b) ? (a) : (b))
#endif

#ifndef ABS
#define ABS(a)  ((a) < 0 ? -(a) : (a))
#endif


#define NUMVERTEXNORMALS  162

static float avertexnormals[NUMVERTEXNORMALS][3] =
{
#include "q2_norms.h"
};


char linebuf[2048];

typedef struct
{
  float x, y, z;
  int s, t;
  float nx, ny, nz;
}
Vertex_t;

static Vertex_t verts[2048];
int num_verts = 0;

static Vertex_t scale;
static Vertex_t translate;


static int skin_width;
static int skin_height;

static int cmd_vertices[2048];
int num_cmd_verts = 0;

static int gl_commands[2048];
int num_gl_cmds = 0;
int total_gl_size = 0;

static int triangle_verts[1000][3];
int num_triangles = 0;


static unsigned char file_data[128000];
int file_len = 0;

#define HEADER_SKINS       (11 * 4)
#define HEADER_TEX_COORDS  (12 * 4)
#define HEADER_TRIANGLES   (13 * 4)
#define HEADER_FRAMES      (14 * 4)
#define HEADER_GL_CMDS     (15 * 4)
#define HEADER_END         (16 * 4)


/* ---------------------------------------------------------------------- */

void ReadSkin(FILE *fp)
{
  if (fscanf(fp, " %d %d ", &skin_width, &skin_height) != 2)
  {
    fprintf(stderr, "Error reading skin size !\n");
    exit(18);
  }

  if (skin_width <= 0 || skin_height <= 0)
  {
    fprintf(stderr, "Bad skin size ! (%dx%d)\n", skin_width, skin_height);
    exit(19);
  }
}

void ReadVertices(FILE *fp)
{
  int total;

  if (fscanf(fp, " %d ", &total) != 1 || total <= 0)
  {
    fprintf(stderr, "Error reading vertex count !\n");
    exit(21);
  }

  for (; total > 0; total--)
  {
    Vertex_t tmp;

    if (fscanf(fp, " %f %f %f  %d %d  %f %f %f ",
        &tmp.x, &tmp.y, &tmp.z,
        &tmp.s, &tmp.t,
        &tmp.nx, &tmp.ny, &tmp.nz) != 8)
    {
      fprintf(stderr, "Error reading vertex[%d] !\n", num_verts);
      exit(22);
    }

    if (tmp.s < 0 || tmp.s >= skin_width ||
        tmp.t < 0 || tmp.t >= skin_height)
    {
      fprintf(stderr, "Bad skin coordinate for vertex[%d] ! (%dx%d)\n", 
          num_verts, tmp.s, tmp.t);
      exit(23);
    }

    /* convert normal to unit vector */

    if (fabs(tmp.nx) + fabs(tmp.ny) + fabs(tmp.nz) < 0.0001)
    {
      fprintf(stderr, "Zero-length normal for vertex[%d] !\n", num_verts);
      exit(24);
    }
    else
    {
      float n_dist = sqrt(tmp.nx * tmp.nx + tmp.ny * tmp.ny + 
          tmp.nz * tmp.nz);
    
      tmp.nx /= n_dist;
      tmp.ny /= n_dist;
      tmp.nz /= n_dist;
    }

    verts[num_verts++] = tmp;
  }

  fprintf(stderr, "Loaded %d vertices\n", num_verts);
}

void ReadGlCommands(FILE *fp)
{
  for (;;)
  {
    int count, p;

    if (fscanf(fp, " %d ", &count) != 1)
    {
      fprintf(stderr, "Error reading glcommand starter !\n");
      exit(31);
    }

    total_gl_size += 4;

    if (count == 0)
      break;

    gl_commands[num_gl_cmds++] = count;

    for (p=0; p < ABS(count); p++)
    {
      int v;

      if (fscanf(fp, " %d ", &v) != 1)
      {
        fprintf(stderr, "Error reading glcommand vertex !\n");
        exit(32);
      }

      if (v < 0 || v >= num_verts)
      {
        fprintf(stderr, "Illegal glcommand vertex %d !\n", v);
        exit(32);
      }

      cmd_vertices[num_cmd_verts++] = v;

      total_gl_size += 3*4;
    }
  }

  if (num_gl_cmds == 0)
  {
    fprintf(stderr, "Missing glcommands !\n");
    exit(34);
  }

  fprintf(stderr, "Loaded %d gl_cmds (%d cmd_verts)\n", 
      num_gl_cmds, num_cmd_verts);
}

void ComputeScaleTrans(void)
{
  int i;

  Vertex_t minbox = { +1e9, +1e9, +1e9, 0, };
  Vertex_t maxbox = { -1e9, -1e9, -1e9, 0, };

  for (i=0; i < num_verts; i++)
  {
    minbox.x = MIN(verts[i].x, minbox.x);
    minbox.y = MIN(verts[i].y, minbox.y);
    minbox.z = MIN(verts[i].z, minbox.z);

    maxbox.x = MAX(verts[i].x, maxbox.x);
    maxbox.y = MAX(verts[i].y, maxbox.y);
    maxbox.z = MAX(verts[i].z, maxbox.z);
  }

  translate.x = minbox.x;
  translate.y = minbox.y;
  translate.z = minbox.z;

  fprintf(stderr, "Computed translation: (%1.2f, %1.2f, %1.2f)\n",
      translate.x, translate.y, translate.z);

  scale.x = (maxbox.x - minbox.x) / 255.0;
  scale.y = (maxbox.y - minbox.y) / 255.0;
  scale.z = (maxbox.z - minbox.z) / 255.0;

  fprintf(stderr, "Computed scale: (%1.2f, %1.2f, %1.2f)\n",
      scale.x, scale.y, scale.z);
}

static void TrianglesFromStrip(int count, int *vert_list)
{
  int i;

  for (i=0; i+3 <= count; i++)
  {
    int v1 = vert_list[i];
    int v2 = vert_list[i+1];
    int v3 = vert_list[i+2];

    /* make sure triangle faces the correct direction (inwards or
     * outwards), since within a strip it alternates.
     */
    if ((i % 2) == 1)
    {
      int tmp = v1; v1 = v2; v2 = tmp;
    }

    /* fprintf(stderr, "TRI: %d .. %d .. %d\n", v1, v2, v3); */

    triangle_verts[num_triangles][0] = v1;
    triangle_verts[num_triangles][1] = v2;
    triangle_verts[num_triangles][2] = v3;

    num_triangles++;
  }
}

static void TrianglesFromFan(int count, int *vert_list)
{
  int i;
  int v1 = vert_list[0];

  for (i=1; i+2 <= count; i++)
  {
    int v2 = vert_list[i];
    int v3 = vert_list[i+1];

    /* fprintf(stderr, "FAN: %d .. %d .. %d\n", v1, v2, v3); */

    triangle_verts[num_triangles][0] = v1;
    triangle_verts[num_triangles][1] = v2;
    triangle_verts[num_triangles][2] = v3;

    num_triangles++;
  }
}

void ComputeTriangles(void)
{
  /* Probably the most complicated bit of code here ! */

  int c;
  int cmd_vert_p = 0;

  for (c=0; c < num_gl_cmds; c++)
  {
    int count = gl_commands[c];

    if (count < 0)
    {
      TrianglesFromFan(-count, cmd_vertices + cmd_vert_p);
    }
    else
    {
      TrianglesFromStrip(count, cmd_vertices + cmd_vert_p);
    }

    cmd_vert_p += ABS(count);
  }

  fprintf(stderr, "Computed %d triangles\n", num_triangles);
}


/* ---------------------------------------------------------------------- */

int ConvertNormal(const Vertex_t *V)
{
  int i;

  float maxdot = -999999.0;
  int best_normal = 0;

  for (i=0; i < NUMVERTEXNORMALS; i++)
  {
    float nx2 = avertexnormals[i][0];
    float ny2 = avertexnormals[i][1];
    float nz2 = avertexnormals[i][2];

    /* This calculation (finding the highest dot product) is
     * equivalent to finding the lowest euclidean distance (sum of the
     * squared differences).  Here's why:
     *
     *   (x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2
     * 
     *   = (x1^2 - 2.x1.x2 + x2^2) + (y1^2 - 2.y1.y2 + y2^2) +
     *     (z1^2 - 2.z1.z2 + z2^2)
     * 
     *   = (x1^2 + y1^2 + z1^2) + (x2^2 + y2^2 + z2^2) -
     *     2 * (x1.x2 + y1.y2 + z1.z2)
     *
     *   = 1 + 1 - 2 * (x1.x2 + y1.y2 + z1.z2)
     *   
     * Since the vectors have unit length, (x1^2 + y1^2 + z1^2) and 
     * (x2^2 + y2^2 + z2^2) are both 1.
     */

    float dot = V->nx * nx2 + V->ny * ny2 + V->nz * nz2;

    if (dot > maxdot)
    {
      maxdot = dot;
      best_normal = i;
    }
  }

  return best_normal;
}


/* ---------------------------------------------------------------------- */

void StoreInt(int value, int pos)
{
  file_data[pos + 0] = (value & 0xFF);
  file_data[pos + 1] = ((value >>  8) & 0xFF);
  file_data[pos + 2] = ((value >> 16) & 0xFF);
  file_data[pos + 3] = ((value >> 24) & 0xFF);
}

void AddByte(int value)
{
  file_data[file_len++] = (unsigned char) value;
}
    
void AddShort(int value)
{
  AddByte(value & 0xFF);
  AddByte(value >> 8);
}
    
void AddInt(int value)
{
  AddByte(value & 0xFF);
  AddByte((value >>  8) & 0xFF);
  AddByte((value >> 16) & 0xFF);
  AddByte((value >> 24) & 0xFF);
}
 
void AddFloat(float value)
{
  volatile union
  {
    float f;
    unsigned char c[4];
  }
  hackiness;

  hackiness.f = value;

  AddByte(hackiness.c[0]);
  AddByte(hackiness.c[1]);
  AddByte(hackiness.c[2]);
  AddByte(hackiness.c[3]);
}

void AddString(const char *str, int len)
{
  int i;
  int str_size = strlen(str);

  for (i = 0; i < len; i++)
  {
    AddByte(i < str_size ? str[i] : 0);
  }
}
 

/* ---------------------------------------------------------------------- */

void MakeHeader(void)
{
  AddString("IDP2", 4);  /* magic */
  AddInt(8);             /* version */

  AddInt(skin_width);
  AddInt(skin_height);
 
  AddInt(4*3 + 4*3 + 16 + 4 * num_verts);  /* frame size */
  
  AddInt(1);           /* num skins */ 
  AddInt(num_verts);   /* num vertices */
  AddInt(num_verts);   /* num tex coords */

  AddInt(num_triangles);
  AddInt(total_gl_size / 4);
  AddInt(1);           /* num frames */

  /* offsets -- filled in later */
  AddInt(0);
  AddInt(0);
  AddInt(0);
  AddInt(0);
  AddInt(0);
  AddInt(0);
}

void MakeSkins(void)
{
  StoreInt(file_len, HEADER_SKINS);
 
  AddString("tris0.ppm", 64);
}

void MakeTexCoords(void)
{
  int i;

  StoreInt(file_len, HEADER_TEX_COORDS);
 
  for (i=0; i < num_verts; i++)
  {
    AddShort(verts[i].s);
    AddShort(verts[i].t);
  }
}

void MakeTriangles(void)
{
  int t;

  StoreInt(file_len, HEADER_TRIANGLES);
 
  for (t=0; t < num_triangles; t++)
  {
    AddShort(triangle_verts[t][0]);
    AddShort(triangle_verts[t][1]);
    AddShort(triangle_verts[t][2]);

    /* tex-coord indices (we use a one-to-one mapping with vertices) */
    AddShort(triangle_verts[t][0]);
    AddShort(triangle_verts[t][1]);
    AddShort(triangle_verts[t][2]);
  }
}

void MakeFrames(void)
{
  int i;
  
  StoreInt(file_len, HEADER_FRAMES);
 
  AddFloat(scale.x);
  AddFloat(scale.y);
  AddFloat(scale.z);
 
  AddFloat(translate.x);
  AddFloat(translate.y);
  AddFloat(translate.z);

  AddString("idle", 16);   /* frame name */

  for (i=0; i < num_verts; i++)
  {
    int x = (verts[i].x - translate.x) / scale.x;
    int y = (verts[i].y - translate.y) / scale.y;
    int z = (verts[i].z - translate.z) / scale.z;

    x = MAX(0, MIN(255, x));
    y = MAX(0, MIN(255, y));
    z = MAX(0, MIN(255, z));

    AddByte(x);
    AddByte(y);
    AddByte(z);

    AddByte(ConvertNormal(verts + i));
  }
}

void MakeGlCommands(void)
{
  int c, p;
  int cmd_vert_p = 0;
  
  StoreInt(file_len, HEADER_GL_CMDS);
 
  for (c=0; c < num_gl_cmds; c++)
  {
    AddInt(gl_commands[c]);

    for (p=0; p < ABS(gl_commands[c]); p++)
    {
      int v = cmd_vertices[cmd_vert_p++];

      float s = (verts[v].s + 0.5) / (float) skin_width;
      float t = (verts[v].t + 0.5) / (float) skin_height;
      
      AddFloat(s);
      AddFloat(t);

      AddInt(v);
    }
  }

  AddInt(0);
}

void MakeEnd(void)
{
  StoreInt(file_len, HEADER_END);
}


/* ---------------------------------------------------------------------- */


int main(int argc, const char **argv)
{
  ReadSkin(stdin);
  ReadVertices(stdin);
  ReadGlCommands(stdin);

  ComputeScaleTrans();
  ComputeTriangles();

  MakeHeader();
  MakeSkins();
  MakeTexCoords();
  MakeTriangles();
  MakeFrames();
  MakeGlCommands();
  MakeEnd();

  if (fwrite(file_data, 1, file_len, stdout) != file_len)
  {
    fprintf(stderr, "ERROR: failed to write %d bytes\n", file_len);
    return 1;
  }

  return 0;
}

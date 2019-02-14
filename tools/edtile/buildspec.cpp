//------------------------------------------------------------------------
//  BUILDSPEC Program
//------------------------------------------------------------------------
//
//  (C) 2001-2002 Andrew Apted
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

#include "z_args.h"
#include "z_string.h"

#include "t_specs.h"


/* NetHack 3.4.0 */

#define TOTAL_NUM   (1057 + 8)

#define MON_3D  "mon_3D.tsp"
#define OBJ_3D  "obj_3D.tsp"
#define OTH_3D  "oth_3D.tsp"


static SpecGroup *mons;
static SpecGroup *objs;
static SpecGroup *oths;


static void InstallSpecs(FILE *fp, SpecGroup *group, int& pos)
{
  int i;

  for (i=0; i < group->total; i++, pos++)
  {
    ModelSpec *spec = group->specs + i;

    char flag_buf[64];
    int flag_count = 0;
    int flag;
    
    const char *model = spec->model[0] ? spec->model : "-";
    const char *skin = spec->skin[0] ? spec->skin : "-";
    const char *material = spec->material[0] ? spec->material : "-";
    const char *light = spec->light[0] ? spec->light : "-";

    for (flag = 0; flag < 26; flag++)
    {
      if (spec->flags & (1 << flag))
        flag_buf[flag_count++] = 'A' + flag;
    }

    if (flag_count == 0)
      flag_buf[flag_count++] = '-';

    flag_buf[flag_count] = 0;

    fprintf(fp, "%s\t%s\t%s\t%s\t" "%1.2f\t%1.1f\t%s\n", 
        model, skin, material, light,
        spec->scale, spec->rotate, flag_buf);
  }
}

static void SaveAsCode(const char *filename,
    SpecGroup *g1, SpecGroup *g2, SpecGroup *g3)
{
  fprintf(stderr, "Saving %s...\n", filename);

  FILE *fp = fopen(filename, "w");

  if (! fp)
    return;

  int pos = 0;

  InstallSpecs(fp, g1, pos);

  if (g2)
    InstallSpecs(fp, g2, pos);

  if (g3)
    InstallSpecs(fp, g3, pos);

  fprintf(stderr, "\n");

  fclose(fp);
}


//------------------------------------------------------------------------

static void ShowInfo()
{
  fprintf(stdout,
      "\n**** BuildSpec 0.6 ****\n\n"
      "Copyright (C) 2001-2002 Andrew Apted.\n"
      "Under the GNU General Public License (GPL).\n\n"
      "USAGE: buildspec tspdir\n\n"
  );

  fflush(stdout);
}

int main(int argc, const char **argv, const char **envp)
{
  if (argc > 1 &&
      (Z_String::CaseCmp(argv[1], "/?") == 0 ||
       Z_String::CaseCmp(argv[1], "-h") == 0 ||
       Z_String::CaseCmp(argv[1], "-help") == 0 ||
       Z_String::CaseCmp(argv[1], "--help") == 0))
  {
    ShowInfo();
    exit(0);
  }

  if (argc != 2)
  {
    fprintf(stderr, "USAGE: buildspec tspdir\n");
    exit(2);
  }
 
  Z_Args::AddMany(argc - 1, argv + 1);
   
  const char *tdir = argv[1];


  mons = SpecGroup::Load(Filename(MON_3D).SetPath(tdir));
  objs = SpecGroup::Load(Filename(OBJ_3D).SetPath(tdir));
  oths = SpecGroup::Load(Filename(OTH_3D).SetPath(tdir));

  Filename dest("glmodels.lst");

  dest.SetPath(tdir);
  
  SaveAsCode(dest.str.c_str(), mons, objs, oths);


  return 0;
}

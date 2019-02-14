//----------------------------------------------------------------------------
//  MODEL Specs
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

using namespace std;

#include "z_config.h"

#include "t_specs.h"
#include "t_alias.h"

#define MAX_LINE  512
#define MAX_SIZE  128


SpecGroup::SpecGroup() : total(0), skip_empties(0),
    got_spec(0), got_id(-1), got_name(0)
{
  memset(specs, 0, sizeof(specs));
  
  memset(&tmp_spec, 0, sizeof(tmp_spec));
  tmp_spec.scale = 1;

  // clear hash table
  for (int i=0; i < T_Alias::hash_size; i++)
    hash_tab[i] = -1;
}

SpecGroup::~SpecGroup()
{
  memset(specs, 0, sizeof(specs));
}


/* ---------------------------------------------------------------- */

void SpecGroup::DoTag(char *line)
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

void SpecGroup::AddToHashTable(int index)
{
  ModelSpec *spec = specs + index;
  int hash;

  ASSERT(spec->name);
 
  spec->hash_next = -1;

  hash = T_Alias::Hash(string(spec->name));

  if (hash_tab[hash] >= 0)
    spec->hash_next = hash_tab[hash];

  hash_tab[hash] = index;
}

void SpecGroup::StartTile()
{
  if (got_spec != 0)
  {
    fprintf(stderr, "Error: unexpected '{' found.\n");
    exit(26);
  }

  got_spec++;

  memset(&tmp_spec, 0, sizeof(tmp_spec));
  tmp_spec.scale = 1;
}

void SpecGroup::EndTile()
{
  ModelSpec *spec;

  got_spec--;

  if (got_spec != 0)
  {
    fprintf(stderr, "Error: unexpected '}' found.\n");
    exit(32);
  }

  if (got_id < 0)
  {
    fprintf(stderr, "Missing spec ID !\n");
    exit(33);
  }

  if (!got_name)
  {
    fprintf(stderr, "Missing spec name !\n");
    exit(33);
  }

  /* copy data into ModelSpec structure */

  spec = specs + total;
  total++;

  if (total >= TL_MAX_SPECS)
  {
    fprintf(stderr, "Too many specs !\n");
    exit(39);
  }
 
  spec->spec_id = got_id;
  got_id = -1;

  spec->name = got_name;
  got_name = NULL;

  ASSERT(spec->name);

  strcpy(spec->model, tmp_spec.model);
  strcpy(spec->skin, tmp_spec.skin);
  strcpy(spec->material, tmp_spec.material);
  strcpy(spec->light, tmp_spec.light);

  spec->scale  = tmp_spec.scale;
  spec->rotate = tmp_spec.rotate;
  spec->flags  = tmp_spec.flags;

  AddToHashTable(total-1);
}

void SpecGroup::TileDatum(char *line)
{
  char keyword[MAX_LINE];
  char *eq;

  while (*line && isspace(*line))
    line++;

  eq = strchr(line, '=');

  if (eq == NULL)
  {
    fprintf(stderr, "Illegal datum: missing `=' in `%s'\n", line);
    exit(41);
  }

  *eq++ = 0;

  if (sscanf(line, "%s", keyword) != 1)
  {
    fprintf(stderr, "Illegal datum: bad keyword `%s'\n", line);
    exit(42);
  }

  line = eq;

  while (*line && isspace(*line))
    line++;

  if (strlen(line) == 0)
  {
    fprintf(stderr, "Illegal datum: missing value for `%s'\n", keyword);
    exit(43);
  }
  else if (strlen(line) > 60)
  {
    fprintf(stderr, "Illegal datum: value too long `%s'\n", line);
    exit(44);
  }

  if (strcmp(keyword, "model") == 0)
  {
    strcpy(tmp_spec.model, line);
  }
  else if (strcmp(keyword, "skin") == 0)
  {
    strcpy(tmp_spec.skin, line);
  }
  else if (strcmp(keyword, "material") == 0)
  {
    strcpy(tmp_spec.material, line);
  }
  else if (strcmp(keyword, "light") == 0)
  {
    strcpy(tmp_spec.light, line);
  }
  else if (strcmp(keyword, "scale") == 0)
  {
    if (sscanf(line, " %f ", &tmp_spec.scale) != 1 ||
        tmp_spec.scale <= 0)
    {
      fprintf(stderr, "Illegal datum: bad scale value `%s'\n", line);
      exit(46);
    }
  }
  else if (strcmp(keyword, "rotate") == 0)
  {
    if (sscanf(line, " %f ", &tmp_spec.rotate) != 1)
    {
      fprintf(stderr, "Illegal datum: bad rotate value `%s'\n", line);
      exit(46);
    }
  }
  else if (strcmp(keyword, "flags") == 0)
  {
    int flag;

    for (flag = 0; flag < 26; flag++)
    {
      if (strchr(line, 'A' + flag) != NULL || 
          strchr(line, 'a' + flag) != NULL)
      {
        tmp_spec.flags |= (1 << flag);
      }
    }
  }
  else
  {
    fprintf(stderr, "Illegal datum: unknown keyword `%s'\n", keyword);
    exit(45);
  }
}

void SpecGroup::HandleLine(char *line)
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
  else if (ch == ' ' || ch == '\t')
  {
    TileDatum(line);
  }
  else
  {
    fprintf(stderr, "Unrecognised line:\n`%s'\n", line);
    exit(38);
  }
}


/* ---------------------------------------------------------------- */

SpecGroup * SpecGroup::Load(const Filename& fname)
{
  SpecGroup *group = new SpecGroup();

  FILE *fp = Z_File::Open(fname, "r");

  if (! fp)
  {
    fprintf(stderr, "Unable to open: %s\n", fname.str.c_str());
    exit(21);
  }

  fprintf(stderr, "Loading: %s\n", fname.str.c_str());

  for (;;)
  {
    char LineBuf[MAX_LINE];

    if (! fgets(LineBuf, MAX_LINE-2, fp))
      break;

    group->HandleLine(LineBuf);
  }

  Z_File::Close(fp);

  return group;
}


/* ---------------------------------------------------------------- */

/*
 * returns spec number that matches name, otherwise -1.  The given
 * name should already have been alias converted.
 */
int SpecGroup::LookupName(const char *name) const
{
  int hash;
  int index;

  if (! name)
    return -1;

  hash = T_Alias::Hash(string(name));

  index = hash_tab[hash];

  for (; index >= 0; index = specs[index].hash_next)
  {
    ASSERT(index < total);
    
    // FIXME: this is not very clean, as specs who names are set to
    //        NULL (or changed) should update the hash table.
    if (! specs[index].name)
      continue;

    if (T_Alias::FuzzyCmp(string(specs[index].name), string(name)) == 0)
    {
      return index;
    }
  }  

  /* not found */
  return -1;
}


//----------------------------------------------------------------------------

void SpecGroup::WriteSpec(FILE *fp, ModelSpec *spec, int id)
{
  ASSERT(spec);

  // handle the (rare) case of specs without ID and/or NAMES

  if (spec->spec_id >= 0)
    id = spec->spec_id;

  const char *name = "unknown";

  if (spec->name)
    name = spec->name;
  
  fprintf(fp, "# tile %d (%s)\n", id, name);
  fprintf(fp, "{\n");

  if (spec->model[0] && spec->model[0] != '-')
    fprintf(fp, "  model = %s\n", spec->model);

  if (spec->skin[0] && spec->skin[0] != '-')
    fprintf(fp, "  skin = %s\n", spec->skin);

  if (spec->material[0] && spec->material[0] != '-')
    fprintf(fp, "  material = %s\n", spec->material);
 
  if (spec->light[0] && spec->light[0] != '-')
    fprintf(fp, "  light = %s\n", spec->light);
 
  if (spec->scale != 1)
    fprintf(fp, "  scale = %f\n", spec->scale);
 
  if (spec->rotate != 0)
    fprintf(fp, "  rotate = %f\n", spec->rotate);
 
  if (spec->flags != 0)
  {
    int flag;

    fprintf(fp, "  flags = ");

    for (flag = 0; flag < 26; flag++)
    {
      if (spec->flags & (1 << flag))
        fprintf(fp, "%c", 'A' + flag);
    }

    fprintf(fp, "\n");
  }
 
  fprintf(fp, "}\n");
  fflush(fp);
}

void SpecGroup::Save(const Filename& fname)
{
  FILE *fp = Z_File::Open(fname, "w");

  if (! fp)
  {
    fprintf(stderr, "Unable to create: %s\n", fname.str.c_str());
    exit(81);
  }

  fprintf(stderr, "Saving: %s\n", fname.str.c_str());

  for (int t=0; t < total; t++)
  {
    if (skip_empties && ! specs[t].name)
      continue;

    WriteSpec(fp, specs + t, t);
  }

  Z_File::Close(fp);
}


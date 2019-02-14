//----------------------------------------------------------------------------
//  ALIAS handling
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

#include "t_alias.h"


#define MAX_ALIAS  1024
#define MAX_LINE   512

struct Alias
{
  public:

    string source;
    string dest;

    Alias() : source(""), dest("") { }
};

static Alias aliases[MAX_ALIAS];
static int num_alias = 0;


static void HandleLine(char *line)
{
  Alias *cur;

  int len = strlen(line);
  char *pos;

  while (len > 0 && isspace(line[len-1]))
    line[--len] = 0;
      
  if (len == 0)
    return;

  pos = strchr(line, '=');

  if (! pos)
  {
    fprintf(stderr, "Missing `=' in line:\n%s\n", line);
    exit(12);
  }

  *pos++ = 0;

  cur = aliases + num_alias;
  num_alias++;

  /* do source */

  len = strlen(line);

  while (len > 0 && isspace(line[len-1]))
    line[--len] = 0;
  
  ASSERT(len > 0);

  cur->source = string(line);
 
  /* do destination */

  while (*pos && isspace(*pos))
    pos++;

  ASSERT(strlen(pos) > 0);

  cur->dest = string(pos);

#if 1  /* DEBUGGING */
  fprintf(stderr, "ALIAS #%d\n", num_alias);
  fprintf(stderr, "`%s' -> `%s'\n", cur->source.c_str(), cur->dest.c_str());
#endif
}

void T_Alias::Load(const Filename& fname)
{
  FILE *fp = Z_File::Open(fname, "r");

  if (! fp)
  {
    fprintf(stderr, "Failed to open: %s\n", fname.str.c_str());
    exit(11);
  }

  fprintf(stderr, "Loading: %s\n", fname.str.c_str());

  char LineBuf[MAX_LINE];

  for (;;)
  {
    if (! fgets(LineBuf, MAX_LINE-2, fp))
      break;

    HandleLine(LineBuf);

    if (num_alias >= MAX_ALIAS)
    {
      fprintf(stderr, "Too many aliases !\n");
      exit(14);
    }
  }

  Z_File::Close(fp);
}

void T_Alias::Free(void)
{
  int i;

  for (i=0; i < num_alias; i++)
  {
    aliases[i].source = "";
    aliases[i].dest = "";
  }
}


/* ---------------------------------------------------------------- */

/* 
 * returns either the given string or alias-stored string.
 */
string T_Alias::Lookup(const string& name)
{
  int i;

  for (i=0; i < num_alias; i++)
  {
    if (FuzzyCmp(name, aliases[i].source) == 0)
    {
      return aliases[i].dest;
    }
  }

  return name;
}

#define ALIAS_CHAR(ch)  (! isspace(ch))

/*
 * checks case-insensitively, ignoring white-space too.
 */
int T_Alias::FuzzyCmp(const string& A, const string& B)
{
  string::const_iterator C = A.begin();
  string::const_iterator D = B.begin();

  for (;;)
  {
    if (C == A.end() || D == B.end())
      return 0;

    if (C == A.end())
      return -1;

    if (D == B.end())
      return +1;

    if (! ALIAS_CHAR(*C))
    {
      C++; continue;
    }

    if (! ALIAS_CHAR(*D))
    {
      D++; continue;
    }

    if (toupper(*C) != toupper(*D))
      break;

    C++; D++;
  }

  return toupper(*C) - toupper(*D);
}

int T_Alias::Hash(const string& name)
{
  string::const_iterator p;

  unsigned int hash = 0;

  for (p=name.begin(); p != name.end(); p++)
  {
    if (! ALIAS_CHAR(*p))
      continue;

    hash = hash * hash_mul + (unsigned int) toupper(*p);
  }

  return (int) (hash % hash_size);
}


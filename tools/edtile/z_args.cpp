//----------------------------------------------------------------------------
//  ARGS : command line arguments
//----------------------------------------------------------------------------
//
//  Z C++ Lib (C) 2001-2002 Andrew Apted
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

#include <iostream>


#ifdef WIN32
#define CHECK_DIV_CHAR(c)  ((c) == '/')
#else
#define CHECK_DIV_CHAR(c)  (false)
#endif


namespace Z_Args
{
  // --- private stuff here (not in header file) ---

  /* list of argument strings.
   *
   * Invariant 1: all options begin with a single `-'.  This means
   *     that options beginning with "--" (and under Win32, ones
   *     beginning with `/') are converted before being stored.
   */
  typedef vector<string> arg_vec_t;

  arg_vec_t arg_strs;
}


void Z_Args::Add(const char *name)
{
  // handle options starting with "--" (GNU style)
  while (name[0] == '-' && name[1] == '-')
    name++;

  // ignore empty arguments and switches
  
  if (name[0] == 0)
    return;
  
  if (name[1] == 0 && (name[0] == '-' || CHECK_DIV_CHAR(name[0])))
  {
    return;
  }

  string tmp = name;  // convert to string
   
  if (CHECK_DIV_CHAR(name[0]))
  {
    // convert `/' switches to `-' ones
    tmp[0] = '-';
  }

  arg_strs.push_back(tmp);
}

void Z_Args::AddMany(int argc, const char **argv)
{
  for (; argc > 0; argc--, argv++)
  {
    Add(*argv);
  }
}


int Z_Args::Size()
{ 
  return arg_strs.size();
}

string Z_Args::At(int index)
{
  ASSERT(0 <= index && index < Size());
  
  return arg_strs[index];
}

bool Z_Args::IsOption(int index)
{
  ASSERT(0 <= index && index < Size());
  
  return arg_strs[index][0] == '-';
}


bool Z_Args::FindFwd(const string& opt_str, int *index, 
    int start)
{
  ASSERT(start >= 0);

  // allow `start' values that are too big
  if (start >= Size())
    return false;

  arg_vec_t::const_iterator p = arg_strs.begin();

  p += start;

  for (; p != arg_strs.end(); p++, start++)
  {
    const string& cur = *p;

    if (cur.length() < 2)
      continue;
    
    if (cur[0] != '-')
      continue;
 
    // -AJA- Should use standard string::compare() routine, BUT gcc
    //       headers (V2.95) don't match the C++ book.
    //
    if (Z_String::CaseCmp(cur.c_str() + 1, opt_str.c_str()) == 0)
    {
      if (index)
        *index = start;

      return true;
    }
  }

  return false;
}
 
string Z_Args::CheckOption(const string& opt_str)
{
  int index;
  
  if (! FindFwd(opt_str, &index))
    return "";

  // very last argument (hence no value) ?
  if (index == Size()-1)
    return "";

  // next argument is another option ?
  if (IsOption(index+1))
    return "";

  return At(index+1);
}
 
void Z_Args::CheckSwitch(const string& switch_str, bool *var, 
    bool reverse)
{
  if (FindFwd(switch_str))
  {
    *var = !reverse;
    return;
  }

  string no_switch = "no" + switch_str;

  if (FindFwd(no_switch))
  {
    *var = reverse;
    return;
  }
}


#ifndef NDEBUG

void Z_Args::TestShow()
{
  cout << "\n---ARGUMENT-SHOW---------------\n";
  cout << "Size = " << Size() << "\n";

  int index;
  
  for (index=0; index < Size(); index++)
  {
    cout << "[" << index << "]" << " = `" << At(index) << "'\n";
  }

  cout << "-------------------------------\n\n";
}

void Z_Args::TestFind()
{
  cout << "\n---ARGUMENT-FINDING------------\n";

  string test1 = CheckOption("test1");
  
  if (test1.size() > 0)
    cout << "Found -test1 with par = `" << test1 << "'\n";
  else
    cout << "Did not find -test1 option\n";
  
  bool test2 = false;
  bool test3 = true;
  
  CheckSwitch("test2", &test2);
  CheckSwitch("test3", &test3);

  cout << "Test2 switch = " << test2 << "\n";
  cout << "Test3 switch = " << test3 << "\n";
  
  cout << "-------------------------------\n\n";
}

#endif  // NDEBUG


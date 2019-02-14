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

#ifndef __Z_ARGS_H__
#define __Z_ARGS_H__

#include <string>
#include <vector>


namespace Z_Args
{
  /* ---- setup routines ---- */
  
  void Add(const char *name);
  void Add(const string& name);
  // add a single argument.  Names that start with `-' (or `/' in
  // Win32) will be treated as switches/options.

  void AddMany(int argc, const char **argv);
  // add a bunch of arguments, typically from the main() function.
  // Should not include the program's name.

  /* ---- normal query routines ---- */
   
  int Size();
  // get number of arguments

  string At(int index);
  // query the argument at the given index (0 to Size()-1).
   
  bool IsOption(int index);
  // returns true if the specified argument is an option or switch
  // (i.e. starts with `-' or (for Win32) '/').  Typically used to
  // find the filename(s) that occur before the options start.
   
  bool FindFwd(const string& opt_str, int *index = 0, int start = 0);
  // looks for the option string in the set of options.  If it exists,
  // sets `index' and returns true, otherwise returns false (and
  // `index' is unchanged).  Searching occurs fowards.  The start
  // position can be specified.  The option string should not include
  // any initial `-' or `/' chars.  The index pointer can be zero.
   
  string CheckOption(const string& opt_str);
  // if the given option exists in the arguments, then return the
  // following argument (the value of the option).  Returns the
  // empty string if the option doesn't exist or the next argument
  // wasn't valid.  The option string should not include any initial
  // `-' or `/' chars.
   
  void CheckSwitch(const string& switch_str, bool *var, 
      bool reverse = false);
  // if the given switch exists in the arguments, the boolean
  // variable is set to true.  If the user added a "no" prefix, the
  // variable is set to false instead.  The `reverse' flag can
  // negate the result, in case the variable name is opposite to what
  // the argument name suggests.  The switch string should not include
  // any initial `-' or `/' chars.

  /* ---- test/debug routines ---- */
   
  void TestShow();
  // print to stdout the current set of arguments.

  void TestFind();
  // check option finding routines, using "-test1 XXX" option and
  // "-test2" switch.
}

inline void Z_Args::Add(const string& name)
{ 
  Add(name.c_str()); 
}


#endif  /* __Z_ARGS_H__ */

//----------------------------------------------------------------------------
//  ERROR and debugging
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

#ifndef __Z_ERROR_H__
#define __Z_ERROR_H__


namespace Z_Error
{
  void Fatal(const char *str, ...);
  // called when something really serious (& unrecoverable) happens.
  // Should show the message to the user and abort the program.

  void Debug(const char *str, ...);
  // display some debugging information, which typically gets written
  // into a debug file, or discarded if debugging is disabled.
}


//
//  ASSERTING ONESELF...
//  

#ifndef __GNUC__
#define __PRETTY_FUNCTION__  "<Unknown>"
#endif


#ifdef NDEBUG

#define ASSERT(x)  do {} while (0)
#define ARG_CHECK(str,x)  do {} while (0)
#define INVARIANT(str,x)  do {} while (0)

#else

#define ASSERT(x)  \
    do { if (! (x)) {  \
         Z_Error::Fatal("Assertion failed --> %s:%d, %s\n",  \
            __FILE__, __LINE__, __PRETTY_FUNCTION__);  \
    }} while(0)

#define ARG_CHECK(str,x)  \
    do { if (! (x)) {  \
         Z_Error::Fatal("Bad arg %s --> %s:%d, %s\n",  \
             str, __FILE__, __LINE__, __PRETTY_FUNCTION__);  \
    }} while(0)

#define INVARIANT(str,x)  \
    do { if (! (x)) {  \
         Z_Error::Fatal("Violated: %s --> %s:%d, %s\n",  \
             str, __FILE__, __LINE__, __PRETTY_FUNCTION__);  \
    }} while(0)

#endif  // NDEBUG

    
#endif /* __Z_DEBUG_H__ */

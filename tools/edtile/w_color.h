//------------------------------------------------------------------------
//  COLOR for widgets
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

#ifndef __W_COLOR_H__
#define __W_COLOR_H__


/* intensity ranges from 0 to 255.
 * percent ranges from 0 to 100.
 */
#define I_GREY(intensity)  \
    fl_gray_ramp(FL_NUM_GRAY * (intensity) / 256)

#define P_GREY(perc)  \
    fl_gray_ramp(FL_NUM_GRAY * (perc) / 101)


#endif  /* __W_COLOR_H__ */

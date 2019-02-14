//------------------------------------------------------------------------
//  EDTILE Program
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

using namespace std;

#include "z_config.h"

#include <FL/Fl_File_Icon.H>

#include "z_args.h"
#include "z_string.h"

#include "w_main.h"
#include "w_browse.h"
#include "w_compact.h"
#include "w_edit.h"
#include "w_license.h"
#include "w_shades.h"
#include "z_hsv.h"
#include "t_colmap.h"
#include "t_hue.h"


static Tileset *work_set;


void DoSave()
{
  if (! work_set)
    return;

  work_set->Save("/tmp/out.txt");
}

static void ShowInfo()
{
  fprintf(stdout,
      "\n**** " PACKAGE_TITLE " " VERSION " ****\n\n"
      "Copyright (C) 2001-2002 Andrew Apted.\n"
      "Under the GNU General Public License (GPL).\n\n"
      "USAGE: edtile filename.txt\n\n"
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
    fprintf(stderr, "USAGE: edtile filename.txt\n");
    exit(2);
  }
 

//// set defaults
//// MainSetDefaults();

  Z_Args::AddMany(argc - 1, argv + 1);
   
  // enable "as good as possible" RGB mode
  Fl::visual(FL_RGB);

  // load icons for file chooser
  Fl_File_Icon::load_system_icons();


  MainWin::appwin = new MainWin(0, 0,
      700 /* MainWin::min_w */, MainWin::min_h,
      PACKAGE_TITLE " " VERSION);


  T_Colmap::cmap16 = ColorMapping::Load("glhack/win/share/colors16.list");
  T_Colmap::cmap32 = ColorMapping::Load("glhack/win/share/colors32.list");


  work_set = Tileset::Load(Filename(argv[1]));

  work_set->cmap->trans_col = work_set->cmap->LookupExact(TRANS_COL);


  BrowserTab *btab = new BrowserTab();
  btab->ChangeTiles(work_set);
  btab->ChangeZoom(1);
  MainWin::appwin->AddTab(btab);


  CompactTab *ctab = new CompactTab();
  ctab->ChangeTiles(work_set);
  ctab->ChangeZoom(1);
  MainWin::appwin->AddTab(ctab);


  EditorTab *edtab = new EditorTab();
  edtab->ChangeTile(work_set, work_set->tiles + 0);
  MainWin::appwin->AddTab(edtab);


  HueSet *hue_set = new HueSet(T_Colmap::cmap32);
 

  ShadeTab *shtab = new ShadeTab();
  shtab->ChangeMap(hue_set);
  MainWin::appwin->AddTab(shtab);

 
  MainWin::appwin->ChangeTab(ctab);
  MainWin::appwin->RunModal();

  delete MainWin::appwin;


  return 0;
}

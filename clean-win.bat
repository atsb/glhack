echo SPOTLESS CLEANING
cd src
rm -f *.o hack.h-t config.h-t
rm -f a.out core glhack Sys*
rm -f ../include/date.h ../include/onames.h ../include/pm.h
rm -f monstr.c ../include/vis_tab.h vis_tab.c tile.c *.moc
cd ../util
rm -f *.o
rm -f lev_lex.c lev_yacc.c dgn_lex.c dgn_yacc.c
rm -f ../include/lev_comp.h ../include/dgn_comp.h
rm -f ../include/tile.h
rm -f makedefs lev_comp dgn_comp recover_glhack dlb
rm -f gif2txt txt2ppm tile2x11 tile2img.ttp xpm2img.ttp tilemap
cd ../dat
rm -f spec_levs quest_levs *.lev data rumors quest.dat oracles options dungeon dungeon.pdf
rm -f nhdat x11tiles beostiles pet_mark.xbm rip.xpm mapbg.xpm
rm -f rip.img GEM_RSC.RSC title.img nh16.img
cd ../doc
rm -f Guidebook.aux Guidebook.log
rm -f Guidebook Guidebook.ps Guidebook.dvi
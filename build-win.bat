set GAME_INSTALLATION_DIR = glhack13/bin
set DATA_INSTALLATION_DIR = glhack13/bin
set EXEC_INSTALLATION_DIR = glhack13
set DOCS_INSTALLATION_DIR = glhack13/docs

echo MAKING MAKEDEFS
cd src
touch ../src/config.h-t
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c monst.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c objects.c
cd ../util
gcc -O -I../include -c -o makedefs.o makedefs.c
gcc -o makedefs makedefs.o ../src/monst.o ../src/objects.o
makedefs.exe -p
makedefs.exe -o
touch ../src/hack.h-t
cd ../src
echo MAKING SOURCE
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o allmain.o allmain.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o alloc.o alloc.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o apply.o apply.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o artifact.o artifact.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o attrib.o attrib.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o ball.o ball.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o bones.o bones.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o botl.o botl.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o cmd.o cmd.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o dbridge.o dbridge.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o decl.o decl.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o detect.o detect.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o dig.o dig.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o display.o display.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o dlb.o dlb.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o do.o do.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o do_name.o do_name.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o do_wear.o do_wear.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o dog.o dog.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o dogmove.o dogmove.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o dokick.o dokick.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o dothrow.o dothrow.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o drawing.o drawing.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o dungeon.o dungeon.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o eat.o eat.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o end.o end.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o engrave.o engrave.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o exper.o exper.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o explode.o explode.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o extralev.o extralev.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o files.o files.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o fountain.o fountain.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o hack.o hack.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o hacklib.o hacklib.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o invent.o invent.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o light.o light.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o lock.o lock.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o mail.o mail.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o makemon.o makemon.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o mapglyph.o mapglyph.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o mcastu.o mcastu.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o mhitm.o mhitm.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o mhitu.o mhitu.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o minion.o minion.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o mklev.o mklev.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o mkmap.o mkmap.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o mkmaze.o mkmaze.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o mkobj.o mkobj.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o mkroom.o mkroom.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o mon.o mon.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o mondata.o mondata.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o monmove.o monmove.c
cd ../util
makedefs.exe -m
cd ../src
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o monstr.o monstr.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o mplayer.o mplayer.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o mthrowu.o mthrowu.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o muse.o muse.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o music.o music.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o o_init.o o_init.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o objnam.o objnam.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o options.o options.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o pager.o pager.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o pickup.o pickup.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o pline.o pline.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o polyself.o polyself.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o potion.o potion.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o pray.o pray.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o priest.o priest.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o quest.o quest.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o questpgr.o questpgr.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o read.o read.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o rect.o rect.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o region.o region.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o restore.o restore.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o rip.o rip.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o rnd.o rnd.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o role.o role.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o rumors.o rumors.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o save.o save.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o shk.o shk.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o shknam.o shknam.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o sit.o sit.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o sounds.o sounds.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o sp_lev.o sp_lev.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o spell.o spell.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o steal.o steal.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o steed.o steed.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o teleport.o teleport.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o timeout.o timeout.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o topten.o topten.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o track.o track.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o trap.o trap.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o u_init.o u_init.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o uhitm.o uhitm.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o vault.o vault.c
cd ../util
makedefs.exe -z
cd ../src
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o vision.o vision.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o vis_tab.o vis_tab.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o weapon.o weapon.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o were.o were.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o wield.o wield.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o windows.o windows.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o wizard.o wizard.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o worm.o worm.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o worn.o worn.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o write.o write.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o zap.o zap.c
echo MAKING GL FRONTEND
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c ../sys/winnt/ntsound.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c ../sys/share/random.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c ../sys/winnt/winnt.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c ../sys/share/pcunix.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c ../sys/share/pcsys.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c ../sys/share/pcmain.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c ../win/gl/gl_3d.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c ../win/gl/gl_conf.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c ../win/gl/gl_emul.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c ../win/gl/gl_font.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c ../win/gl/gl_hardw.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c ../win/gl/gl_image.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c ../win/gl/gl_main.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c ../win/gl/gl_map.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c ../win/gl/gl_menu.c
cd ../util
gcc -O -I../include -o tilemap ../win/share/tilemap.c 
tilemap.exe
makedefs.exe -v
cd ../src
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c ../win/gl/gl_opt.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c ../win/gl/gl_rendu.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c ../win/gl/gl_role.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c ../win/gl/gl_softw.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c ../win/gl/gl_stat.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c ../win/gl/gl_text.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c ../win/gl/gl_tile.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c ../win/gl/gl_unit.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c ../win/gl/gl_win.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o tile.o tile.c
gcc -O -I../include -D_GNU_SOURCE=1 -D_REENTRANT -c -o version.o version.c
echo LINKING OBJECTS
gcc -o glhack monst.o objects.o allmain.o alloc.o apply.o artifact.o attrib.o ball.o bones.o botl.o cmd.o dbridge.o decl.o detect.o dig.o display.o dlb.o do.o do_name.o do_wear.o dog.o dogmove.o dokick.o dothrow.o drawing.o dungeon.o eat.o end.o engrave.o exper.o explode.o extralev.o files.o fountain.o hack.o hacklib.o invent.o light.o lock.o mail.o makemon.o mapglyph.o mcastu.o mhitm.o mhitu.o minion.o mklev.o mkmap.o mkmaze.o mkobj.o mkroom.o mon.o mondata.o monmove.o monstr.o mplayer.o mthrowu.o muse.o music.o o_init.o objnam.o options.o pager.o pickup.o pline.o polyself.o potion.o pray.o priest.o quest.o questpgr.o read.o rect.o region.o restore.o rip.o rnd.o role.o rumors.o save.o shk.o shknam.o sit.o sounds.o sp_lev.o spell.o steal.o steed.o teleport.o timeout.o topten.o track.o trap.o u_init.o uhitm.o vault.o vision.o vis_tab.o weapon.o were.o wield.o windows.o wizard.o worm.o worn.o write.o zap.o ntsound.o random.o winnt.o pcunix.o pcsys.o pcmain.o gl_3d.o gl_conf.o gl_emul.o gl_font.o gl_hardw.o gl_image.o gl_main.o gl_map.o gl_menu.o gl_opt.o gl_rendu.o gl_role.o gl_softw.o gl_stat.o gl_text.o gl_tile.o gl_unit.o gl_win.o tile.o version.o -lpng -lz -lSDL -lopengl32 -lwinmm
echo COMPILATION OF SOURCECODE FINISHED
echo MAKING UTILITIES
cd ../util
gcc -O -I../include -c -o recover.o recover.c
gcc -o recover_glhack recover.o 
cd ../dat
echo MAKING DATA
../util/makedefs.exe -d
echo MAKING ORACLES
../util/makedefs.exe -h
echo MAKING QUEST
../util/makedefs.exe -q
echo MAKING RUMORS
../util/makedefs.exe -r
echo LEXING
cd ../util
bison -y -d dgn_comp.y
mv y.tab.c dgn_yacc.c
mv y.tab.h ../include/dgn_comp.h
gcc -O -I../include -c -o dgn_yacc.o dgn_yacc.c
flex dgn_comp.l
mv lex.yy.c dgn_lex.c
gcc -c -O -I../include dgn_lex.c
gcc -O -I../include -c -o dgn_main.o dgn_main.c
gcc -O -I../include -c -o panic.o panic.c
gcc  -o dgn_comp dgn_yacc.o dgn_lex.o dgn_main.o ../src/alloc.o panic.o 
cd ../dat
echo MAKING DUNGEON
../util/makedefs.exe -e
../util/dgn_comp.exe dungeon.pdf
cd ../util
bison -y -d lev_comp.y
mv y.tab.c lev_yacc.c
mv y.tab.h ../include/lev_comp.h
gcc -O -I../include -c -o lev_yacc.o lev_yacc.c
flex lev_comp.l
mv lex.yy.c lev_lex.c
gcc -c -O -I../include lev_lex.c
gcc -O -I../include -c -o lev_main.o lev_main.c
gcc -o lev_comp lev_yacc.o lev_lex.o lev_main.o ../src/alloc.o panic.o ../src/drawing.o ../src/decl.o ../src/monst.o ../src/objects.o 
cd ../dat
../lev_comp.exe bigroom.des
../lev_comp.exe castle.des
../lev_comp.exe endgame.des
../lev_comp.exe gehennom.des
../lev_comp.exe knox.des
../lev_comp.exe medusa.des
../lev_comp.exe mines.des
../lev_comp.exe oracle.des
../lev_comp.exe sokoban.des
../lev_comp.exe tower.des
../lev_comp.exe yendor.des
touch spec_levs
../lev_comp.exe Arch.des
../lev_comp.exe Barb.des
../lev_comp.exe Caveman.des
../lev_comp.exe Healer.des
../lev_comp.exe Knight.des
../lev_comp.exe Monk.des
../lev_comp.exe Priest.des
../lev_comp.exe Ranger.des
../lev_comp.exe Rogue.des
../lev_comp.exe Samurai.des
../lev_comp.exe Tourist.des
../lev_comp.exe Valkyrie.des
../lev_comp.exe Wizard.des
touch quest_levs
cd ../util
echo MAKING DLB
gcc -O -I../include -c dlb_main.c
gcc -o dlb dlb_main.o ../src/dlb.o ../src/alloc.o panic.o
cd ../dat
../util/dlb.exe cf nhdat help.txt hh.txt cmdhelp.txt history.txt opthelp.txt wizhelp.txt dungeon asmodeus.lev baalz.lev bigrm-?.lev castle.lev fakewiz?.lev juiblex.lev knox.lev medusa-?.lev minend-?.lev minefill.lev minetn-?.lev oracle.lev orcus.lev sanctum.lev soko?-?.lev tower?.lev valley.lev wizard?.lev astral.lev air.lev earth.lev fire.lev water.lev ???-goal.lev ???-fil?.lev ???-loca.lev ???-strt.lev data oracles.txt ttyoptions.txt quest.dat rumors
cd ..
echo CREATING INSTALLATION DESTINATIONS 
mkdir glhack13
cd glhack13
mkdir bin
mkdir docs
cd ..
cd dat
echo ADDING dat/options
sed -n -e '/librarian/{' -e	's/.*/dlb/p' -e	'q' -e '}' -e 's/.*/nodlb/p' < ttyoptions.txt
../util/dlb.exe cf nhdat help.txt hh.txt cmdhelp.txt history.txt opthelp.txt wizhelp.txt dungeon asmodeus.lev baalz.lev bigrm-?.lev castle.lev fakewiz?.lev juiblex.lev knox.lev medusa-?.lev minend-?.lev minefill.lev minetn-?.lev oracle.lev orcus.lev sanctum.lev soko?-?.lev tower?.lev valley.lev wizard?.lev astral.lev air.lev earth.lev fire.lev water.lev ???-goal.lev ???-fil?.lev ???-loca.lev ???-strt.lev data oracles.txt ttyoptions.txt quest.dat rumors
cp nhdat license glfont8.png glfont14.png glfont20.png glfont22.png gltile16.png gltile32.png gltile64.png gllogo.png glrip.png glface16.lst glface32.lst ../glhack13/bin
cd ..
echo INSTALLING GAME
cp src/glhack.exe glhack13/bin
cp util/recover_glhack.exe glhack13/bin
cp win/gl/glhack.sh glhack13
touch glhack13/bin/perm
touch glhack13/bin/record
touch glhack13/bin/logfile
cp doc/glhack.6 glhack13/docs
cp doc/glhackrc.5 glhack13/docs
cp doc/nethack-guidebook.6 glhack13/docs
cp doc/nethack.6 glhack13/docs
cp doc/lev_comp.6 glhack13/docs
cp doc/dgn_comp.6 glhack13/docs
cp doc/recover.6 glhack13/docs
cp doc/dlb.6 glhack13/docs
echo GLHACK IS INSTALLED!
#
# EdTile
#
# Requires GNU make.
#

# --- variables that can be overridden ---

# compilers
CC=gcc
CXX=g++

# optimisation
OPTIM=-O -g


# --- internal stuff from here on ---

X11_LIBS=-L/usr/X11R6/lib

FLTK_FLAGS := $(shell fltk-config --cxxflags)
FLTK_PREFIX := $(shell fltk-config --prefix)
FLTK_LIBS := -lfltk_images $(shell fltk-config --ldflags)  -lpng -ljpeg

CPPFLAGS=-I../../include
CFLAGS=-Wall $(OPTIM)
CXXFLAGS=-Wall $(OPTIM) $(FLTK_FLAGS)

LIBS=-lm $(FLTK_LIBS)
LDFLAGS=-O
# LDFLAGS=-O -Wl,-warn-common

T_OBJS=t_alias.o  t_colmap.o  t_hue.o  t_pair.o  t_tiles.o

W_OBJS=w_base.o  w_browse.o  w_compact.o  w_edit.o  w_license.o  \
       w_main.o  w_panel.o   w_shades.o   w_tab.o   w_tlinfo.o   \
       w_webbut.o

Z_OBJS=z_args.o  z_error.o  z_file.o  z_math.o  z_hsv.o  z_rgb.o  \
       z_string.o

PROGRAMS=edtile  makeset  renumber  loadppm  saveppm  samecmap


# ----- rules ----------------------------------------

all:    $(PROGRAMS)

clean: imgclean
	rm -f $(PROGRAMS) core *.o

halfclean: imgclean
	rm -f $(PROGRAMS) core [^z]*.o

imgclean:
	rm -f gl[tf]*.ppm gl[tf]*.png btile*.raw

tilesets: makeset
	./makeset ../../win/share/
	./pngify.sh

.PHONY: all clean halfclean imgclean tilesets

.DELETE_ON_ERROR:

%.o: %.c
	@echo -e "\nCompiling $@\n"
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	@echo -e "\nCompiling $@\n"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


# ----- programs ----------------------------------------

ED_OBJS=edtile.o  $(Z_OBJS)  $(T_OBJS)  $(W_OBJS)

edtile: $(ED_OBJS)
	@echo -e "\nLinking $@\n"
	$(CXX) $(LDFLAGS) -o $@ $(ED_OBJS) $(LIBS)


MK_OBJS=makeset.o  $(Z_OBJS)  $(T_OBJS)

makeset: $(MK_OBJS)
	@echo -e "\nLinking $@\n"
	$(CXX) $(LDFLAGS) -o $@ $(MK_OBJS) $(LIBS)


BS_OBJS=buildspec.o  $(Z_OBJS)  $(T_OBJS)  t_specs.o

buildspec: $(BS_OBJS)
	@echo -e "\nLinking $@\n"
	$(CXX) $(LDFLAGS) -o $@ $(BS_OBJS) $(LIBS)


MD2_OBJS=create_md2.o

create_md2: $(MD2_OBJS)
	@echo -e "\nLinking $@\n"
	$(CC) $(LDFLAGS) -o $@ $(MD2_OBJS) -lm


IM_OBJS=import.o  $(Z_OBJS)  $(T_OBJS)

import: $(IM_OBJS)
	@echo -e "\nLinking $@\n"
	$(CXX) $(LDFLAGS) -o $@ $(IM_OBJS) $(LIBS)

IM3D_OBJS=import3D.o  $(Z_OBJS)  $(T_OBJS)

import3D: $(IM3D_OBJS)
	@echo -e "\nLinking $@\n"
	$(CXX) $(LDFLAGS) -o $@ $(IM3D_OBJS) $(LIBS)


SAME_OBJS=samecmap.o  $(Z_OBJS)  $(T_OBJS)

samecmap: $(SAME_OBJS)
	@echo -e "\nLinking $@\n"
	$(CXX) $(LDFLAGS) -o $@ $(SAME_OBJS) $(LIBS)


LPPM_OBJS=loadppm.o  $(Z_OBJS)  $(T_OBJS)

loadppm: $(LPPM_OBJS)
	@echo -e "\nLinking $@\n"
	$(CXX) $(LDFLAGS) -o $@ $(LPPM_OBJS) $(LIBS)


SPPM_OBJS=saveppm.o  $(Z_OBJS)  $(T_OBJS)

saveppm: $(SPPM_OBJS)
	@echo -e "\nLinking $@\n"
	$(CXX) $(LDFLAGS) -o $@ $(SPPM_OBJS) $(LIBS)


SJPEG_OBJS=savejpeg.o  $(Z_OBJS)  $(T_OBJS)

savejpeg: $(SJPEG_OBJS)
	@echo -e "\nLinking $@\n"
	$(CXX) $(LDFLAGS) -o $@ $(SJPEG_OBJS) $(LIBS)


SRAW_OBJS=storeraw.o  $(Z_OBJS)  $(T_OBJS)

storeraw: $(SRAW_OBJS)
	@echo -e "\nLinking $@\n"
	$(CXX) $(LDFLAGS) -o $@ $(SRAW_OBJS) $(LIBS)


RN_OBJS=renumber.o  $(Z_OBJS)  $(T_OBJS)

renumber: $(RN_OBJS)
	@echo -e "\nLinking $@\n"
	$(CXX) $(LDFLAGS) -o $@ $(RN_OBJS) $(LIBS)


EXPL_OBJS=explode.o  $(Z_OBJS)  $(T_OBJS)

explode: $(EXPL_OBJS)
	@echo -e "\nLinking $@\n"
	$(CXX) $(LDFLAGS) -o $@ $(EXPL_OBJS) $(LIBS)


HSV_OBJS=conv2hsv.o  $(Z_OBJS)

conv2hsv: $(HSV_OBJS)
	@echo -e "\nLinking $@\n"
	$(CXX) $(LDFLAGS) -o $@ $(HSV_OBJS) $(LIBS)

RGB_OBJS=conv2rgb.o  $(Z_OBJS)

conv2rgb: $(RGB_OBJS)
	@echo -e "\nLinking $@\n"
	$(CXX) $(LDFLAGS) -o $@ $(RGB_OBJS) $(LIBS)


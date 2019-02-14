#!/bin/sh
#	SCCS Id: @(#)nethack.sh	3.3	90/02/26

HACKDIR=$PWD/bin
export HACKDIR
HACK=$HACKDIR/glhack

# -AJA- removed HACKPAGER stuff, unused in glHack.
# 
# Note: the above HACKDIR directory gets modified to the actual
#       directory by the top-level Makefile during installation.

cd $HACKDIR
exec $HACK "$@"

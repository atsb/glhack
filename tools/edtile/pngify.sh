#!/bin/sh

echo
echo "Converting 16x16 tiles..."
echo
pnmtopng -verbose -compression 9 -background '#000000' gltile16.ppm > gltile16.png

echo
echo "Converting 32x32 tiles..."
echo
pnmtopng -verbose -compression 9 -background '#000000' -transparent '#00FFFF' gltile32.ppm > gltile32.png

echo
echo "Converting 48x64 (3D) tiles..."
echo
pnmtopng -verbose -compression 9 -background '#000000' -transparent '#00FFFF' gltile64.ppm > gltile64.png

# -- SLASH'EM --
# echo
# echo Converting 3D tiles...
# echo
# pnmtopng -verbose -compression 9 -background '#000000' -transparent '#00FFFF' gltile3d.ppm > gltile3d.png

echo
echo "Converting 8x8 font..."
echo
pnmtopng -verbose -compression 9 -background '#000000' -transparent '#000000' -force glfont8.ppm > glfont8.png

echo
echo "Converting 8x14 font..."
echo
pnmtopng -verbose -compression 9 -background '#000000' -transparent '#000000' -force glfont14.ppm > glfont14.png

echo
echo "Converting 10x20 font..."
echo
pnmtopng -verbose -compression 9 -background '#000000' -transparent '#000000' -force glfont20.ppm > glfont20.png

echo
echo "Converting 12x22 font..."
echo
pnmtopng -verbose -compression 9 -background '#000000' -transparent '#000000' -force glfont22.ppm > glfont22.png


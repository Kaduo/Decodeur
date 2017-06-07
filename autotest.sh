#!/bin/bash

IMG="images"
EXEC="bin/jpeg2ppm"

# Compilation

make clean > /dev/null 2>&1
make > /dev/null 2>&1

# Suppression des .ppm et .pgm

rm $IMG/*.pgm 2> /dev/null
rm $IMG/*.ppm 2> /dev/null

# Lancement des tests

echo "== invader.jpeg =="
time $EXEC $IMG/invader.jpeg > /dev/null 2>&1
echo -e

echo "== gris.jpg =="
time $EXEC $IMG/gris.jpg > /dev/null 2>&1
echo -e

echo "== bisou.jpeg =="
time $EXEC $IMG/bisou.jpeg > /dev/null 2>&1
echo -e

echo "== albert.jpg =="
time $EXEC $IMG/albert.jpg > /dev/null 2>&1
echo -e

echo "== zig-zag.jpg =="
time $EXEC $IMG/zig-zag.jpg > /dev/null 2>&1
echo -e

echo "== unicorn.jpg =="
time $EXEC $IMG/unicorn.jpg > /dev/null 2>&1
echo -e

echo "== zig-zag.jpg =="
time $EXEC $IMG/zig-zag.jpg > /dev/null 2>&1
echo -e

echo "== thumbs.jpg =="
time $EXEC $IMG/thumbs.jpg > /dev/null 2>&1
echo -e

echo "== horizontal.jpg =="
time $EXEC $IMG/horizontal.jpg > /dev/null 2>&1
echo -e

echo "== vertical.jpg =="
time $EXEC $IMG/vertical.jpg > /dev/null 2>&1
echo -e

echo "== shaun_the_sheep.jpeg =="
time $EXEC $IMG/shaun_the_sheep.jpeg > /dev/null 2>&1
echo -e

echo "== complexite.jpeg =="
time $EXEC $IMG/complexite.jpeg > /dev/null 2>&1

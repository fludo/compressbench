#!/bin/sh

echo "Cleaning everything..."
make mrproper
rm -f *.bin
rm -f *.huf
rm -f *.uhuf
rm  -f testIO
rm -f *.mtf
rm -f *.umtf
rm -f *.*~
rm -f *.ubh
rm -f *.bh
rm -f *.bwt
rm -f *.ubwt
rm -f *.rle
rm -f *.urle
echo "Clean !"

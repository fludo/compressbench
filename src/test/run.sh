#!/bin/sh

echo "Building Tests..."
make mrproper
make
echo "__________________________________________"
echo "Running Tests.."
echo 
echo
./testHNode
echo "------------------------------------------"
echo 
echo
./testHuffman
echo "------------------------------------------"
echo 
echo
./testBinaryIO
echo "------------------------------------------"
echo 
echo
./testMTF
echo "------------------------------------------"
echo
echo
./testBWT
echo "------------------------------------------"
echo
echo
./testRLE
echo "__________________________________________"

echo "All test have been run"

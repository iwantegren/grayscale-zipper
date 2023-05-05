#!/bin/bash

# compile shared library
mkdir -p out/lib/
cd out/lib/

# remove old library to prevent running in case of build errors
rm libgrayscalezipper.so
cmake -S ../../src/ZipperLibrary -B .
sudo make install
cd -

# compile project
mkdir -p out/build/
cd out/build/

# remove old executable to prevent running in case of build errors
rm grayscale-zipper

cmake -S ../../ -B .
make

# run compiled executable
./grayscale-zipper ../../res
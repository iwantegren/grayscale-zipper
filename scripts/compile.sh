#!/bin/bash

# compile shared library
cd ../out/lib/
cmake -S ../../src/ZipperLibrary -B .
sudo make install

# compile project
# cd ../build
# cmake -S ../../ -B .
# make
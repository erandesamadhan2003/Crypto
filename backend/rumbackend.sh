#!/bin/bash

rm -rf build
mkdir build
cd build
cmake ..
make 
echo "Build Successful"

# Run from project root instead of build directory
cd ..
./build/Crypto
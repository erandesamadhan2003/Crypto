#!/bin/bash

rm -rf build
mkdir build
cd build
cmake ..
make 
echo "Build Successful"
echo 

cd ..
./build/Crypto
#!/bin/bash

rm build/ -rf
mkdir build
cmake -Bbuild -H. -DCMAKE_BUILD_TYPE=Debug
cd build
make -j9
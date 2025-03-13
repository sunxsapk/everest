#!/bin/bash

set -e

if ! [ -d build/bin/everest-engine ]; then
    cmake -B build -DCMAKE_BUILD_TYPE=Release
fi

echo "Building Everest Engine ..."
cmake --build build/
echo "Installing Everest Engine ..."
cmake --install build/
echo "Installed Everest Engine successfully ..."


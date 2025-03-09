#!/bin/bash

set -e

if ! [ -d build/bin/everest-engine ]; then
    cmake -B build -DBuild=Debug -DINSTALL_ASSETS=ON
fi

echo "Building Everest Engine ..."
cmake --build build/
echo "Installing Everest Engine ..."
sudo cmake --install build/
echo "Installed Everest Engine successfully ..."


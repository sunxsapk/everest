#!/bin/bash

if ! [ -d build/ ]; then
    cmake -B build/
fi

echo ""

if cmake --build build/; then
    echo "----------- Build Successful ------------"
    ./build/bin/towerdefense/tower-defense
else
    echo "-x-x-x-x-x-x Build failed x-x-x-x-x-x-"
fi

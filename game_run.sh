#!/bin/bash

if ! [ -d build/ ]; then
    cmake -B build -DBuild=Debug -DTarget=Game
fi

echo ""

if cmake --build build/; then
    echo "----------- Build Successful ------------"
    ./build/bin/tower-defense/tower-defense
else
    echo "-x-x-x-x-x-x Build failed x-x-x-x-x-x-"
fi

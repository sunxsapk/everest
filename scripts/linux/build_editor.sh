#!/bin/bash

if ! [ -d build/ ]; then
    cmake -B build -DBuild=Release
fi

if cmake --build build/; then
    echo "----------- Build Successful ------------"
    # ./build/bin/everest-editor/everest-editor
else
    echo "-x-x-x-x-x-x Build failed x-x-x-x-x-x-"
fi



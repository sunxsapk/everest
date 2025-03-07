#!/bin/bash

if ! [ -d build/bin/examples ]; then
    cmake -B build -DBuild=Release -DTarget=Game
fi

if cmake --build build/; then
    echo "----------- Build Successful ------------"
    # ./build/bin/examples/examples
else
    echo "-x-x-x-x-x-x Build failed x-x-x-x-x-x-"
fi



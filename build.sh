#!/bin/bash

if cmake --build build/; then
    echo "xxxxxxxxxxx Build Successful xxxxxxxxxxxx"
    #./build/bin/towerdefense/tower-defense
else
    echo "xxxxxxxxxxx Build failed xxxxxxxxxxx"
fi

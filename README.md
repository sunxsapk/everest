# Everest

__Everest__ is an ongoing cross platform game engine project written in C/C++.


## Setup and Running Project


Ensure you've got the dependencies installed.

1. Compiler : C/C++ minimum standard required : c++17

2. `cmake`  : Project file builder for different editors and platforms

3. `glfw`   : Graphics Library framework for opengl
    - Linux     : `sudo apt-get install libglfw3 libglfw3-dev`
    - MacOS     : `brew install glfw`
    - Windows   : ___Download precompiled binaries or build them yourself. Consider following link: [glfw](https://www.glfw.org/download)___

4. Lua      : Scripting language for engine
    - Linux     : `sudo apt install lua5.4`
    - MacOS     : `brew install lua`
    - Windows   : use vcpkg `vcpkg install lua`
    - __If any issues: you can always download precompiled binaries or build [lua](https://www.lua.org/download.html) from source code. It is far more easier.__

5. Specific tools and libraries:
  - Make sure all development tools are installed for your system (try to compile once and you'll get the idea what is missing)
  - Apple & Linux : make sure `zenity` is in your system (it is in most of distros by default). It is used for file dialogs.
    - Apple     : `brew install zenity`
    - Linux     : `sudo apt install zenity`
    - Windows   : Here, window's API for filesystem interaction is used. ___(Not tested yet)___

Setting up the project
- clone the repository with submodules `git clone --recurse-submodules https://github.com/therealsunx/everest`
- go into the project directory `cd everest`

Quick View
- Run `editor_run.sh` to quickly view the editor

Hand-Building the project:
- Run `cmake -B build` from root project directory to build the project
    Setting options : `-D[option]=[value]`
    - Build modes [Debug, Release, Dist]: Example:- `cmake -B build -DBuild=Debug`
    - Build Target [Editor, Game]: Example:-  `cmake -B build -DTarget=Editor`

- For setting up Engine as build target : run `cmake -B build -DBuild=Dist -DTarget=Editor`
- Build the project using `cmake --build build/`
- Run editor using `./build/bin/everest-editor/everest-editor`

- For setting up Game as build target   : run `cmake -B build -DBuild=Dist -DTarget=Game`
- Build the project using `cmake --build build/`
- Run game using `./build/bin/tower-defense/tower-defense`



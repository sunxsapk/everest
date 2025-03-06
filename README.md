# Everest

<p align="center">
    <img src="assets/everest_logo.png" />
</p>


## Overview

__Everest__ is an cross platform game engine written in C/C++. As of now, Engine is 3D scene-editor 
supported but allows build for only 2d games (i.e. 3d camera and view is supported but there is no
mesh loader and 3d physics added to it as of now). Let's talk brielfly about the core everst-engine and the
editor that comes with it:

Everest Engine is the game engine library built with c/c++, using OpenGL as underlying API for its
its renderer. It has its own custom physics system, input system, rendering system and runtime scripting
system. It wraps [entt](https://github.com/skypjack/entt) for its entity component system and 
[sol2](https://github.com/ThePhD/sol2) for its lua-scripting system. It uses YAML as its scene serialization
format, and thus uses [yaml-cpp](https://github.com/jbeder/yaml-cpp) for parsing and writing YAML files.

## Documents for you

1. Project Setup and build is available [here](/docs/Setup.md).
2. Scripting API in detail in available [here](/docs/ScriptingAPI.md)


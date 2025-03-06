# Everest

<p align="center">
    <img src="assets/everest_logo.png" />
</p>


## Overview

__Everest__ is an cross platform game engine under development, written in C/C++. As of now, Engine
is 3D scene-editor supported but allows build for only 2d games (i.e. 3d camera and view is supported
but there is no mesh loader and 3d physics added to it as of now). Let's talk brielfly about the core
everest-engine and the editor that comes with it:

Everest Engine is the game engine library built with c/c++, using OpenGL as underlying API for its
its renderer. It has its own custom physics system, input system, rendering system and runtime scripting
system. It wraps [entt](https://github.com/skypjack/entt) for its entity component system and 
[sol2](https://github.com/ThePhD/sol2) for its lua-scripting system. It uses YAML as its scene serialization
format, and thus uses [yaml-cpp](https://github.com/jbeder/yaml-cpp) for parsing and writing YAML files.
And finally, for UI, it uses ImGui because of its excellent and easy to use programming API.


__What does Everest engine has that you might be interested in?__

- Layered Application Architecture:
    Engine supports dynamic layered architecture, which allows you to compose your custom application
    as multiple layers (like debug-layer, scene-layer, etc).

- UI system:
    Engine provides the support for ImGui-based UI system, which you can use to create all sorts of
    interactive features.

- Input Management:
    Everest engine has a properly managed input system for your game. Various types of input devices
    like mouse, keyboard, joystick, etc are supported. Also, the event listener and dispatcher are very
    intutive and hide complexities for programmer excellently.

- Window Management:
    Everest engine provides a decent window management system, with intuitive API for handling it.

- Logging System:
    A C-style formatted logging system is provided by engine to make your applications easy to debug.

- Instrumentation:
    Your applications are runtime profilable by using a simple macro inside all the functions or scopes
    you need to monitor. You can look at the results in your browsers profile-tracer to look at all the
    functions called along with the time they took.

- Physics System:
    As of now, Everest has its own 2d physics system. In depth, it uses a lot of optimization techniques for
    collision approximation, detection and resolution, along with physics simulation. It has provided
    components like colliders, rigidbodies and springJoints.

- Rendering System:
    Everest engine's renderer has everything set up for supporting 3d rendering, and has a complete
    2d renderer. There is no mesh loader provided as of now by engine. So, if you write your own mesh
    loader you can use engine's renderer to render the mesh easily. Engine's 2d renderer uses batching
    for improving performance. Instanced renderer will be added after mesh loader. Talking about 2d
    renderer, there is an intuitive API provided for making development and debugging easier.

- Entity Component System:
    Engine uses entity component system with following components provided for use:
    - Transform
    - Tag
    - Camera
    - SpriteRenderer
    - CircleRenderer
    - Rigidbody2d
    - SpringJoint2d
    - BoxCollider2d
    - CircleCollider2d
    - EvScript

- Runtime lua-scripting
    Everest engine uses lua as its runtime scripting language due to a primary reason that lua is a
    very easy to learn and use language, and also due to the fact that Just In Time(JIT) compiler for
    lua is the fastest one that is available there for scripting. Engine provides a decent scripting
    API for you to use it. You can use this feature to give custom behaviour to entities in game.

- Native C++ Scripting
    Everest engine also has support for native (c++) scripting to make your faster game, even faster.
    You can use core engine API for more advanced scripting in C++.

- Assets Manager
    Everest engine has a decent assets management. Managing assets like fonts, scenes, textures, etc.
    are very sensitive things, when it comes to performance. Everest got you covered.

- Scene Editor
    Everest editor is the scene editor provided by everest engine for editing your scenes and saving
    them, testing them. Editor also has some easily overlookable features like rendering stats, timing
    and frame information, etc which might be vital for development of your game.

- Other
    Other important features that engine provides are,
    - Timing and Clock
    - Game Mathematics

## Documents for you

1. Project Setup and build is available [here](/docs/Setup.md).
2. Programming API is available [here](/docs/ProgrammingAPI.md)
3. Lua Scripting API in detail in available [here](/docs/ScriptingAPI.md)


# Everest Engine Build

Building Everest Engine from source code has a very simple set of steps to be followed. Simply go to
the build guide for your platform and make yourself comfortable.

## Linux

### Install dependencies
In linux, you have to install following set of tools and dependencies, before you start building the
engine. (__As of now, Everest Engine is only tested on debian-based distros.__)

1. __C/C++ Compiler__
    ```bash
        sudo apt-get update
        sudo apt-get install g++ build-essential
    ```

2. __CMake & Ninja__
    ```bash
        sudo apt-get install cmake ninja-build
    ```

### Setup & Build

1. Clone the source code recursively into your machine or simply download it from [here](https://github.com/sunxsapk/everest/releases).
```bash
    git clone --recurse-submodules https://github.com/sunxsapk/everest
    cd everest
```

2. Set up the project in appropriate mode (___Debug, Release___)
example:-
    ```bash
    cmake -B build -DCMAKE_BUILD_TYPE=Release
    ```

3. Build the project
```bash
    cmake --build build
```

4. `everest-editor` executable should be built and located at `build/bin/everest-editor/` directory, which
    you can run easily.
```bash
    ./build/bin/everest-editor/everest-editor
```

5. Installing in your machine
    If you want to install everest-editor to your machine, then you will have to do so in __Release__ mode.
    __Debug__ mode is only recommended during development stage of engine. If you have followed up to here
    using the snippets provided above, then you can run following to install everest-engine on your system.
    ```bash
        sudo cmake --install build/
    ```

6. Now, you can run everest-editor anywhere from the command-line.
    ```bash
        everest-editor
    ```

__If you have installed the Everest Engine according to above defined ways, then you can also use it to
develop applications is C++. Simply, link `everest` library to your C++ project and include `everest/everest.h`
in C++ file.__
```c++
    #include <everest/everest.h>

    // ... rest of your code ...
```
Linking `everest` library, example :-
`g++ main.cpp -o myproject -leverest -L/usr/local/lib`


---

## Windows

### Install dependencies

In windows, you're gonna have to first install the microsoft visual studio development environment for
C/C++ development. Get it from [here](https://visualstudio.microsoft.com/downloads/).


### Setup & Build

1. Clone the source code recursively into your machine or simply download it from [here](https://github.com/sunxsapk/everest/releases).
```bash
    git clone --recurse-submodules https://github.com/sunxsapk/everest
    cd everest
```

2. Set up the project in appropriate mode (___Debug, Release___)
example:-
    ```bash
    cmake -B build -DCMAKE_BUILD_TYPE=Release
    ```

3. Build the project
```bash
    cmake --build build
```

4. `everest-editor` executable should be built and located at `build\bin\everest-editor\<BuildMode>\` directory, which
    you can run easily. Example :-
```cmd
    build\bin\everest-editor\Release\everest-editor
```

5. Installing in your machine
    If you want to install everest-editor to your machine, then you will have to do so in __Release__ mode.
    __Debug__ mode is only recommended during development stage of engine. If you have followed up to here
    using the snippets provided above, then you can run following to install everest-engine on your system.

    Install using following command. (__Make sure you've ran command prompt with administrative privilages__).
    ```cmd
        cmake --install build
    ```

    After this, you will have to add the __bin__ directory inside installed location of __Everest Engine__ to __PATH__.

6. Now, you can run everest-editor anywhere from the command-line.
    ```bash
        everest-editor
    ```

__If you have installed the Everest Engine according to above defined ways, then you can also use it to
develop applications is C++. Simply, link `everest` library to your C++ project and include `everest/everest.h`
in C++ file.__
```c++
    #include <everest/everest.h>

    // ... rest of your code ...
```
Linking `everest` library, example :-
`cl main.cpp -o myproject -leverest -L/path/to/lib`

___Change the link directory to appropriate path according to your installed location.___


---

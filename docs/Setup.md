# Everest Engine Setup

Setting up everest engine on your machine requires you to first of all, install all the dependencies
then build it from source code.

___You can also download the precompiled binaries as soon as they are made available. In that case,
you won't have to follow the steps here.___

___Also, soon enough scripts for automating this process will be made available. In that case, you can
just run the platform specific script for setting up the project. Take in consideration that the project
is currently under continous development, so feedback on some bugs found, feature requests, etc are highly
appreciated.___


## Installing the Dependencies

You should have the dependencies installed before you can actually build and run the engine. Here is
the guide on how to install all the dependencies on each machine:

Here’s a step-by-step guide to installing the required tools on **Windows, Linux, and macOS**.

---

# **Linux Installation Guide (Ubuntu/Debian-based)**

### 1. Install **CMake**
```sh
sudo apt update
sudo apt install -y cmake
```
Verify installation:
```sh
cmake --version
```

### 2. Install **C++ Compiler**
```sh
sudo apt install -y build-essential
```
Verify installation:
```sh
g++ --version
```

### 3. Install **GLFW**
```sh
sudo apt install -y libglfw3-dev
```

### 4. Install **Lua**
```sh
sudo apt install -y lua5.4
```

- Alternatively, install manually:
  - Download Lua from: [https://www.lua.org/download.html](https://www.lua.org/download.html)
  - Extract and follow the build instructions.

Verify installation:
```sh
lua -v
```

### 5. Install **Zenity**
```sh
sudo apt install -y zenity
```

---

# **macOS Installation Guide**

### 1. Install **Homebrew** (if not installed)
```sh
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```
Verify installation:
```sh
brew --version
```

### 2. Install **CMake**
```sh
brew install cmake
```
Verify installation:
```sh
cmake --version
```

### 3. Install **C++ Compiler**
```sh
xcode-select --install
```
Verify installation:
```sh
g++ --version
```

### 4. Install **GLFW**
```sh
brew install glfw
```

### 5. Install **Lua**
```sh
brew install lua
```

- Alternatively, install manually:
  - Download Lua from: [https://www.lua.org/download.html](https://www.lua.org/download.html)
  - Extract and follow the build instructions.

Verify installation:
```sh
lua -v
```

### 6. Install **Zenity**
```sh
brew install zenity
```

# **Windows Installation Guide (!! UnTested As Of Now !!)**

### 1. Install **CMake**
- Download the latest **CMake** from: [https://cmake.org/download/](https://cmake.org/download/)
- Run the installer and select **"Add CMake to system PATH"**.
- Verify installation:
  ```sh
  cmake --version
  ```

### 2. Install **C++ Compiler**
- Install **MinGW-w64** (GCC):
  - Download **MinGW-w64** from: [https://winlibs.com/](https://winlibs.com/)
  - Extract it to `C:\mingw64`
  - Add `C:\mingw64\bin` to **System PATH** (via Environment Variables)
  - Verify installation:
    ```sh
    g++ --version
    ```

- Alternatively, install **MSVC** (Microsoft C++ Compiler):
  - Install **Visual Studio Community** from: [https://visualstudio.microsoft.com/](https://visualstudio.microsoft.com/)
  - Select **C++ Development Tools** during installation.
  - Verify installation:
    ```sh
    cl
    ```

### 3. Install **GLFW**
- Install via **vcpkg** (Recommended):
  ```sh
  git clone https://github.com/microsoft/vcpkg.git
  cd vcpkg
  bootstrap-vcpkg.bat
  vcpkg install glfw3
  ```

- Alternatively, install manually:
  - Download from: [https://www.glfw.org/download.html](https://www.glfw.org/download.html)
  - Extract and build using **CMake**.

### 4. Install **Lua**
- Install via **vcpkg**:
  ```sh
  vcpkg install lua
  ```

- Alternatively, install manually:
  - Download Lua from: [https://www.lua.org/download.html](https://www.lua.org/download.html)
  - Extract and follow the build instructions.

---

## Building the engine

Clone the repository and go into the project directory and run the `editor_run.sh` script for quickly 
using the engine.
``` bash
    git clone --recurse-submodules https://github.com/sunxsapk/everest
    cd everest
    ./editor_run.sh
```

## Customizing the build

Run commands yourself the project after cloning the repo for different build options:
- Run `cmake -B build` from root project directory to build the project
    Setting options : `-D[option]=[value]`
    - Build modes [Debug, Release, Dist]: 
        Example:- `cmake -B build -DBuild=Debug` ...  `cmake -B build -DBuild=Release`
    - Build Target [Editor, Game]: Example:-  `cmake -B build -DTarget=Editor` or `cmake -B build -DTarget=Editor`

- For setting up Engine as build target : run `cmake -B build -DBuild=Dist -DTarget=Editor`
- Build the project using `cmake --build build/`
- Run editor using `./build/bin/everest-editor/everest-editor`

__As of now, Game is unbuildable from editor. If you want to run a game you've edited then you can do
so by writing some C++ code for running the saved scenes. Building the game form the editor will be
made available soon.__

__For an example on how to run a game out of editor, checkout the following example game scene running
standalone.__
- For setting up Game as build target   : run `cmake -B build -DBuild=Dist -DTarget=Game`
- Build the project using `cmake --build build/`
- Run game using `./build/bin/examples/examples`


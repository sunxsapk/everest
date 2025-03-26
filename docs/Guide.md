# Everest Engine Guide

## Introduction

Everest Engine is a powerful yet lightweight game engine designed for flexibility and ease of use.
It provides a Lua scripting system similar to Unity's C# workflow, with support for serializable fields
for UI accessibility. The engine includes an intuitive scene editor, a robust entity-component system (ECS)
using EnTT, and seamless integration with modern graphics and physics libraries.

### Features:

- __Lua Scripting__: Seamless scripting integration with dynamic serialization.
- __Entity-Component System__: Powerful ECS for efficient game object management.
- __Cross-Platform__: Supports Windows and Linux. (Support for macOS soming soon)
- __Scene-Editor__: A scene editor for intuitive game development.

## Installation

___Everest Engine provides pre-built installers and zipped binaries, available on the GitHub Releases page.___

### Windows Installation

_The engine does not have a digitally signed Windows installer. As a result, Windows Defender may block the
installation. You can bypass this by selecting "More info" → "Run anyway"._

Alternatively, you can download the zipped binaries and extract them to your preferred location.

### Linux Installation

Linux installer is available as ___.deb___ file. _( __.rpm__ installer will be available after testing)_

## Generating Project Files

Everest Engine provides a command-line tool to generate project files for different build systems.

### Generating a Project

Everest Engine provides a command-line option to generate a new project using a default template.

__`-G` : Generates Project__

- __Usage__: `everest-editor -G <path>`
    - Generates project files at the specified <path> and loads the default project template.

- __Example__:
    ```bash
        everest-editor -G Desktop/FlappyBird
    ```

### Command-Line Options

__`-B` : Builds Standalone Project__

- __Usage__: `everest-editor -B <path>`
    - Builds the project located at <path> into a standalone executable.

- __Example__:
    ```bash
        everest-editor -B Desktop/FlappyBird
    ```

__`-H` : Command-Line Help__

- __Usage__: `everest-editor -H`
    - Displays the command line help page for Everest Engine

- __Example__:
    ```bash
        everest-editor -H
    ```

### Launching a Project

__Method 1: Launching in the Current Directory__

```bash
everest-editor
```
- Launches the project located in the current directory using Everest Editor.

__Method 2: Launching a Specific Project__

```bash
everest-editor <path>
```
- Launches the project located at the given <path> in Everest Editor.

___Failure Case___

- The command fails to launch the project if an Everest.project file is not found in the target directory.


## Editor's First Impression

When launching Everest Editor, you will be greeted with a clean and minimal interface designed for efficient workflow.

### Key UI Elements:

- __Scene View__: Displays the game world and allows object manipulation.

- __Entity Hierarchy__: Lists all entities in the current scene.

- __Inspector Panel__: Allows editing of entity components and properties.

- __Asset Browser__: Helps manage game assets efficiently.

---

### First Steps

Your first steps for making games with Everest Engine begins by creating a new scene. By default a
default scene is opened, or you can create one from the menu bar. You can add or remove entities from
the __Scene Hierarchy Panel__ by right clicking on it. In order to add components to the entities, you can
select the entity and add the components form properties panel as you like. Each components have some
unique features that they will add to the entity that contains them. Learn more about the components or
Entity Componenet System (ECS) of Everest Engine [here](/docs/ECS.md).

Let's create a simple physics based scene, by creating few entities, adding renderers to them, then adding
colliders (box or circle collider) and rigidbody2d components. There are example scenes provided too, you can
take a look at them.

_In order to simulate the physics, you can click the __run__ icon on __Scene Panel__ or press __f5__ key._

Now, you're gonna want to add custom behaviour to the entities, to make them controllable using user inputs or
custom update scripts. Everest got you covered as it provides the __Lua Scripting System__ for you to be able to
write the custom scripts in easy and simple language such as __lua__. You can take a look at 
[__Scripting API__](/docs/ScriptingAPI.md) for detail information on runtime scripting system.

___You should be now able to make our own game and run it.___

If you are more hardcore programmer and want to build custom C++ applications, games or tools using C++
programming API, you can take a look at [this](/docs/ProgrammingAPI.md). You can also look the Everest Editor's
source code to understand how the C++ API is used to make a custom scene editor.


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


## Features

Everest Engine is desgined to provide a decent game development experience. Here are few things that
makes this project interesing:

### **1. Layered Application Architecture**
Everest Engine features a dynamic layered architecture, allowing you to structure your application using multiple layers. This approach enables you to separate different functionalities—such as rendering, debugging, scene management, and UI—into distinct layers. This modular system improves code organization, enhances maintainability, and allows for greater flexibility in application design.

For instance, you can have:

- A Debug Layer for real-time debugging tools.
- A Scene Layer for handling the main game world.
- A UI Layer for user interface rendering.

Each of these layers can be independently added, removed, or modified as needed.

---

#### **2. UI System**
Everest Engine integrates an **ImGui-based UI system**, making it simple to create interactive and responsive interfaces. Whether it's debugging overlays, menus, or custom tools, the UI system provides:
- Predefined widgets for quick development.
- Customizability to tailor interfaces to your needs.
- A straightforward API that accelerates development.

---

#### **3. Input Management**
Input handling is intuitive and well-organized in Everest Engine. The input management system:
- Supports multiple input devices such as keyboards, mice, and joysticks.
- Provides an **event listener and dispatcher** to streamline input handling.
- Abstracts complexities, allowing developers to focus on gameplay instead of technical intricacies.

---

#### **4. Window Management**
Everest Engine offers a **window management system** with a user-friendly API. Developers can easily:
- Create and manage application windows.
- Handle window events such as resizing and focus changes.
This system ensures a smooth user experience and reduces boilerplate code.

---

#### **5. Logging System**
Debugging is made easier with a **C-style formatted logging system**. The logging system:
- Enables you to log messages and events effectively.
- Helps track and resolve issues during development by providing clear and concise outputs.

---

#### **6. Instrumentation**
Everest Engine supports **runtime profiling**, allowing developers to analyze the performance of their applications. By using a simple macro, you can:
- Monitor functions or code scopes at runtime.
- Visualize profiling data in your browser’s trace viewer.
This feature makes it easier to identify performance bottlenecks.

---

#### **7. Physics System**
The engine includes a custom **2D physics system** with advanced features such as:
- **Collision detection and resolution** using optimized techniques.
- Physics simulation components like:
  - Colliders (Box and Circle)
  - Rigidbodies
  - Spring Joints
This robust system provides accurate physics behavior while maintaining performance.

---

#### **8. Rendering System**
Everest Engine's rendering system is designed for both **2D and 3D rendering**:
- **2D Renderer**:
  - Supports batching for improved performance.
  - Includes an intuitive API for development and debugging.
- **3D Renderer**:
  - While 3D rendering is supported, there’s no mesh loader included yet.
  - Developers can write their own mesh loader and use the engine's renderer seamlessly.

Upcoming features like an **instanced renderer** will further enhance its capabilities.

---

#### **9. Entity Component System (ECS)**
The engine uses an **Entity Component System (ECS)** for organizing game objects. It provides the following components:
- **Transform**: For position, rotation, and scaling.
- **Tag**: For object identification.
- **Camera**: For view management.
- **SpriteRenderer** and **CircleRenderer**: For rendering 2D objects.
- **Rigidbody2D**, **SpringJoint2D**, **BoxCollider2D**, and **CircleCollider2D**: For physics and collision.
- **EvScript**: A component for attaching Lua scripts to entities.

---

#### **10. Runtime Lua Scripting**
Everest Engine uses **Lua** as its runtime scripting language, chosen for its:
- Ease of learning and usage.
- Speed, thanks to Lua’s JIT compiler.
The scripting API allows developers to create custom behaviors for entities dynamically, adding flexibility to game development.

---

#### **11. Native C++ Scripting**
For developers who need high performance, the engine supports **native C++ scripting**. This allows direct access to the core engine API, enabling advanced and optimized gameplay logic.

---

#### **12. Asset Manager**
Everest Engine includes an **asset management system** for handling critical resources such as:
- Fonts
- Textures
- Scenes
It ensures efficient loading and management of assets, optimizing performance.

---

#### **13. Scene Editor**
The **Everest Editor** is a built-in tool for creating and managing scenes. Its features include:
- Scene editing and saving capabilities.
- Real-time testing.
- Tools for monitoring rendering stats, frame rates, and timing.

---

#### **14. Other Features**
Additional tools and features provided by Everest Engine include:
- **Timing and Clock**: For accurate time management in games.
- **Game Mathematics**: A suite of mathematical utilities tailored for game development.

---

Everest Engine is a comprehensive, developer-friendly framework that balances ease of use with powerful features, making it an excellent choice for game creators.


## Documents for you

1. Project Setup and build is available [here](/docs/Setup.md).
2. Programming API is available [here](/docs/ProgrammingAPI.md)
3. Lua Scripting API in detail in available [here](/docs/ScriptingAPI.md)


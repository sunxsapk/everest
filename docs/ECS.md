# Everest Engine - Entity Component System (ECS)

Everest Engine uses a modern and efficient **Entity Component System (ECS)** powered by [EnTT](https://github.com/skypjack/entt), a fast and reliable ECS library for C++.

---

## What is ECS?

The Entity Component System is a design pattern commonly used in game development to manage game objects. It promotes composition over inheritance, making it easier to build flexible and reusable systems.

- **Entity**: A lightweight identifier (like an integer) that represents a game object.
- **Component**: Plain data attached to entities (e.g., Transform, Sprite, PhysicsBody).
- **System**: Logic that operates on entities with specific components (e.g., a rendering system operates on entities with a Sprite and Transform).

---

## How ECS Works in Everest

Everest Engine's core `Scene` class manages entities and components through EnTT. Entities are created and destroyed using the `Scene` API, and components can be attached dynamically.

### Example
```cpp
// Create an entity
Entity player = scene.CreateEntity("Player");

// Add components
auto& tfr = player.add<transform_c>();
tfr.position = vec3(10.f);
auto& spr = player.add<spriteRenderer_c>();
spr.color = vec4(1.f, 0.f, 0.f, 1.f);
```

### Component Serialization
Components in Everest Engine are serializable using `yaml-cpp`, allowing scene data to be saved and loaded easily. This is especially useful in the editor and during runtime.

---

## Built-in Components
Everest provides several core components out of the box. All of the components inherit the base class
`component_t` which provides the basic functionalities to the components to keep information about parent
entity. It looks somewhat like this:

```cpp
    struct component_t {
        Entity entity;
        bool active = true;
    };

```
---

___Each of the components of the Everest Engine have sample implementation details of them provided. Actual
implementation might be a little bit different, more or might have more details to them. But the basic idea
behind them will still be the same.___

---

### id_c

Stores the universally unique identifier of the entity (used for special purposes by the engine).

```cpp
    class UUID {
         public:
             operator u64() const {return _uuid;}
         private:
             u64 _uuid;
    };

    struct id_c : public component_t {
        UUID id;

        operator u64() const {return id;}
    };
```

### tag_c

Stores the name of the entity.

```cpp
    struct tag_c : public component_t {
        std::string tag;
    };
```

### transform_c

Holds position, rotation, and scale data.

```cpp
    struct transform_c : public component_t {
        vec3 position{0.f}, rotation{0.f}, scale{1.f};

        operator mat4(){
            mat4 trl = glm::translate(mat4(1.f), position);
            mat4 rot = glm::toMat4(glm::quat(glm::radians(rotation)));
            mat4 scl = glm::scale(mat4( 1.f), scale);
            return trl * rot * scl;
        }
    };
```

### circleRenderer_c

Used for 2D rendering (textured-quad).

```cpp
    struct circleRenderer_c : public component_t {
        ref<Texture> texture = nullptr;
        vec4 color{1.f};
        vec2 startUV = vec2(0.f);
        vec2 sizeUV  = vec2(1.f);
        f32 thickness = 1.f;
        f32 fade = 0.f;
    };
```

### spriteRenderer_c

Used for 2D rendering (textured-circle).

```cpp
    struct spriteRenderer_c : public component_t{
        ref<Texture> texture = nullptr;
        vec4 color{1.f}; 
        vec2 startUV = vec2(0.f);
        vec2 sizeUV  = vec2(1.f);
    };
```

### camera_c

Defines the rendering view.

```cpp
    struct camera_c : public component_t {
        bool isPrimary, fixedAspect;


        mat4 getProjection();

        f32 get_lenssize() const;
        f32 get_fov() const;
        f32 get_aspect() const;
        f32 get_near() const;
        f32 get_far() const;

        void set_fov(f32 fov);
        void set_near(f32 near_);
        void set_far(f32 far_);
        void set_lenssize(f32 size);
        void set_aspect(f32 aspect);
        

        bool is2d() const;
        bool is3d() const;
        void set2d();
        void set3d();

        protected:
        mat4 _projection;
        f32 _u_size_fov, _aspect, _near, _far;
        bool _isOrtho;
    };
```

### rigidbody2d_c

Adds physics behaviour to body.

```cpp
    struct rigidbody2d_c : public component_t {
        vec2 velocity;
        f32 angularVelocity;
        f32 drag;
        f32 inverseMass;
        f32 inverseInertia;
        int definition;

        void addForce(const vec2 force, const ForceMode mode = ForceMode::Force);
        void addForceAtOffset(const vec2 force, const vec2 offset);
        void setMass(f32 value);
        f32 getMass();
        vec2 getAcceleration() const;
    };
```


### boxCollider2d_c

Adds 2D Box Collider to the entity.

```cpp
    struct box2dprops_t {
        vec2 offset = vec2(0.f);
        vec2 halfExtents = vec2(0.5f);

        bool contains(vec2 point);
    };

    struct boxCollider2d_c : public component_t {
        box2dprops_t box;
        f32 restitution = 0.5f;
    };
```

### circleCollider2d_c

Adds Circle Collider to the entity.

```cpp
    struct circleprops_t {
        vec2 offset = vec2(0.0f);
        f32 radius = 0.5f;

        bool contains(vec2 point);
    };

    struct circleCollider2d_c : public component_t {
        circleprops_t circle;
        f32 restitution = 0.5f;
    };
```

### EvScript

Attaches a Lua script to the entity.

```cpp
    struct scriptHandler_t {
        std::filesystem::path scriptpath;
        ref<luastate_t> state = nullptr;

        std::function<void()> onCreate;
        std::function<void(double)> onUpdate;
        std::function<void(collision2d_t)> onCollision;

        bool _initialized = false;

        void init(Entity entity);
        void update(double deltaTime);
        void collisionCallback(collision2d_t& data);

        bool getSerializedFields(sol::table& resultTable) const;

        void setScriptPath(std::filesystem::path path, Entity ent);
        std::string getScriptName();

        sol::table call(std::string func_name, sol::table inp_args);

        template<typename T>
        T get(std::string var_name){
            return (*state)[var_name];
        }
    };

    struct evscript_c : public component_t {
        std::vector<scriptHandler_t> scripts;

        void init();
        void update(double deltaTime);
        void collisionCallback(collision2d_t& data);

        scriptHandler_t& addScript(const std::filesystem::path& path);

        scriptHandler_t* tryGetScriptHandler(std::string name);
    };
```

---

## Benefits of ECS in Everest

- **Performance**: Data-oriented structure is cache-friendly.
- **Flexibility**: Add/remove behavior via components without inheritance.
- **Modularity**: Systems can be developed and tested independently.
- **Serialization**: Scene data is easily saved/loaded.

---

Everest Engine’s ECS is designed to be intuitive, high-performance, and tightly integrated with both scripting and the editor. It gives you powerful control over your game world while staying flexible and modular.

---



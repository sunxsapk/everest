# Scripting API

Everest has 'lua' as a runtime scripting language and provides easy to use API for writing your custom
behaviour for Entities in game.

__Why Lua?__
Lua is an extremely simple and easy to - understand & write - language. It is so simple that it has
world's fastest Just-In-Time(JIT) compiler. Unless you are making some high-end AAA game, Lua has
almost no performance impact. ___(And also because it made it way too easy for me to integrate it.)___

## Rules / Syntax

Everest Engine provides extension for some of its objects into the lua script. Here are some of the
ground rules you have to follow for writing a proper script.

1. __Predefined functions__
    A lua behaviour script has some predefined functions which are called by engine at certain instances.
    You are not adviced to call those functions yourselves inside the script. You can only define those
    functions. _(Although you can call those functions but calling them at wrong time might cause some
    undefined behaviours.)_

    Here are those functions:
    - ___OnCreate___
        This function is executed whenever the script instance is created on the object. You can put
        some initialization code on here.
        ```lua
            function OnCreate() 
                -- your code
            end
        ```

    - ___OnUpdate___
        This function is called every frame. This is where you put code to continously update the
        script. Like handling inputs, custom behaviour and whatnot.
        ```lua
            function OnUpdate(deltaTime) 
                -- your code
            end
        ```

    - ___OnCollision___
        This function is called everytime the entity collides with another one. Some information about
        collision is passed as an arguments to the function which you can then utilize to do some crazy
        stuff.
        ```lua
            function OnCollision(collisionData) 
                -- your code
            end
        ```

2. __Constructors__
    Whenever you want to instantiate a object or type provided by engine, you have to follow the syntax:
    `type.new(...args)`

3. __Methods__
    Calling member functions (aka methods) of objects has simple syntax as follows:
    `instance:function(...args)`

4. __entity__
    You can access the Entity which holds the script by using the global variable `entity`.
    For all the scripting API provided for Entity look [here](#Entity)

__Example Script__
```lua
    -- Player Behaviour
OnCreate = function()
        rigidbody = entity:get("Rigidbody") -- TODO
    end

    OnUpdate = function(deltaTime)
        local dp = vec3.new(0, 0)
        if Input:getKey('D') then
            dp.x = deltaTime;
        elseif Input:getKey('A') then
            dp.x = -deltaTime;
        end
        rigidbody.velocity:add(dp)
    end

    OnCollision = function(other, contact_point, contact_normal) -- other : Entity
        local b_info = other.get("BulletInfo") -- BulletInfo component if collision is with bullet
        if b_info ~= nil then
            health = health - b_info.damage
        end
    end
```

---


### Extended Types

#### **vec2**

Represents a 2D vector with `x` and `y` components.

- **Constructors:**
  - `vec2.new(f32, f32)`: Creates a `vec2` from two `f32` values (x, y).
  - `vec2.new(vec2)`: Creates a `vec2` by copying another `vec2`.
  - `vec2.new(f32)`: Creates a `vec2` with both `x` and `y` set to the given `f32` value.
  - `vec2.new()`: Creates a `vec2` with both `x` and `y` initialized to 0.

- **Members:**
  - **x** (`f32`): The x-component of the vector.
  - **y** (`f32`): The y-component of the vector.

- **Methods:**
  - **length**(): `f32`  
    Returns the magnitude (length) of the vector.
  
  - **normalized**(): `vec2`  
    Returns a normalized version of the vector (a vector with a magnitude of 1).
  
  - **add**(vec2 b): `vec2`  
    Adds vector `b` to the current vector, modifying it in place (`a += b`).

  - **mul**(vec2 b): `vec2`  
    Multiplies the current vector by vector `b`, modifying it in place (`a *= b`).

  - **div**(vec2 b): `vec2`  
    Divides the current vector by vector `b`, modifying it in place (`a /= b`).

  - **smul**(f32 b): `vec2`  
    Scales the vector by a scalar `b`, modifying it in place (`a *= b`).

  - **sdiv**(f32 b): `vec2`  
    Scales the vector by the scalar `1 / b`, modifying it in place (`a /= b`).

  - **sub**(vec2 b): `vec2`  
    Subtracts vector `b` from the current vector, modifying it in place (`a -= b`).

  - **dot**(vec2 b): `f32`  
    Returns the dot product of the current vector and vector `b`.

---

#### **vec3**

Represents a 3D vector with `x`, `y`, and `z` components.

- **Constructors:**
  - `vec3.new(f32, f32, f32)`: Creates a `vec3` from three `f32` values (x, y, z).
  - `vec3.new(vec3)`: Creates a `vec3` by copying another `vec3`.
  - `vec3.new(f32)`: Creates a `vec3` with all components set to the given `f32` value.
  - `vec3.new()`: Creates a `vec3` with all components initialized to 0.

- **Members:**
  - **x** (`f32`): The x-component of the vector.
  - **y** (`f32`): The y-component of the vector.
  - **z** (`f32`): The z-component of the vector.

- **Methods:**
  - **length**(): `f32`  
    Returns the magnitude (length) of the vector.

  - **normalized**(): `vec3`  
    Returns a normalized version of the vector (a vector with a magnitude of 1).

  - **add**(vec3 b): `vec3`  
    Adds vector `b` to the current vector, modifying it in place (`a += b`).

  - **mul**(vec3 b): `vec3`  
    Multiplies the current vector by vector `b`, modifying it in place (`a *= b`).

  - **div**(vec3 b): `vec3`  
    Divides the current vector by vector `b`, modifying it in place (`a /= b`).

  - **smul**(f32 b): `vec3`  
    Scales the vector by a scalar `b`, modifying it in place (`a *= b`).

  - **sdiv**(f32 b): `vec3`  
    Scales the vector by the scalar `1 / b`, modifying it in place (`a /= b`).

  - **sub**(vec3 b): `vec3`  
    Subtracts vector `b` from the current vector, modifying it in place (`a -= b`).

  - **dot**(vec3 b): `f32`  
    Returns the dot product of the current vector and vector `b`.

  - **cross**(vec3 b): `vec3`  
    Returns the cross product of the current vector and vector `b`, a vector perpendicular to both.

---

#### **vec4**

Represents a 4D vector with `x`, `y`, `z`, and `w` components.

- **Constructors:**
  - `vec4.new(f32, f32, f32, f32)`: Creates a `vec4` from four `f32` values (x, y, z, w).
  - `vec4.new(vec4)`: Creates a `vec4` by copying another `vec4`.
  - `vec4.new(f32)`: Creates a `vec4` with all components set to the given `f32` value.
  - `vec4.new()`: Creates a `vec4` with all components initialized to 0.

- **Members:**
  - **x** (`f32`): The x-component of the vector.
  - **y** (`f32`): The y-component of the vector.
  - **z** (`f32`): The z-component of the vector.
  - **w** (`f32`): The w-component of the vector.

- **Methods:**
  - **length**(): `f32`  
    Returns the magnitude (length) of the vector.

  - **normalized**(): `vec4`  
    Returns a normalized version of the vector (a vector with a magnitude of 1).

  - **add**(vec4 b): `vec4`  
    Adds vector `b` to the current vector, modifying it in place (`a += b`).

  - **mul**(vec4 b): `vec4`  
    Multiplies the current vector by vector `b`, modifying it in place (`a *= b`).

  - **div**(vec4 b): `vec4`  
    Divides the current vector by vector `b`, modifying it in place (`a /= b`).

  - **smul**(f32 b): `vec4`  
    Scales the vector by a scalar `b`, modifying it in place (`a *= b`).

  - **sdiv**(f32 b): `vec4`  
    Scales the vector by the scalar `1 / b`, modifying it in place (`a /= b`).

  - **sub**(vec4 b): `vec4`  
    Subtracts vector `b` from the current vector, modifying it in place (`a -= b`).

  - **dot**(vec4 b): `f32`  
    Returns the dot product of the current vector and vector `b`.

---


#### **mat4**

Represents a 4x4 matrix, typically used for transformations like translations, rotations, and scaling in 3D graphics.

- **Constructors:**
  - `mat4()`: Creates an identity matrix (a 4x4 matrix with 1s on the diagonal and 0s elsewhere).
  - `mat4(f32)`: Creates a 4x4 matrix where diagonal elements are initialized to the provided scalar value `f32`.

- **Methods:**
  - **add**(mat4 b): `mat4`  
    Adds matrix `b` to the current matrix, modifying it in place (`a += b`).

  - **sub**(mat4 b): `mat4`  
    Subtracts matrix `b` from the current matrix, modifying it in place (`a -= b`).

  - **mul**(mat4 b): `mat4`  
    Multiplies the current matrix by matrix `b`, modifying it in place (`a *= b`).

  - **smul**(f32 b): `mat4`  
    Scales the current matrix by the scalar `b`, modifying it in place (`a *= b`).

---

## Enums

### **Enum: ForceMode**

`ForceMode` defines different ways in which forces can be applied to a rigidbody. Each mode dictates how the force is interpreted and applied, such as whether it’s an instant change in velocity or an ongoing force.

- **ForceMode::Force**  
  Applies a continuous force to the rigidbody over time. This mode simulates a physical force such as gravity or friction.

- **ForceMode::Acceleration**  
  Applies a constant acceleration to the rigidbody, which is independent of the rigidbody's mass.

- **ForceMode::Impulse**  
  Applies an instant change to the rigidbody’s velocity, simulating a sudden force, such as an explosion or collision.

- **ForceMode::VelocityChange**  
  Directly changes the rigidbody’s velocity without considering mass, allowing for instantaneous velocity changes.

---

### **Enum: BodyDefBits**

`BodyDefBits` defines flags that modify the behavior of the rigidbody’s definition. These flags allow for adjusting various properties of the body when creating or configuring it.

- **BodyDefBits::UseGravity**  
  Enables or disables gravity for the rigidbody. When set, the body will be affected by gravity, otherwise, it will not.

- **BodyDefBits::Static**  
  Marks the rigidbody as static. A static body doesn’t move and is unaffected by forces or collisions, typically used for immovable objects like the ground or walls.

Here is the documentation for the `Types` enum in your Lua scripting API:

---

### **Enum: Types**

`Types` defines various data types that can be used in your scripting system. This enum helps specify the type of a value, whether it's a basic type like `bool` or `int`, or a more complex type like `vec2` or `Entity`.

- **Types::Bool**  
  Represents the `bool` type, which can hold a value of either `true` or `false`.

- **Types::Int**  
  Represents the `int` type, which is a 32-bit signed integer used for whole numbers.

- **Types::Float**  
  Represents the `float` type, which is a single-precision floating-point number used for decimal numbers.

- **Types::String**  
  Represents the `string` type, used for sequences of characters (text).

- **Types::Vec2**  
  Represents the `vec2` type, typically a 2D vector containing two floating-point numbers (e.g., for coordinates, direction, or other 2D data).

- **Types::Vec3**  
  Represents the `vec3` type, a 3D vector containing three floating-point numbers (e.g., for position, rotation, or other 3D data).

- **Types::Vec4**  
  Represents the `vec4` type, a 4D vector containing four floating-point numbers (commonly used for color, homogeneous coordinates, or extended vector data).

- **Types::Entity**  
  Represents the `Entity` type, which typically refers to an object in the game world with components like transform, physics, etc.

- **Types::Color**  
  Represents the `color` type, commonly used to define color values (usually represented as a `vec4` with RGBA channels).


---

## Entity

Represents an entity in your game or scene, allowing access to various components attached to it and providing methods for manipulating them.

- **Constructors:**
  - `Entity.new()`: Creates a new empty `Entity` instance.

- **Methods:**

  #### **Component Accessors**
  These methods allow you to get various components attached to the entity:

  - **destroy**(): `void`  
    Destroys the entity, removing it from the scene or game world.

  - **get_scripts**(): `evscript_c`  
    Returns the script component (`evscript_c`) attached to the entity.

  - **get_tag**(): `tag_c`  
    Returns the tag component (`tag_c`) of the entity.

  - **get_transform**(): `transform_c`  
    Returns the transform component (`transform_c`), typically used to manage position, rotation, and scale.

  - **get_rigidbody2d**(): `rigidbody2d_c`  
    Returns the 2D rigidbody component (`rigidbody2d_c`) of the entity.

  - **get_spriteRenderer**(): `spriteRenderer_c`  
    Returns the sprite renderer component (`spriteRenderer_c`), used for rendering sprites on the entity.

  - **get_circleRenderer**(): `circleRenderer_c`  
    Returns the circle renderer component (`circleRenderer_c`), used for rendering circles.

  - **get_springJoint2d**(): `springJoint2d_c`  
    Returns the 2D spring joint component (`springJoint2d_c`), used for adding spring joints between entities.

  - **get_camera**(): `camera_c`  
    Returns the camera component (`camera_c`) attached to the entity.

  - **get_circleCollider2d**(): `circleCollider2d_c`  
    Returns the 2D circle collider component (`circleCollider2d_c`) attached to the entity.

  - **get_boxCollider2d**(): `boxCollider2d_c`  
    Returns the 2D box collider component (`boxCollider2d_c`) attached to the entity.

  #### **Component Adder**
  These methods add the respective component to the entity:

  - **add_rigidbody2d**(): `rigidbody2d_c`  
    Adds a 2D rigidbody component (`rigidbody2d_c`) to the entity.

  - **add_spriteRenderer**(): `spriteRenderer_c`  
    Adds a sprite renderer component (`spriteRenderer_c`) to the entity.

  - **add_circleRenderer**(): `circleRenderer_c`  
    Adds a circle renderer component (`circleRenderer_c`) to the entity.

  - **add_springJoint2d**(): `springJoint2d_c`  
    Adds a 2D spring joint component (`springJoint2d_c`) to the entity.

  - **add_camera**(): `camera_c`  
    Adds a camera component (`camera_c`) to the entity.

  - **add_circleCollider2d**(): `circleCollider2d_c`  
    Adds a 2D circle collider component (`circleCollider2d_c`) to the entity.

  - **add_boxCollider2d**(): `boxCollider2d_c`  
    Adds a 2D box collider component (`boxCollider2d_c`) to the entity.

  #### **Component Checkers**
  These methods check if a specific component exists on the entity:

  - **has_rigidbody2d**(): `bool`  
    Returns `true` if the entity has a 2D rigidbody component attached.

  - **has_spriteRenderer**(): `bool`  
    Returns `true` if the entity has a sprite renderer component attached.

  - **has_circleRenderer**(): `bool`  
    Returns `true` if the entity has a circle renderer component attached.

  - **has_springJoint2d**(): `bool`  
    Returns `true` if the entity has a 2D spring joint component attached.

  - **has_camera**(): `bool`  
    Returns `true` if the entity has a camera component attached.

  - **has_circleCollider2d**(): `bool`  
    Returns `true` if the entity has a 2D circle collider component attached.

  - **has_boxCollider2d**(): `bool`  
    Returns `true` if the entity has a 2D box collider component attached.

  #### **Component Removal**
  These methods remove the respective component from the entity:

  - **remove_rigidbody2d**(): `void`  
    Removes the 2D rigidbody component from the entity.

  - **remove_spriteRenderer**(): `void`  
    Removes the sprite renderer component from the entity.

  - **remove_circleRenderer**(): `void`  
    Removes the circle renderer component from the entity.

  - **remove_springJoint2d**(): `void`  
    Removes the 2D spring joint component from the entity.

  - **remove_camera**(): `void`  
    Removes the camera component from the entity.

  - **remove_circleCollider2d**(): `void`  
    Removes the 2D circle collider component from the entity.

  - **remove_boxCollider2d**(): `void`  
    Removes the 2D box collider component from the entity.

---

#### **Summary of Operations**

This `Entity` class provides a powerful way to manage game objects, allowing you to:

- **add_...** components like physics, rendering, and collision detection.
- **get_...** components attached to the entity to manipulate or query data.
- **check_...** if certain components are present on the entity.
- **remove_...** components from the entity as needed.

The methods provide a flexible interface for manipulating the entity's components directly from Lua, allowing you to add, access, check, and remove game object components efficiently.

---

## Components

#### **tag_c**

Represents a tag associated with an entity in your game, typically used for identifying or categorizing entities (e.g., "player", "enemy", etc.).

- **Methods:**

  - **entity**(): `Entity&`  
    Returns the `Entity` associated with this `tag_c`.

  - **name**(): `string`  
    Returns the name of the tag as a `string`.

  - **tag**(): `string`  
    Returns the tag itself as a `string`.

---

#### **transform_c**

Represents an entity's transformation, including position, rotation, and scale in 3D space. Used for modifying the entity's transformation in the scene.

- **Methods:**

  - **entity**(): `Entity&`  
    Returns the `Entity` associated with this `transform_c`.

  - **position**(): `vec3`  
    Returns the position (`vec3`) of the entity.

  - **rotation**(): `vec3`  
    Returns the rotation (`vec3`) of the entity, typically in Euler angles.

  - **scale**(): `vec3`  
    Returns the scale (`vec3`) of the entity.

  - **translate**(vec3 d): `void`  
    Translates (moves) the entity by the vector `d`, modifying its position in place.

  - **rotate**(vec3 d): `void`  
    Rotates the entity by the vector `d`, modifying its rotation in place.

---

#### **spriteRenderer_c**

The `spriteRenderer_c` component is used to render a 2D sprite on an entity. It handles properties such as visibility, color, and texture mapping.

- **Methods:**

  - **entity**(): `Entity&`  
    Returns the `Entity` associated with this `spriteRenderer_c`.

  - **active**(): `bool`  
    Returns whether the sprite renderer is active. If `false`, the sprite is not rendered.

  - **color**(): `Color`  
    Returns the color of the sprite, typically represented as a `vec4` (RGBA).

  - **startUV**(): `vec2`  
    Returns the starting UV coordinates of the texture for the sprite.

  - **sizeUV**(): `vec2`  
    Returns the size of the UV coordinates, determining how much of the texture is applied to the sprite.

---

#### **circleRenderer_c**

The `circleRenderer_c` component is used to render a circle on an entity. It also handles properties like color, size, and texture mapping, as well as additional properties for circle rendering.

- **Methods:**

  - **entity**(): `Entity&`  
    Returns the `Entity` associated with this `circleRenderer_c`.

  - **active**(): `bool`  
    Returns whether the circle renderer is active. If `false`, the circle is not rendered.

  - **color**(): `Color`  
    Returns the color of the circle, typically represented as a `vec4` (RGBA).

  - **startUV**(): `vec2`  
    Returns the starting UV coordinates for the texture applied to the circle.

  - **sizeUV**(): `vec2`  
    Returns the size of the UV coordinates, affecting how much of the texture is mapped to the circle.

  - **thickness**(): `f32`  
    Returns the thickness (`f32`) of the circle's border, used for rendering the circle as a ring or filled shape.

  - **fade**(): `f32`  
    Returns the fade value (`f32`), which controls how transparent the circle is. Typically a value between `0` (completely transparent) and `1` (fully opaque).

---


#### **rigidbody2d_c**

Represents a 2D rigidbody, which is a component that provides physics-based behavior to an entity. This component handles velocity, force application, mass, and other physical properties related to movement and interaction in a 2D environment.

- **Methods:**

  - **entity**(): `Entity&`  
    Returns the `Entity` associated with this `rigidbody2d_c`.

  - **active**(): `bool`  
    Returns whether the rigidbody is active. An inactive rigidbody does not participate in physics simulations.

  - **velocity**(): `vec2`  
    Returns the current velocity (`vec2`) of the rigidbody.

  - **angularVelocity**(): `f32`  
    Returns the angular velocity (`f32`) of the rigidbody, which defines its rotation rate.

  - **drag**(): `f32`  
    Returns the drag (`f32`) applied to the rigidbody, affecting how its velocity slows over time.

  - **useGravity**(): `bool`  
    Returns whether gravity is applied to this rigidbody. If `true`, the rigidbody will be affected by gravity; if `false`, it will not.

  - **addForce**(vec2 force): `void`  
    Adds the specified force (`vec2`) to the rigidbody. This force will affect the rigidbody's velocity based on the mass and other properties.

  - **addForceAtOffset**(vec2 force, vec2 offset): `void`  
    Adds a force (`vec2`) at a specific offset (`vec2`) from the rigidbody's center of mass. This is typically used for applying torques or forces at a specific point in the rigidbody.

  - **setMass**(f32 mass): `void`  
    Sets the mass (`f32`) of the rigidbody. The mass affects how forces are applied and how the object behaves under physics simulations.

  - **getMass**(): `f32`  
    Returns the mass (`f32`) of the rigidbody.

  - **getAcceleration**(): `vec2`  
    Returns the current acceleration (`vec2`) of the rigidbody based on its velocity and forces acting on it.

---

#### **springJoint2d_c**

The `springJoint2d_c` component simulates a 2D spring joint between two entities. It uses properties like the spring constant, damping, and rest length to create realistic spring physics.

- **Methods:**

  - **entity**(): `Entity&`  
    Returns the `Entity` associated with this `springJoint2d_c`.

  - **active**(): `bool`  
    Returns whether the spring joint is active. If `false`, the spring joint is not simulated.

  - **anchor**(): `vec2`  
    Returns the anchor point of the spring joint, typically the point where the spring is attached to the first entity.

  - **offset**(): `vec2`  
    Returns the offset of the spring joint relative to the anchor point.

  - **springConstant**(): `f32`  
    Returns the spring constant, which controls the stiffness of the spring.

  - **damping**(): `f32`  
    Returns the damping factor, which controls the friction or resistance in the spring system.

  - **restLength**(): `f32`  
    Returns the rest length of the spring, which is the length at which the spring is neither compressed nor stretched.

---

#### **circleprops_t**

The `circleprops_t` structure represents the properties of a 2D circle collider, specifically its offset and radius.

- **Methods:**

  - **offset**(): `vec2`  
    Returns the offset position of the circle collider relative to the entity's center.

  - **radius**(): `f32`  
    Returns the radius of the circle collider.

---

#### **circleCollider2d_c**

The `circleCollider2d_c` component simulates a 2D circular collider on an entity. It includes properties like the circle's restitution (bounciness) and the associated collider shape.

- **Methods:**

  - **entity**(): `Entity&`  
    Returns the `Entity` associated with this `circleCollider2d_c`.

  - **active**(): `bool`  
    Returns whether the circle collider is active. If `false`, the collider does not participate in collision detection.

  - **circle**(): `circleprops_t&`  
    Returns the properties of the circle collider, including its offset and radius.

  - **restitution**(): `f32`  
    Returns the restitution (bounciness) of the circle collider. A value of `1.0` means a perfectly elastic collision.

---

#### **box2dprops_t**

The `box2dprops_t` structure represents the properties of a 2D box collider, including its offset and half-extents.

- **Methods:**

  - **offset**(): `vec2`  
    Returns the offset position of the box collider relative to the entity's center.

  - **halfExtents**(): `vec2`  
    Returns the half-extents of the box collider, which define the width and height (half of the full dimensions) of the box.

---

#### **boxCollider2d_c**

The `boxCollider2d_c` component simulates a 2D rectangular collider on an entity. It includes properties like the box's restitution and associated collider shape.

- **Methods:**

  - **entity**(): `Entity&`  
    Returns the `Entity` associated with this `boxCollider2d_c`.

  - **active**(): `bool`  
    Returns whether the box collider is active. If `false`, the collider does not participate in collision detection.

  - **box**(): `box2dprops_t&`  
    Returns the properties of the box collider, including its offset and half-extents.

  - **restitution**(): `f32`  
    Returns the restitution (bounciness) of the box collider. A value of `1.0` means a perfectly elastic collision.

---

#### **scriptHandler_t**

The `scriptHandler_t` class provides functions for interacting with scripts attached to entities. It allows you to get values of different types and call functions from the script.

- **Methods:**

  - **get<i32>**(): `i32`  
    Gets a value of type `i32` from the script.

  - **get<f64>**(): `f64`  
    Gets a value of type `f64` from the script.

  - **get<bool>**(): `bool`  
    Gets a value of type `bool` from the script.

  - **get<vec2>**(): `vec2`  
    Gets a value of type `vec2` from the script.

  - **get<vec3>**(): `vec3`  
    Gets a value of type `vec3` from the script.

  - **get<vec4>**(): `vec4`  
    Gets a value of type `vec4` from the script.

  - **get<Entity>**(): `Entity`  
    Gets a value of type `Entity` from the script.

  - **get<std::string>**(): `std::string`  
    Gets a value of type `std::string` from the script.

  - **call**(): `void`  
    Calls a function in the script. This allows the execution of a script's function from C++.

---

#### **evscript_c**

The `evscript_c` class represents the script component attached to an entity. It allows you to retrieve the associated `scriptHandler_t` and control the script's activation state.

- **Methods:**

  - **get**(): `scriptHandler_t&`  
    Returns the `scriptHandler_t` associated with this script component, allowing interaction with the script attached to the entity.

  - **active**(): `bool`  
    Returns whether the script is active. If `false`, the script is inactive and will not be executed.

  - **entity**(): `Entity&`  
    Returns the `Entity` associated with this script component.

---

#### **collision2d_t**

The `collision2d_t` class represents a 2D collision event between two entities. It provides information about the other colliding entity, the contact point, the normal of the collision, and the penetration depth.

- **Methods:**

  - **other**(): `Entity&`  
    Returns the `Entity` that this entity is colliding with. The "other" entity in the collision.

  - **contact**(): `vec2`  
    Returns the contact point of the collision, typically where the two colliders touch.

  - **normal**(): `vec2`  
    Returns the collision normal, representing the direction in which the colliders are separating.

  - **penetration**(): `f32`  
    Returns the penetration depth of the collision, which is how deep the colliders are intersecting.

---


#### **camera_c**

The `camera_c` class represents the camera component of an entity. It controls the projection settings, field of view (FOV), aspect ratio, and other properties related to the camera's functionality. It also provides methods for managing 2D and 3D camera modes.

- **Members:**

  - **entity**: `Entity&`  
    The entity associated with this camera. This allows access to the entity to which the camera component is attached.

  - **active**: `bool&`  
    A flag indicating whether the camera is active or not. If `false`, the camera is inactive and will not affect the rendering.

  - **fixedAspect**: `bool&`  
    A flag that controls whether the aspect ratio of the camera is fixed. If `true`, the camera will maintain its aspect ratio when the window is resized.

### **Methods:**

  - **getProjection**(): `mat4`  
    Returns the projection matrix of the camera. This matrix is used to transform 3D coordinates to 2D screen coordinates.

  - **getLenssize**(): `f32`  
    Returns the lens size (or zoom) of the camera. This affects the field of view and rendering.

  - **getFov**(): `f32`  
    Returns the camera's field of view (FOV), which determines how much of the scene is visible to the camera.

  - **getAspect**(): `f32`  
    Returns the aspect ratio of the camera, which is the width of the screen divided by the height.

  - **getNear**(): `f32`  
    Returns the near clipping plane distance of the camera. This defines how close objects can be to the camera before being clipped.

  - **getFar**(): `f32`  
    Returns the far clipping plane distance of the camera. This defines how far objects can be from the camera before being clipped.

  - **setLenssize**(f32 lensSize): `void`  
    Sets the lens size (zoom level) of the camera. This affects the field of view and rendering.

  - **setFov**(f32 fov): `void`  
    Sets the camera's field of view (FOV). A higher FOV gives a wider view of the scene.

  - **setAspect**(f32 aspect): `void`  
    Sets the aspect ratio of the camera. This controls the width-to-height ratio of the camera's view.

  - **setNear**(f32 nearDistance): `void`  
    Sets the near clipping plane distance of the camera. Objects closer than this value will be clipped.

  - **setFar**(f32 farDistance): `void`  
    Sets the far clipping plane distance of the camera. Objects farther than this value will be clipped.

  - **is2d**(): `bool`  
    Checks if the camera is currently in 2D mode. Returns `true` if in 2D mode.

  - **is3d**(): `bool`  
    Checks if the camera is currently in 3D mode. Returns `true` if in 3D mode.

  - **set2d**(): `void`  
    Switches the camera to 2D mode. This will adjust the projection to be orthographic and remove depth perception.

  - **set3d**(): `void`  
    Switches the camera to 3D mode. This will adjust the projection to be perspective-based and enable depth perception.

---

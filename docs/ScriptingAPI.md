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



## Extended Types

1. __vec2__
    - __Constructor__
    - __Members__
        - s
        - s
    - __Methods__
        - s
        - s
2. __vec3__
    - __Constructor__
    - __Members__
        - s
        - s
    - __Methods__
        - s
        - s
3. __vec4__
    - __Constructor__
    - __Members__
        - s
        - s
    - __Methods__
        - s
        - s


### Entity
- __Constructor__
- __Members__
    - s
    - s
- __Methods__
    - s
    - s

## Components

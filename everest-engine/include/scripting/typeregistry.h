/*
 * ======= Type registry =======
 * Author: Sunil Sapkota
 * Description: Registry of all the types extended for scripting in lua. It has some simple mathematical
 * types to more complex components, managers, etc.
 */

#pragma once

#include "sol.hpp"
#include <functional>

namespace Everest {
namespace Scripting {
    
    using luastate_t = sol::state;
    using lualibs = sol::lib;
    struct evscript_c;

    void reg_vecx(luastate_t& lua);
    void reg_matx(luastate_t& lua);
    void reg_entity(luastate_t& lua);
    void reg_tag(luastate_t& lua);
    void reg_transform(luastate_t& lua);
    void reg_rigidbody2d(luastate_t& lua);
    void reg_spriteRenderer(luastate_t& lua);
    void reg_circleRenderer(luastate_t& lua);
    void reg_springJoint2d(luastate_t& lua);
    void reg_circleCollider2d(luastate_t& lua);
    void reg_boxCollider2d(luastate_t& lua);
    void reg_collision(luastate_t& lua);
    void reg_camera(luastate_t& lua);
    void reg_input(luastate_t& lua);
    void reg_time(luastate_t& lua);
    void reg_scene(luastate_t& lua);

    

    static const std::vector<std::function<void(luastate_t&)>> Registry{
        // types
        reg_vecx,
        reg_matx,
        reg_collision,

        // components
        reg_transform,
        reg_tag,
        reg_rigidbody2d,
        reg_spriteRenderer,
        reg_circleRenderer,
        reg_springJoint2d,
        reg_circleCollider2d,
        reg_boxCollider2d,
        reg_camera,

        // entity
        reg_entity,

        // engine extensions
        reg_input,
        reg_time,
        reg_scene,
    };
    



    inline void registerTypes(luastate_t& lua){
        for(auto& regfunc : Registry){
            regfunc(lua);
        }
    }
}
}

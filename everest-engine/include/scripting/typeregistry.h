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
    void reg_entity(luastate_t& lua);
    void reg_tag(luastate_t& lua);
    void reg_transform(luastate_t& lua);
    void reg_rigidbody2d(luastate_t& lua);
    void reg_spriteRenderer(luastate_t& lua);
    void reg_circleRenderer(luastate_t& lua);
    void reg_springJoint2d(luastate_t& lua);
    void reg_circleCollider2d(luastate_t& lua);
    void reg_boxCollider2d(luastate_t& lua);
    

    static const std::vector<std::function<void(luastate_t&)>> Registry{
        // types
        reg_vecx,

        // components
        reg_transform,
        reg_tag,
        reg_rigidbody2d,
        reg_spriteRenderer,
        reg_circleRenderer,
        reg_springJoint2d,
        reg_circleCollider2d,
        reg_boxCollider2d,

        // entity
        reg_entity,

        // engine extensions
    };
    



    inline void registerTypes(luastate_t& lua){
        for(auto& regfunc : Registry){
            regfunc(lua);
        }
    }
}
}

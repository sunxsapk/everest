/*
 * ======= Type registry =======
 * Author: Sunil Sapkota
 * Description: Registry of all the types extended for scripting in lua. It has some simple mathematical
 * types to more complex components, managers, etc.
 */

#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include "sol.hpp"
#include <functional>

namespace Everest {
namespace Scripting {

    enum class Types {
        Int,
        Bool,
        Float,
        String,
        Vec2,
        Vec3,
        Vec4,
        Color,
        Entity,
    };
    
    using luastate_t = sol::state;
    using lualibs = sol::lib;
    struct evscript_c;

    void reg_types(luastate_t& lua);
    void reg_vecx(luastate_t& lua);
    void reg_matx(luastate_t& lua);
    void reg_entity(luastate_t& lua);
    void reg_tag(luastate_t& lua);
    void reg_script(luastate_t& lua);
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


    void registerTypes(luastate_t& lua);
}
}

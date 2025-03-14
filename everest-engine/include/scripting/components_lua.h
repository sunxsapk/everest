#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include "sol.hpp"



namespace Everest {
    namespace Scripting {
        using luastate_t = sol::state;
        using lualibs = sol::lib;


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
    }
}
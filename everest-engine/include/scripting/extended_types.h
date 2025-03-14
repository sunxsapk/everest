#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include "sol.hpp"

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

        void reg_types(luastate_t& lua);
        void reg_vecx(luastate_t& lua);
        void reg_matx(luastate_t& lua);
        void reg_entity(luastate_t& lua);

    }
}
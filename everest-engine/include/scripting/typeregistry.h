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

    inline void reg_vecx(luastate_t& lua){
        lua.new_usertype<vec2>("vec2",
                "x", &vec2::x,
                "y", &vec2::y,
                "add", [](vec2& a, vec2 b){a += b;},
                "mul", [](vec2& a, vec2 b){a *= b;},
                "div", [](vec2& a, vec2 b){a /= b;},
                "sub", [](vec2& a, vec2 b){a -= b;}
            );
        lua.new_usertype<vec3>("vec3",
                "x", &vec3::x,
                "y", &vec3::y,
                "z", &vec3::z,
                "add", [](vec3& a, vec3 b){a += b;},
                "mul", [](vec3& a, vec3 b){a *= b;},
                "div", [](vec3& a, vec3 b){a /= b;},
                "sub", [](vec3& a, vec3 b){a -= b;}
            );
        lua.new_usertype<vec4>("vec4",
                "x", &vec4::x,
                "y", &vec4::y,
                "z", &vec4::z,
                "w", &vec4::w,
                "add", [](vec4& a, vec4 b){a += b;},
                "mul", [](vec4& a, vec4 b){a *= b;},
                "div", [](vec4& a, vec4 b){a /= b;},
                "sub", [](vec4& a, vec4 b){a -= b;}
            );
    }

    static const std::vector<std::function<void(luastate_t&)>> Registry{
        reg_vecx,
    };
    



    inline void registerTypes(luastate_t& lua){
        for(auto& regfunc : Registry){
            regfunc(lua);
        }
    }
}
}

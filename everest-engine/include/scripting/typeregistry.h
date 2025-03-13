/*
 * ======= Type registry =======
 * Author: Sunil Sapkota
 * Description: Registry of all the types extended for scripting in lua. It has some simple mathematical
 * types to more complex components, managers, etc.
 */

#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include "sol.hpp"

namespace Everest {
namespace Scripting {
    
    using luastate_t = sol::state;
    using lualibs = sol::lib;

    void reg_time(luastate_t& lua);
    void reg_scene(luastate_t& lua);


    void registerTypes(luastate_t& lua);
}
}

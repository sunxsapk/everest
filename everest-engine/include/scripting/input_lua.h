#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include "sol.hpp"

namespace Everest {
    namespace Scripting {

        using luastate_t = sol::state;
        using lualibs = sol::lib;

        void reg_input(luastate_t& lua);

    }
}
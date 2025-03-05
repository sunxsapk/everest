#pragma once

#include "scene/entity.h"

namespace Everest {

    struct component_t {
        Entity entity;
        bool active = true;

        component_t(){}
        component_t(Entity ent):entity(ent){}

        friend class Scene;
    };

}

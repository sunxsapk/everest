/*
 * ============= Components ===============
 * Author: Sunil Sapkota
 * Description : Contains all of the components made available by the engine.
 */
#pragma once

#include "camera.h"
#include "physics/colliders2d.h"
#include "scene/def_components.h"
#include "scriptable.h"
#include "renderer/spriteRenderer.h"
#include "renderer/circleRenderer.h"
#include "physics/rigidbody.h"
#include "physics/springJoint.h"
#include "scripting/evscript.h"

namespace Everest {

    using Transform = transform_c;
    using Tag = tag_c;
    using Camera = camera_c;
    using SpriteRenderer = spriteRenderer_c;
    using CircleRenderer = circleRenderer_c;
    using Rigidbody2d = rigidbody2d_c;
    using SpringJoint2d = springJoint2d_c;
    using BoxCollider2d = boxCollider2d_c;
    using CircleCollider2d = circleCollider2d_c;
    using EvScript = Scripting::evscript_c;

/*#ifndef __NO_3D__
    using Rigidbody = rigidbody_c;
    using SpringJoint = springJoint_c;
#endif*/

    using NativeScript = nativeScript_c;
    using Collider2D = collider2d_t;
}

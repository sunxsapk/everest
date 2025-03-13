#include "scripting/components_lua.h"
#include "scene/components.h"

namespace Everest {
	namespace Scripting {
	
        void reg_tag(luastate_t& lua) {
            lua.new_usertype<tag_c>("tag_c",
                "entity", &tag_c::entity,
                "name", &tag_c::tag,
                "tag", &tag_c::tag
            );
        }

        void reg_transform(luastate_t& lua) {
            lua.new_usertype<transform_c>("transform_c",
                "entity", &transform_c::entity,
                "position", &transform_c::position,
                "rotation", &transform_c::rotation,
                "scale", &transform_c::scale,
                "translate", [](transform_c& tfr, vec3 d) {tfr.position += d; },
                "rotate", [](transform_c& tfr, vec3 d) {tfr.rotation += d; }
            );
        }

        void reg_rigidbody2d(luastate_t& lua) {

            lua.new_enum("ForceMode",
                "Force", ForceMode::Force,
                "Acceleration", ForceMode::Acceleration,
                "Impulse", ForceMode::Impulse,
                "VelocityChange", ForceMode::VelocityChange
            );

            lua.new_enum("BodyDefBits",
                "UseGravity", BodyDefBits::UseGravity,
                "Static", BodyDefBits::Static
            );

            lua.new_usertype<rigidbody2d_c>("rigidbody2d_c",
                "entity", &rigidbody2d_c::entity,
                "active", &rigidbody2d_c::active,
                "velocity", &rigidbody2d_c::velocity,
                "angularVelocity", &rigidbody2d_c::angularVelocity,
                "drag", &rigidbody2d_c::drag,
                "useGravity", &rigidbody2d_c::definition,
                "addForce", &rigidbody2d_c::addForce,
                "addForceAtOffset", &rigidbody2d_c::addForceAtOffset,
                "setMass", &rigidbody2d_c::setMass,
                "getMass", &rigidbody2d_c::getMass,
                "getAcceleration", &rigidbody2d_c::getAcceleration
            );
        }

        void reg_spriteRenderer(luastate_t& lua) {
            lua.new_usertype<spriteRenderer_c>("spriteRenderer_c",
                "entity", &spriteRenderer_c::entity,
                "active", &spriteRenderer_c::active,
                "color", &spriteRenderer_c::color,
                "startUV", &spriteRenderer_c::startUV,
                "sizeUV", &spriteRenderer_c::sizeUV
            );
        }

        void reg_circleRenderer(luastate_t& lua) { // TODO: marshall texture to lua
            lua.new_usertype<circleRenderer_c>("circleRenderer_c",
                "entity", &circleRenderer_c::entity,
                "active", &circleRenderer_c::active,
                "color", &circleRenderer_c::color,
                "startUV", &circleRenderer_c::startUV,
                "sizeUV", &circleRenderer_c::sizeUV,
                "thickness", &circleRenderer_c::thickness,
                "fade", &circleRenderer_c::fade
            );
        }

        void reg_springJoint2d(luastate_t& lua) {
            lua.new_usertype<springJoint2d_c>("springJoint2d_c",
                "entity", &springJoint2d_c::entity,
                "active", &springJoint2d_c::active,
                "anchor", &springJoint2d_c::anchor,
                "offset", &springJoint2d_c::offset,
                "springConstant", &springJoint2d_c::springConstant,
                "damping", &springJoint2d_c::damping,
                "restLength", &springJoint2d_c::restLength
            );
        }

        void reg_circleCollider2d(luastate_t& lua) {
            lua.new_usertype<circleprops_t>("circleprops_t",
                "offset", &circleprops_t::offset,
                "radius", &circleprops_t::radius
            );

            lua.new_usertype<circleCollider2d_c>("circleCollider2d_c",
                "entity", &circleCollider2d_c::entity,
                "active", &circleCollider2d_c::active,
                "circle", &circleCollider2d_c::circle,
                "restitution", &circleCollider2d_c::restitution
            );
        }

        void reg_boxCollider2d(luastate_t& lua) {
            lua.new_usertype<box2dprops_t>("box2dprops_t",
                "offset", &box2dprops_t::offset,
                "halfExtents", &box2dprops_t::halfExtents
            );

            lua.new_usertype<boxCollider2d_c>("boxCollider2d_c",
                "entity", &boxCollider2d_c::entity,
                "active", &boxCollider2d_c::active,
                "box", &boxCollider2d_c::box,
                "restitution", &boxCollider2d_c::restitution
            );
        }

        void reg_script(luastate_t& lua) {
            lua.new_usertype<scriptHandler_t>("scriptHandler_t",
                "get_i", &scriptHandler_t::get<i32>,
                "get_f", &scriptHandler_t::get<f64>,
                "get_b", &scriptHandler_t::get<bool>,
                "get_v2", &scriptHandler_t::get<vec2>,
                "get_v3", &scriptHandler_t::get<vec3>,
                "get_v4", &scriptHandler_t::get<vec4>,
                "get_e", &scriptHandler_t::get<Entity>,
                "get_s", &scriptHandler_t::get<std::string>,
                "call", &scriptHandler_t::call
            );
            lua.new_usertype<evscript_c>("evscript_c",
                "get", &evscript_c::tryGetScriptHandler,
                "active", &evscript_c::active,
                "entity", &evscript_c::entity
            );
        }

        void reg_collision(luastate_t& lua) {
            lua.new_usertype<collision2d_t>("collision2d_t",
                "other", &collision2d_t::other,
                "contact", &collision2d_t::contact,
                "normal", &collision2d_t::normal,
                "penetration", &collision2d_t::penetration
            );
        }

        void reg_camera(luastate_t& lua) {
            lua.new_usertype<camera_c>("camera_c",
                "entity", &camera_c::entity,
                "active", &camera_c::active,
                "fixedAspect", &camera_c::fixedAspect,

                "getProjection", &camera_c::getProjection,
                "getLenssize", &camera_c::get_lenssize,
                "getFov", &camera_c::get_fov,
                "getAspect", &camera_c::get_aspect,
                "getNear", &camera_c::get_near,
                "getFar", &camera_c::get_far,

                "setLenssize", &camera_c::set_lenssize,
                "setFov", &camera_c::set_fov,
                "setAspect", &camera_c::set_aspect,
                "setNear", &camera_c::set_near,
                "setFar", &camera_c::set_far,

                "is2d", &camera_c::is2d,
                "is3d", &camera_c::is3d,
                "set2d", &camera_c::set2d,
                "set3d", &camera_c::set3d
            );
        }
	
	}
}
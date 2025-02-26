#include "scripting/typeregistry.h"
#include "math/types.h"
#include "scene/components.h"
#include "scene/entity.h"

namespace Everest {
namespace Scripting {

    void reg_vecx(luastate_t& lua){
        lua.new_usertype<vec2>("vec2",
                sol::constructors<vec2(f32, f32), vec2(f32), vec2()>(),
                "x", &vec2::x,
                "y", &vec2::y,
                "add", [](vec2& a, vec2 b){a += b;},
                "mul", [](vec2& a, vec2 b){a *= b;},
                "div", [](vec2& a, vec2 b){a /= b;},
                "sub", [](vec2& a, vec2 b){a -= b;}
                );
        lua.new_usertype<vec3>("vec3",
                sol::constructors<vec3(f32, f32, f32), vec3(f32), vec3()>(),
                "x", &vec3::x,
                "y", &vec3::y,
                "z", &vec3::z,
                "add", [](vec3& a, vec3 b){a += b;},
                "mul", [](vec3& a, vec3 b){a *= b;},
                "div", [](vec3& a, vec3 b){a /= b;},
                "sub", [](vec3& a, vec3 b){a -= b;}
                );
        lua.new_usertype<vec4>("vec4",
                sol::constructors<vec4(f32, f32, f32, f32), vec4(f32), vec4()>(),
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

    void reg_tag(luastate_t& lua){
        lua.new_usertype<tag_c>("tag_c",
                "name", &tag_c::tag,
                "tag", &tag_c::tag
                );
    }

    void reg_transform(luastate_t& lua){
        lua.new_usertype<transform_c>("transform_c",
                "position", &transform_c::position,
                "rotation", &transform_c::rotation,
                "scale", &transform_c::scale,
                "translate", [](transform_c& tfr, vec3 d){tfr.position += d;},
                "rotate", [](transform_c& tfr, vec3 d){tfr.rotation += d;}
                );
    }

    void reg_rigidbody2d(luastate_t& lua){

        lua.new_enum("ForceMode",
                "Force", ForceMode::Force,
                "Acceleration", ForceMode::Acceleration,
                "Impulse", ForceMode::Impulse,
                "VelocityChange", ForceMode::VelocityChange
                );

        lua.new_usertype<rigidbody2d_c>("rigidbody2d_c",
                "velocity", &rigidbody2d_c::velocity,
                "angularVelocity", &rigidbody2d_c::angularVelocity,
                "drag", &rigidbody2d_c::drag,
                "useGravity", &rigidbody2d_c::useGravity,
                "useGravity", &rigidbody2d_c::useGravity,
                "addForce", &rigidbody2d_c::addForce,
                "addForceAtOffset", &rigidbody2d_c::addForceAtOffset,
                "setMass", &rigidbody2d_c::setMass,
                "getMass", &rigidbody2d_c::getMass,
                "getAcceleration", &rigidbody2d_c::getAcceleration
                );
    }

    void reg_spriteRenderer(luastate_t& lua){
        lua.new_usertype<spriteRenderer_c>("spriteRenderer_c",
                "color", &spriteRenderer_c::color,
                "startUV", &spriteRenderer_c::startUV,
                "sizeUV", &spriteRenderer_c::sizeUV
                );
    }

    void reg_circleRenderer(luastate_t& lua){
        lua.new_usertype<circleRenderer_c>("circleRenderer_c",
                "color", &circleRenderer_c::color,
                "thickness", &circleRenderer_c::thickness,
                "fade", &circleRenderer_c::fade
                );
    }

    void reg_springJoint2d(luastate_t& lua){
        lua.new_usertype<springJoint2d_c>("springJoint2d_c",
                "anchor", &springJoint2d_c::anchor,
                "offset", &springJoint2d_c::offset,
                "springConstant", &springJoint2d_c::springConstant,
                "damping", &springJoint2d_c::damping,
                "restLength", &springJoint2d_c::restLength
                );
    }

    void reg_circleCollider2d(luastate_t& lua){
        lua.new_usertype<circleprops_t>("circleprops_t",
                "offset", &circleprops_t::offset,
                "radius", &circleprops_t::radius
                );

        lua.new_usertype<circleCollider2d_c>("circleCollider2d_c",
                "circle", &circleCollider2d_c::circle,
                "restitution", &circleCollider2d_c::restitution
                );
    }

    void reg_boxCollider2d(luastate_t& lua){
        lua.new_usertype<box2dprops_t>("box2dprops_t",
                "offset", &box2dprops_t::offset,
                "halfExtents", &box2dprops_t::halfExtents
                );

        lua.new_usertype<boxCollider2d_c>("boxCollider2d_c",
                "box", &boxCollider2d_c::box,
                "restitution", &boxCollider2d_c::restitution
                );
    }

    void reg_collision(luastate_t& lua){
        lua.new_usertype<Collision2D>("Collision2D",
                "other", &Collision2D::other,
                "contact", &Collision2D::contact,
                "normal", &Collision2D::normal,
                "penetration", &Collision2D::penetration
                );
    }

    template<typename T>
    T* _getc(Entity& ent){
        return ent.has<T>()?&ent.get<T>():nullptr;
    }

    void reg_entity(luastate_t& lua){
        lua.new_usertype<Entity>("Entity_t",
                "get_tag", _getc<tag_c>,
                "get_transform", _getc<transform_c>,
                "get_rigidbody2d", _getc<rigidbody2d_c>,
                "get_spriteRenderer", _getc<spriteRenderer_c>,
                "get_circleRenderer", _getc<circleRenderer_c>,
                "get_springJoint2d", _getc<springJoint2d_c>,
                "get_circleCollider2d", _getc<circleCollider2d_c>,
                "get_boxCollider2d", _getc<boxCollider2d_c>
                );
    }

}
}

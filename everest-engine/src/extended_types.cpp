#include "scripting/extended_types.h"
#include "math/types.h"
#include "scene/entity.h"
#include "scene/components.h"

namespace Everest {
	namespace Scripting {

        void reg_types(luastate_t& lua) {
            lua.new_enum("Types",
                "bool", Types::Bool,
                "int", Types::Int,
                "float", Types::Float,
                "string", Types::String,
                "vec2", Types::Vec2,
                "vec3", Types::Vec3,
                "vec4", Types::Vec4,
                "Entity", Types::Entity,
                "color", Types::Color
            );
        }

        void reg_vecx(luastate_t& lua) {
            lua.new_usertype<vec2>("vec2",
                sol::constructors<vec2(f32, f32), vec2(vec2), vec2(f32), vec2()>(),
                "x", &vec2::x,
                "y", &vec2::y,
                "length", [](vec2& a) {return glm::length(a); },
                "normalized", [](vec2& a) {return glm::normalize(a); },
                "add", [](vec2& a, vec2 b) {return a += b; },
                "mul", [](vec2& a, vec2 b) {return a *= b; },
                "div", [](vec2& a, vec2 b) {return a /= b; },
                "smul", [](vec2& a, f32 b) {return a *= b; },
                "sdiv", [](vec2& a, f32 b) {return a /= b; },
                "sub", [](vec2& a, vec2 b) {return a -= b; },
                "dot", [](vec2& a, vec2 b) {return glm::dot(a, b); }
            );
            lua.new_usertype<vec3>("vec3",
                sol::constructors<vec3(f32, f32, f32), vec3(vec3), vec3(f32), vec3()>(),
                "x", &vec3::x,
                "y", &vec3::y,
                "z", &vec3::z,
                "length", [](vec3& a) {return glm::length(a); },
                "normalized", [](vec3& a) {return glm::normalize(a); },
                "add", [](vec3& a, vec3 b) {return a += b; },
                "mul", [](vec3& a, vec3 b) {return a *= b; },
                "div", [](vec3& a, vec3 b) {return a /= b; },
                "smul", [](vec3& a, f32 b) {return a *= b; },
                "sdiv", [](vec3& a, f32 b) {return a /= b; },
                "sub", [](vec3& a, vec3 b) {return a -= b; },
                "dot", [](vec3& a, vec3 b) {return glm::dot(a, b); },
                "cross", [](vec3& a, vec3 b) {return glm::cross(a, b); }
            );
            lua.new_usertype<vec4>("vec4",
                sol::constructors<vec4(f32, f32, f32, f32), vec4(vec4), vec4(f32), vec4()>(),
                "x", &vec4::x,
                "y", &vec4::y,
                "z", &vec4::z,
                "w", &vec4::w,
                "length", [](vec4& a) {return glm::length(a); },
                "normalized", [](vec4& a) {return glm::normalize(a); },
                "add", [](vec4& a, vec4 b) {return a += b; },
                "mul", [](vec4& a, vec4 b) {return a *= b; },
                "div", [](vec4& a, vec4 b) {return a /= b; },
                "smul", [](vec4& a, f32 b) {return a *= b; },
                "sdiv", [](vec4& a, f32 b) {return a /= b; },
                "sub", [](vec4& a, vec4 b) {return a -= b; },
                "dot", [](vec4& a, vec4 b) {return glm::dot(a, b); }
            );
        }

        void reg_matx(luastate_t& lua) {
            lua.new_usertype<glm::mat4>("mat4",
                sol::constructors<mat4(), mat4(f32)>(),
                "add", [](mat4& a, mat4& b) {return a += b; },
                "sub", [](mat4& a, mat4& b) {return a -= b; },
                "mul", [](mat4& a, mat4& b) {return a *= b; },
                "smul", [](mat4& a, f32 b) {return a *= b; }
            );
        }

        template<typename T>
        T* _getc(Entity& ent) {
            return ent.isValid() && ent.has<T>() ? &ent.get<T>() : nullptr;
        }

        template<typename T>
        bool _hasc(Entity& ent) {
            return ent.isValid() && ent.has<T>();
        }

        template<typename T>
        T* _addc(Entity& ent) {
            return ent.isValid() ? &ent.tryAdd<T>() : nullptr;
        }

        template<typename T>
        void _removec(Entity& ent) {
            if (ent.isValid()) ent.tryRemove<T>();
        }

        void reg_entity(luastate_t& lua) {
            lua.new_usertype<Entity>("Entity",
                sol::constructors<Entity()>(),

                "destroy", &Entity::destroy,

                "get_scripts", _getc<evscript_c>,
                "get_tag", _getc<tag_c>,
                "get_transform", _getc<transform_c>,

                "get_rigidbody2d", _getc<rigidbody2d_c>,
                "get_spriteRenderer", _getc<spriteRenderer_c>,
                "get_circleRenderer", _getc<circleRenderer_c>,
                "get_springJoint2d", _getc<springJoint2d_c>,
                "get_camera", _getc<camera_c>,
                "get_circleCollider2d", _getc<circleCollider2d_c>,
                "get_boxCollider2d", _getc<boxCollider2d_c>,

                "add_rigidbody2d", _addc<rigidbody2d_c>,
                "add_spriteRenderer", _addc<spriteRenderer_c>,
                "add_circleRenderer", _addc<circleRenderer_c>,
                "add_springJoint2d", _addc<springJoint2d_c>,
                "add_camera", _addc<camera_c>,
                "add_circleCollider2d", _addc<circleCollider2d_c>,
                "add_boxCollider2d", _addc<boxCollider2d_c>,

                "has_rigidbody2d", _hasc<rigidbody2d_c>,
                "has_spriteRenderer", _hasc<spriteRenderer_c>,
                "has_circleRenderer", _hasc<circleRenderer_c>,
                "has_springJoint2d", _hasc<springJoint2d_c>,
                "has_camera", _hasc<camera_c>,
                "has_circleCollider2d", _hasc<circleCollider2d_c>,
                "has_boxCollider2d", _hasc<boxCollider2d_c>,

                "remove_rigidbody2d", _removec<rigidbody2d_c>,
                "remove_spriteRenderer", _removec<spriteRenderer_c>,
                "remove_circleRenderer", _removec<circleRenderer_c>,
                "remove_springJoint2d", _removec<springJoint2d_c>,
                "remove_camera", _removec<camera_c>,
                "remove_circleCollider2d", _removec<circleCollider2d_c>,
                "remove_boxCollider2d", _removec<boxCollider2d_c>
            );
        }


	}
}
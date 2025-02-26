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

    template<typename T>
    T* _getc(Entity& ent){
        return ent.has<T>()?&ent.get<T>():nullptr;
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
                "scale", &transform_c::scale
                );
    }

    void reg_entity(luastate_t& lua){
        lua.new_usertype<Entity>("Entity_t",
                "get_tag", _getc<tag_c>,
                "get_transform", _getc<transform_c>
                );
    }

}
}

#include "scripting/typeregistry.h"
#include "core/input.h"
#include "core/time.h"
#include "math/types.h"
#include "scene/components.h"
#include "scene/entity.h"
#include "scene/scenemanager.h"

namespace Everest {
namespace Scripting {

    void reg_types(luastate_t& lua){
        lua.new_enum("Types",
                "nil", Types::Nil,
                "int", Types::Int,
                "float", Types::Float,
                "string", Types::String,
                "vec2", Types::Vec2,
                "vec3", Types::Vec3,
                "vec4", Types::Vec4,
                "color", Types::Color,
                "Entity", Types::Entity
                );
    }

    void reg_vecx(luastate_t& lua){
        lua.new_usertype<vec2>("vec2",
                sol::constructors<vec2(f32, f32), vec2(vec2), vec2(f32), vec2()>(),
                "x", &vec2::x,
                "y", &vec2::y,
                "add", [](vec2& a, vec2 b){return a += b;},
                "mul", [](vec2& a, vec2 b){return a *= b;},
                "div", [](vec2& a, vec2 b){return a /= b;},
                "smul", [](vec2& a, f32 b){return a *= b;},
                "sdiv", [](vec2& a, f32 b){return a /= b;},
                "sub", [](vec2& a, vec2 b){return a -= b;},
                "dot", [](vec2& a, vec2 b){return glm::dot(a, b);}
                );
        lua.new_usertype<vec3>("vec3",
                sol::constructors<vec3(f32, f32, f32), vec3(vec3), vec3(f32), vec3()>(),
                "x", &vec3::x,
                "y", &vec3::y,
                "z", &vec3::z,
                "add", [](vec3& a, vec3 b){return a += b;},
                "mul", [](vec3& a, vec3 b){return a *= b;},
                "div", [](vec3& a, vec3 b){return a /= b;},
                "smul", [](vec3& a, f32 b){return a *= b;},
                "sdiv", [](vec3& a, f32 b){return a /= b;},
                "sub", [](vec3& a, vec3 b){return a -= b;},
                "dot", [](vec3& a, vec3 b){return glm::dot(a, b);},
                "cross", [](vec3& a, vec3 b){return glm::cross(a, b);}
                );
        lua.new_usertype<vec4>("vec4",
                sol::constructors<vec4(f32, f32, f32, f32), vec4(vec4), vec4(f32), vec4()>(),
                "x", &vec4::x,
                "y", &vec4::y,
                "z", &vec4::z,
                "w", &vec4::w,
                "add", [](vec4& a, vec4 b){return a += b;},
                "mul", [](vec4& a, vec4 b){return a *= b;},
                "div", [](vec4& a, vec4 b){return a /= b;},
                "smul", [](vec4& a, f32 b){return a *= b;},
                "sdiv", [](vec4& a, f32 b){return a /= b;},
                "sub", [](vec4& a, vec4 b){return a -= b;},
                "dot", [](vec4& a, vec4 b){return glm::dot(a, b);}
                );
    }

    void reg_matx(luastate_t& lua){
        lua.new_usertype<glm::mat4>("mat4",
                sol::constructors<mat4(), mat4(f32)>(),
                "add", [](mat4& a, mat4& b){return a += b;},
                "sub", [](mat4& a, mat4& b){return a -= b;},
                "mul", [](mat4& a, mat4& b){return a *= b;},
                "smul", [](mat4& a, f32 b) {return a *= b;}
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

        lua.new_enum("BodyDefBits",
                "UseGravity", BodyDefBits::UseGravity,
                "Static", BodyDefBits::Static
                );

        lua.new_usertype<rigidbody2d_c>("rigidbody2d_c",
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
        lua.new_usertype<collision2d_t>("collision2d_t",
                "other", &collision2d_t::other,
                "contact", &collision2d_t::contact,
                "normal", &collision2d_t::normal,
                "penetration", &collision2d_t::penetration
                );
    }

    void reg_camera(luastate_t& lua){
        lua.new_usertype<camera_c>("camera_c",
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

    void reg_input(luastate_t& lua){
        lua.new_enum("Keycode",
                "K_unknown", K_unknown,
                "K_space", K_space,
                "K_apostrophe", K_apostrophe,
                "K_comma", K_comma,
                "K_minus", K_minus,
                "K_period", K_period,
                "K_slash", K_slash,
                "K_0", K_0,
                "K_1", K_1,
                "K_2", K_2,
                "K_3", K_3,
                "K_4", K_4,
                "K_5", K_5,
                "K_6", K_6,
                "K_7", K_7,
                "K_8", K_8,
                "K_9", K_9,
                "K_semicolon", K_semicolon,
                "K_equal", K_equal,
                "K_a", K_a,
                "K_b", K_b,
                "K_c", K_c,
                "K_d", K_d,
                "K_e", K_e,
                "K_f", K_f,
                "K_g", K_g,
                "K_h", K_h,
                "K_i", K_i,
                "K_j", K_j,
                "K_k", K_k,
                "K_l", K_l,
                "K_m", K_m,
                "K_n", K_n,
                "K_o", K_o,
                "K_p", K_p,
                "K_q", K_q,
                "K_r", K_r,
                "K_s", K_s,
                "K_t", K_t,
                "K_u", K_u,
                "K_v", K_v,
                "K_w", K_w,
                "K_x", K_x,
                "K_y", K_y,
                "K_z", K_z,
                "K_left_bracket", K_left_bracket,
                "K_backslash", K_backslash,
                "K_right_bracket", K_right_bracket,
                "K_tilde", K_tilde,
                "K_escape", K_escape,
                "K_enter", K_enter,
                "K_tab", K_tab,
                "K_backspace", K_backspace,
                "K_insert", K_insert,
                "K_delete", K_delete,
                "K_right", K_right,
                "K_left", K_left,
                "K_down", K_down,
                "K_up", K_up,
                "K_page_up", K_page_up,
                "K_page_down", K_page_down,
                "K_home", K_home,
                "K_end", K_end,
                "K_caps_lock", K_caps_lock,
                "K_scroll_lock", K_scroll_lock,
                "K_num_lock", K_num_lock,
                "K_print_screen", K_print_screen,
                "K_pause", K_pause,
                "K_f1", K_f1,
                "K_f2", K_f2,
                "K_f3", K_f3,
                "K_f4", K_f4,
                "K_f5", K_f5,
                "K_f6", K_f6,
                "K_f7", K_f7,
                "K_f8", K_f8,
                "K_f9", K_f9,
                "K_f10", K_f10,
                "K_f11", K_f11,
                "K_f12", K_f12,
                "K_f13", K_f13,
                "K_f14", K_f14,
                "K_f15", K_f15,
                "K_f16", K_f16,
                "K_f17", K_f17,
                "K_f18", K_f18,
                "K_f19", K_f19,
                "K_f20", K_f20,
                "K_f21", K_f21,
                "K_f22", K_f22,
                "K_f23", K_f23,
                "K_f24", K_f24,
                "K_f25", K_f25,
                "K_kp_0", K_kp_0,
                "K_kp_1", K_kp_1,
                "K_kp_2", K_kp_2,
                "K_kp_3", K_kp_3,
                "K_kp_4", K_kp_4,
                "K_kp_5", K_kp_5,
                "K_kp_6", K_kp_6,
                "K_kp_7", K_kp_7,
                "K_kp_8", K_kp_8,
                "K_kp_9", K_kp_9,
                "K_kp_decimal", K_kp_decimal,
                "K_kp_divide", K_kp_divide,
                "K_kp_multiply", K_kp_multiply,
                "K_kp_subtract", K_kp_subtract,
                "K_kp_add", K_kp_add,
                "K_kp_enter", K_kp_enter,
                "K_kp_equal", K_kp_equal,
                "K_left_shift", K_left_shift,
                "K_left_control", K_left_control,
                "K_left_alt", K_left_alt,
                "K_left_super", K_left_super,
                "K_right_shift", K_right_shift,
                "K_right_control", K_right_control,
                "K_right_alt", K_right_alt,
                "K_right_super", K_right_super,
                "K_menu", K_menu,
                "K_last", K_last
                    );

        lua.new_enum("MouseButton",
                "M_0", M_0,
                "M_left", M_left,
                "M_1", M_1,
                "M_right", M_right,
                "M_2", M_2,
                "M_middle", M_middle,
                "M_3", M_3,
                "M_4", M_4,
                "M_5", M_5,
                "M_6", M_6,
                "M_7", M_7
                );

        sol::table it = lua.create_table();

        it["setStickyKeys"] = Input::setStickyKeys;
        it["getKey"] = Input::getKey;
        it["getKeyUp"] = Input::getKeyUp;
        it["getKeyDown"] = Input::getKeyDown;

        it["getAxis"] = Input::getAxis;
        it["getHorizontal"] = Input::getHorizontal;
        it["getVertical"] = Input::getVertical;

        it["getMouseButtonDown"] = Input::mouseButtonDown;
        it["getMouseButtonUp"] = Input::mouseButtonUp;

        it["getMousePosition"] = Input::mousePosition;
        it["getMousePositionX"] = Input::mousePositionX;
        it["getMousePositionY"] = Input::mousePositionY;

        it["getMouseScroll"] = Input::mouseScroll;
        it["getMouseScrollX"] = Input::mouseScrollX;
        it["getMouseScrollY"] = Input::mouseScrollY;

        lua["Input"] = it;
    }

    void reg_time(luastate_t& lua){
        sol::table tt = lua.create_table();

        tt["getDeltatime"] = Time::getDeltatime;
        tt["getUnscaledDeltatime"] = Time::getUnscaledDeltatime;
        tt["getTime"] = Time::getTime;
        tt["getUnscaledTime"] = Time::getUnscaledTime;
        tt["getScale"] = Time::getScale;
        tt["setScale"] = Time::setScale;
        tt["getFrameRate"] = Time::getFrameRate;
        tt["getFrameCount"] = Time::getFrameCount;

        lua["Time"]  = tt;
    }

    void reg_scene(luastate_t& lua){
        sol::table st = lua.create_table();

        st["createEntity"] = [](const char* name)->Entity{
            return SceneManager::getActiveScene()->createEntity(name);
        };
        st["destroyEntity"] =  [](Entity entity){
            entity.destroy();
        };
        st["getMainCameraEntity"] = []()->Entity{
            auto& sc = SceneManager::getActiveScene();
            if(sc == nullptr) return Entity(entt::null, sc.get());
            return sc->getMainCameraEntity();
        };
        st["getMainCamera"] = []()->camera_c*{
            auto& sc = SceneManager::getActiveScene();
            if(sc == nullptr) return nullptr;
            return sc->getMainCamera();
        };
        st["setMainCamera"] = [](Entity entity)->camera_c*{
            return SceneManager::getActiveScene()->setMainCamera(entity);
        };
        st["worldToScreen"] = [](vec3 worldPos)->vec2{
            return SceneManager::getActiveScene()->worldToScreen(worldPos);
        };
        st["screenToWorld"] = [](vec2 screenPos)->vec3{
            return SceneManager::getActiveScene()->screenToWorld(screenPos);
        };
        st["screenToWorldDir"] = [](vec2 screenPos)->vec3{
            return SceneManager::getActiveScene()->screenToWorldDir(screenPos);
        };

        lua["Scene"]  = st;
    }

    template<typename T>
    T* _getc(Entity& ent){
        return ent.isValid() && ent.has<T>()?&ent.get<T>():nullptr;
    }

    template<typename T>
    bool _hasc(Entity& ent){
        return ent.isValid() && ent.has<T>();
    }

    template<typename T>
    T* _addc(Entity& ent){
        return ent.isValid() ? &ent.tryAdd<T>() : nullptr;
    }

    template<typename T>
    void _removec(Entity& ent){
        if(ent.isValid()) ent.tryRemove<T>();
    }

    void reg_entity(luastate_t& lua){
        lua.new_usertype<Entity>("Entity",
                "destroy", &Entity::destroy,

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


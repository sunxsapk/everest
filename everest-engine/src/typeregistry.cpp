#include "scripting/typeregistry.h"
#include "core/input.h"
#include "core/time.h"
#include "scene/scenemanager.h"
#include "scene/entity.h"
#include "scene/camera.h"

#include "scripting/extended_types.h"
#include "scripting/components_lua.h"
#include "scripting/input_lua.h"


namespace Everest {
    namespace Scripting {

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
        st["load"] = [](int ind)->bool {
            return SceneManager::loadScene(ind);
        };
        st["loadNext"] = SceneManager::loadNext;
        st["getCurrentSceneIndex"] = []()->int{
            return SceneManager::getCurrentSceneIndex();
        };

        lua["Scene"]  = st;
    }

   
    void registerTypes(luastate_t& lua) {
        // types
        reg_types(lua);
        reg_matx(lua);
        reg_vecx(lua);
        reg_collision(lua);

        // components
        reg_transform(lua);
        reg_tag(lua);
        reg_script(lua);
        reg_rigidbody2d(lua);
        reg_spriteRenderer(lua);
        reg_circleRenderer(lua);
        reg_springJoint2d(lua);
        reg_circleCollider2d(lua);
        reg_boxCollider2d(lua);
        reg_camera(lua);

        // entity
        reg_entity(lua);

        // engine extensions
        reg_input(lua);
        reg_time(lua);
        reg_scene(lua);
    }

    }
}


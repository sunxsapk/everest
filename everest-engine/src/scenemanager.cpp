#include "scene/scenemanager.h"
#include "scene/serializer.h"


namespace Everest {

    SceneManager* SceneManager::_instance = nullptr;

    void SceneManager::init(){
        if(_instance) return;
        _instance = new SceneManager();
    }

    void SceneManager::quit(){
        delete _instance;
    }

    bool SceneManager::loadScene(const char* filepath){
        ref<Scene> scene = createScene();
        SceneSerializer::setSerializationContext(scene.get());
        bool _success = SceneSerializer::deserialize(filepath);
        if(_success){
            activateScene(scene);
            setSceneTarget(filepath);
        }
        return _success;
    }

    void SceneManager::saveScene(const char* filepath){
        SceneSerializer::setSerializationContext(_instance->activeScene.get());
        SceneSerializer::serialize(filepath);
        setSceneTarget(filepath);
    }
}

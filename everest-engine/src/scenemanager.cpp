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

    bool SceneManager::loadScene(size_t sceneIndex){
        if(sceneIndex < 0 || sceneIndex >= _instance->sceneSequence.size()) return false;
        if(loadScene(_instance->sceneSequence[sceneIndex].c_str())){
            _instance->currentSceneIndex = sceneIndex;
            return true;
        }
        return false;
    }

    void SceneManager::saveScene(const char* filepath){
        SceneSerializer::setSerializationContext(_instance->activeScene.get());
        SceneSerializer::serialize(filepath);
        setSceneTarget(filepath);
    }

    ref<Scene> SceneManager::onScenePlay(){
        if(!_instance->activeScene) return nullptr;
        _instance->savedActiveScene = Scene::copy(_instance->activeScene);
        _instance->activeScene->onScenePlay();
        return _instance->activeScene;
    }

    void SceneManager::onSceneStop(){
        if(_instance->activeScene){
            _instance->activeScene->onSceneStop(); // stop
            _instance->activeScene.reset(); // destroy
        }
        _instance->activeScene = _instance->savedActiveScene; // restore
    }
}

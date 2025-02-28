/*
 * ============ Scene Manager ==================
 * Author: Sunil Sapkota
 * Description: Scene Manager is responsible for creating/loading, saving, running and managing the
 * scenes by respecting their order of execution. It provides functionalities to manager scene assets.
 */


#pragma once

#include "core/utils.h"
#include "core/dbghelper.h"
#include "scene.h"

namespace Everest {

    class SceneManager {
        using sceneChangeCallback_t = std::function<void(ref<Scene>&)>;
        public:
            static void init();
            static void quit();

            static inline void onSceneChanged(sceneChangeCallback_t callback){
                _instance->sceneChangecb = callback;}

            static inline ref<Scene> createScene(const char* name = "Untitled Scene"){
                return createRef<Scene>(name); }

            static inline void activateScene(ref<Scene>& scene){
                _instance->activeScene = scene;
                if(_instance->sceneChangecb) _instance->sceneChangecb(_instance->activeScene);}

            static inline ref<Scene> createAndActivateScene(const char* name = "Untitled Scene"){
                _instance->activeScene = createScene(name);
                setSceneTarget("");
                if(_instance->sceneChangecb) _instance->sceneChangecb(_instance->activeScene);
                return _instance->activeScene;}

            static inline ref<Scene>& getActiveScene(){return _instance->activeScene;}
            static inline ref<Scene>& getRuntimeScene(){return _instance->runtimeScene;}

            static ref<Scene> onScenePlay();
            static void onSceneStop();

            static bool loadScene(const char* filepath);
            static void saveScene(const char* filepath);
            static void saveScene(){
                ASSERT(hasSaveTarget(), "Scene doesnot have a save Target");
                saveScene(_instance->scenePath.c_str());}

            static bool hasSaveTarget(){ return _instance->scenePath != ""; }
            static const char* getSceneTargetPath(){return _instance->scenePath.c_str();}

        private:
            ref<Scene> activeScene, runtimeScene;
            std::string scenePath = "";
            sceneChangeCallback_t sceneChangecb = nullptr;

            static inline void setSceneTarget(std::string path){
                _instance->scenePath = path;}
        private:
            static SceneManager* _instance;
            friend class AssetsManager;
    };
}

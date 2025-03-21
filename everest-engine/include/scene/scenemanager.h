/*
 * ============ Scene Manager ==================
 * Author: Sunil Sapkota
 * Description: Scene Manager is responsible for creating/loading, saving, running and managing the
 * scenes by respecting their order of execution. It provides functionalities to manager scene assets.
 */


#pragma once

#include "core/utils.h"
#include "core/dbghelper.h"
#include "core/application.h"
#include "scene.h"
#include <filesystem>

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
                onSceneChanged();}

            static inline ref<Scene> createAndActivateScene(const char* name = "Untitled Scene"){
                _instance->activeScene = createScene(name);
                setSceneTarget("");
                onSceneChanged();
                return _instance->activeScene;}

            static inline void onSceneChanged(){
#ifndef EDITOR_BUILD
                _instance->activeScene->onViewportResize(vec2(0), Application::getAppWindow().getSize());
#endif
                if(_instance->sceneChangecb) _instance->sceneChangecb(_instance->activeScene);
            }

            static inline ref<Scene>& getActiveScene(){return _instance->activeScene;}

            static ref<Scene> onScenePlay();
            static void onSceneStop();

            static bool loadScene(const char* filepath);
            static void saveScene(const char* filepath);

            static void saveScene(){
                ASSERT(hasSaveTarget(), "Scene doesnot have a save Target");
                saveScene(_instance->scenePath.c_str());}

            static bool hasSaveTarget(){ return _instance->scenePath != ""; }
            static std::string getSceneTargetPath(){return _instance->scenePath;}


            static bool loadScene(size_t sceneIndex = 0);
            static inline bool loadNext(){return loadScene(_instance->currentSceneIndex+1);}
            static inline size_t getCurrentSceneIndex(){return _instance->currentSceneIndex;}

            static inline std::vector<std::filesystem::path>& getSceneSeq(){
                return _instance->sceneSequence;}

        private:
            sceneChangeCallback_t sceneChangecb = nullptr;
            std::vector<std::filesystem::path> sceneSequence;
            ref<Scene> activeScene, savedActiveScene;
            std::string scenePath = "";
            size_t currentSceneIndex = 0;

            static inline void setSceneTarget(std::string path){
                _instance->scenePath = path;}

        private:
            static SceneManager* _instance;
            friend class AssetsManager;
            friend class EditorLayer;
    };
}

/*
 * ============ Scene Manager ==================
 * Author: Sunil Sapkota
 * Description: Scene Manager is responsible for creating/loading, saving, running and managing the
 * scenes by respecting their order of execution. It provides functionalities to manager scene assets.
 */


#pragma once

#include "scene.h"

namespace Everest {

    class SceneManager {
        public:
            static void init();
            static void quit();

            static inline ref<Scene> createScene(const char* name = "Untitled Scene"){
                return createRef<Scene>(name); }
            static inline void activateScene(ref<Scene>& scene){
                _instance->activeScene = scene;}
            static inline ref<Scene> createAndActivateScene(const char* name = "Untitled Scene"){
                return (_instance->activeScene = createScene(name));}

            static inline ref<Scene>& getActiveScene(){return _instance->activeScene;}

            static bool loadScene(const char* filepath);
            static void saveScene(const char* filepath);


        private:
            ref<Scene> activeScene;
        private:
            static SceneManager* _instance;
    };
}

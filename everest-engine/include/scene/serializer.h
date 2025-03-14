/*
 * ========== Scene SceneSerializer ===========
 * Author: Sunil Sapkota
 * Description: SceneSerializer class handles everything regarding serialization and deserialization of the
 * scene. It provides abstraction for saving and loading scenes into some readable file format (most
 * likely 'yaml' or some custom made format.
 */

#pragma once

#include "scene.h"

namespace Everest {

    class SceneSerializer {
        public:
            static void serialize(const char* filepath);
            static bool deserialize(const char* filepath);

            static inline void setSerializationContext(Scene* scene){
                _scene = scene;}
            static inline void clearSerializationContext(){_scene = nullptr;}

        private:
            static Scene* _scene;
    };
}

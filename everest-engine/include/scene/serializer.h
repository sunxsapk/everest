/*
 * ========== Scene SceneSerializer ===========
 * Author: Sunil Sapkota
 * Description: SceneSerializer class handles everything regarding serialization and deserialization of the
 * scene. It provides abstraction for saving and loading scenes into some readable file format (most
 * likely 'yaml' or some custom made format.
 */

#pragma once

#include <yaml-cpp/yaml.h>

#include "scene.h"
#include "scene/components.h"


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


    YAML::Emitter& operator<<(YAML::Emitter& out, const vec3& v);
    YAML::Emitter& operator<<(YAML::Emitter& out, const vec4& v);

    YAML::Emitter& operator<<(YAML::Emitter& out, const transform_c& tfr);
    YAML::Emitter& operator<<(YAML::Emitter& out, const tag_c& tag);
    YAML::Emitter& operator<<(YAML::Emitter& out, const camera_c& camera);
    YAML::Emitter& operator<<(YAML::Emitter& out, const spriteRenderer_c& spriteRenderer);
}

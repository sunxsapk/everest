/*
 * ========== Scene Serializer ===========
 * Author: Sunil Sapkota
 * Description: Serializer class handles everything regarding serialization and deserialization of the
 * scene. It provides abstraction for saving and loading scenes into some readable file format (most
 * likely 'yaml' or some custom made format.
 */

#pragma once

#include <yaml-cpp/yaml.h>

#include "scene.h"
#include "scene/components.h"


namespace Everest {

    class Serializer {
        public:
            Serializer() = default;
            Serializer(const ref<Scene>& scene);

            void serialize(const char* filepath);
            bool deserialize(const char* filepath);

            inline void setSerializationContext(const ref<Scene>& scene){ _scene = scene;}

        private:
            ref<Scene> _scene;
    };


    YAML::Emitter& operator<<(YAML::Emitter& out, const vec3& v);
    YAML::Emitter& operator<<(YAML::Emitter& out, const vec4& v);

    YAML::Emitter& operator<<(YAML::Emitter& out, const transform_c& tfr);
    YAML::Emitter& operator<<(YAML::Emitter& out, const tag_c& tag);

}

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
#include "components.h"


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


    YAML::Emitter& operator<<(YAML::Emitter& out, const vec2& v);
    YAML::Emitter& operator<<(YAML::Emitter& out, const vec3& v);
    YAML::Emitter& operator<<(YAML::Emitter& out, const vec4& v);

    YAML::Emitter& operator<<(YAML::Emitter& out, const transform_c& tfr);
    YAML::Emitter& operator<<(YAML::Emitter& out, const tag_c& tag);
    YAML::Emitter& operator<<(YAML::Emitter& out, const camera_c& camera);
    YAML::Emitter& operator<<(YAML::Emitter& out, const spriteRenderer_c& spriteRenderer);
    YAML::Emitter& operator<<(YAML::Emitter& out, const circleRenderer_c& circleRenderer);
    YAML::Emitter& operator<<(YAML::Emitter& out, const rigidbody2d_c& rigidbody2d);
    YAML::Emitter& operator<<(YAML::Emitter& out, const springJoint2d_c& springJoint2d);
    YAML::Emitter& operator<<(YAML::Emitter& out, const boxCollider2d_c& boxCollider2d);
    YAML::Emitter& operator<<(YAML::Emitter& out, const circleCollider2d_c& circleCollider2d);
    YAML::Emitter& operator<<(YAML::Emitter& out, const EvScript& script);

#ifndef __NO_3D__
    YAML::Emitter& operator<<(YAML::Emitter& out, const rigidbody_c& rigidbody);
    YAML::Emitter& operator<<(YAML::Emitter& out, const springJoint_c& springJoint);
#endif
}

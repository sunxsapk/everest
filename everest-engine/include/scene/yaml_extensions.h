/*
 * ============== YAML Extensions ===============
 * Author: Sunil Sapkota
 * Description: Defines extended types for serialization and deserialization
 */


#pragma once

#include <yaml-cpp/yaml.h>
#include "components.h"


namespace YAML {
    template<>
    struct convert<Everest::vec2> {
        static Node encode(const Everest::vec2& rhs){
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            return node;
        }

        static bool decode(const Node& node, Everest::vec2& rhs){
            if(!node.IsSequence() || node.size() != 2) return false;
            rhs.x = node[0].as<f32>();
            rhs.y = node[1].as<f32>();
            return true;
        }
    };

    template<>
    struct convert<Everest::vec3> {
        static Node encode(const Everest::vec3& rhs){
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node& node, Everest::vec3& rhs){
            if(!node.IsSequence() || node.size() != 3) return false;
            rhs.x = node[0].as<f32>();
            rhs.y = node[1].as<f32>();
            rhs.z = node[2].as<f32>();
            return true;
        }
    };

    template<>
    struct convert<Everest::vec4> {
        static Node encode(const Everest::vec4& rhs){
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            return node;
        }

        static bool decode(const Node& node, Everest::vec4& rhs){
            if(!node.IsSequence() || node.size() != 4) return false;
            rhs.x = node[0].as<f32>();
            rhs.y = node[1].as<f32>();
            rhs.z = node[2].as<f32>();
            rhs.w = node[3].as<f32>();
             return true;
        }
    };
}


namespace Everest {

    YAML::Emitter& operator<<(YAML::Emitter& out, const vec2& v);
    YAML::Emitter& operator<<(YAML::Emitter& out, const vec3& v);
    YAML::Emitter& operator<<(YAML::Emitter& out, const vec4& v);

    YAML::Emitter& operator<<(YAML::Emitter& out, const transform_c& tfr);
    YAML::Emitter& operator<<(YAML::Emitter& out, const tag_c& tag);
    YAML::Emitter& operator<<(YAML::Emitter& out, const camera_c& camera);
    YAML::Emitter& operator<<(YAML::Emitter& out, const spriteRenderer_c& spriteRenderer);
    YAML::Emitter& operator<<(YAML::Emitter& out, const circleRenderer_c& circleRenderer);
    YAML::Emitter& operator<<(YAML::Emitter& out, const rigidbody2d_c& rigidbody2d);
    YAML::Emitter& operator<<(YAML::Emitter& out, springJoint2d_c& springJoint2d);
    YAML::Emitter& operator<<(YAML::Emitter& out, const boxCollider2d_c& boxCollider2d);
    YAML::Emitter& operator<<(YAML::Emitter& out, const circleCollider2d_c& circleCollider2d);
    YAML::Emitter& operator<<(YAML::Emitter& out, const EvScript& script);

/*#ifndef __NO_3D__
    YAML::Emitter& operator<<(YAML::Emitter& out, const rigidbody_c& rigidbody);
    YAML::Emitter& operator<<(YAML::Emitter& out, const springJoint_c& springJoint);
#endif*/
}


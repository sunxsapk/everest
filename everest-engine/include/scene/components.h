/*
 * ============= Components ===============
 * Author: Sunil Sapkota
 * Description : Contains all of the components made available by the engine.
 */
#pragma once

#include "camera.h"
#include "renderer/sprite.h"
#include "scriptable.h"
#include "utils/uuid.h"
#include "yaml-cpp/emitter.h"

namespace Everest {

    struct id_c {
        UUID id;

        operator u64() const {return id;}
    };

    struct tag_c {
        std::string tag;
    };
    
    struct transform_c {
        vec3 position{0.f}, rotation{0.f}, scale{1.f};

        operator mat4(){
            mat4 trl = glm::translate(mat4(1.f), position);
            mat4 rot = glm::toMat4(glm::quat(glm::radians(rotation)));
            mat4 scl = glm::scale(mat4( 1.f), scale);
            return trl * rot * scl;
        }

    };

    struct spriteRenderer_c{
        vec4 color{1.f}; 
        Sprite sprite; // TODO: single and multiple
    };

    struct circleRenderer_c {
        vec4 color{1.f};
        f32 thickness = 1.f;
        f32 fade = 0.f;
    };

    struct camera_c {
        Camera camera;
        bool isPrimary = false;
        bool fixedAspect = false;

        operator Camera&(){return camera;}
    };

    enum class ForceMode {
        Force, Acceleration, Impulse, VelocityChange
    };

    struct rigidbody_c {
        vec3 velocity = vec3(0.f);
        f32 drag = 1.f;

        void addForce(const vec3 force, const ForceMode mode = ForceMode::Force);
        void setMass(f32 value);
        inline void setInverseMass(f32 invm){_inverseMass = invm;}
        inline f32 getMass(){ return 1.f / _inverseMass; }

        private:
        f32 _inverseMass = 1.f;
        vec3 _forceAccumulator = vec3(0.f);
        vec3 _impulse = vec3(0.f);

        // performs integration and returns the change in distance
        vec3 integrate(const f32 timeStep);
        friend class PhysicsHandler;
        friend YAML::Emitter& operator<<(YAML::Emitter&, const rigidbody_c&);
    };

    struct rigidbody2d_c {
        vec2 velocity = vec2(0.f);
        f32 drag = 1.f;

        void addForce(const vec2 force, const ForceMode mode = ForceMode::Force);
        void setMass(f32 value);
        inline void setInverseMass(f32 invm){_inverseMass = invm;}
        inline f32 getMass(){ return 1.f / _inverseMass; }

        private:
        f32 _inverseMass = 1.f;
        vec2 _forceAccumulator = vec2(0.f);
        vec2 _impulse = vec2(0.f);

        // performs integration and returns the change in distance
        vec2 integrate(const f32 timeStep);
        friend class PhysicsHandler;
        friend YAML::Emitter& operator<<(YAML::Emitter&, const rigidbody2d_c&);
    };

    struct nativeScript_c {
        Scriptable* _instance;

        Scriptable* (*create)();
        void (*destroy)(nativeScript_c*);

        template<typename script_t>
        void bind(){
            create = [](){return static_cast<Scriptable*>(new script_t());};
            destroy = [](nativeScript_c* nsc){
                delete nsc->_instance;
                nsc->_instance = nullptr;
            };
        }
    };

}

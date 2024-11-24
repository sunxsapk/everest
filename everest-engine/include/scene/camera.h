/*
 * ============= camera ==============
 * Author: Sunil Sapkota
 * Description: Camera system provides all the required functionalities required
 * for calculating view-frustum, projection, coordinates mapping, etc.
 */

#pragma once
#include "math/types.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "core/log.h"

#define __str__(x) #x

namespace Everest {
    enum CameraType {
        Orthographic = 1,
        Perspective
    };

    class Camera {
        public:
            Camera(mat4 projection):_projection(projection){}

            inline mat4 getProjection(){ return _projection;}
            inline void setProjection(mat4 projection){
                _projection = projection;
            }
            inline virtual void setAspect(f32 aspect){}
            inline CameraType getType(){return _type;}
            inline void setType(CameraType type){_type = type;}
        protected:
            mat4 _projection;
            CameraType _type = CameraType::Orthographic;
    };

    class OrthographicCamera : public Camera {
        public:
            OrthographicCamera(f32 orthosize, f32 aspect, f32 near = 0.f,
                    f32 far = 1000.f);

            static inline mat4 orthoFromLens(f32 orthosize, f32 aspect, f32 near, f32 far){
                f32 _lw = orthosize * aspect;
                return glm::ortho(-_lw, _lw, -orthosize, orthosize, near, far);
            }
            inline void setAspect(f32 aspect) override {
                _aspect = aspect;
                recalc();
            }
            inline void setOrthoSize(f32 size){ _orthosize = size; recalc(); }
            inline f32 getOrthoSize(){ return _orthosize;}

        private:
            inline void recalc(){
                _projection = orthoFromLens(_orthosize, _aspect, _near, _far);
            }
        private:
            f32 _orthosize, _aspect, _near, _far;
    };

    class PerspectiveCamera : public Camera {
        public:
            PerspectiveCamera(f32 fov = 60.f, f32 aspect = 16.f/9.f,
                    f32 near=0.f, f32 far=1000.f);

            static inline mat4 persFromLens(f32 fov, f32 aspect, f32 near, f32 far){
                return glm::perspective(fov, aspect, near, far);
            }
            inline void setAspect(f32 aspect) override {
                _aspect = aspect;
                recalc();
            }

        private:
            inline void recalc(){
                _projection = persFromLens(_fov, _aspect, _near, _far);
            }
        private:
            f32 _fov, _aspect, _near, _far;
    };
}

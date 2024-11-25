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

#define __str__(x) #x

namespace Everest {
    enum CameraType {
        Orthographic = 0,
        Perspective
    };

    struct OrthographicData {
        f32 orthoSize = 1.f, aspect = 1.f, near = -10.f, far=10.f;
    };

    struct PerspectiveData {
        f32 fov = 60.f, aspect = 1.f, near = 0.01f, far = 1000.f;
    };

    class Camera {
        public:
            Camera();
            Camera(CameraType type, mat4 projection):_projection(projection), _type(type){}
            Camera(OrthographicData data);
            Camera(PerspectiveData data);

            inline mat4 getProjection(){ return _projection;}
            inline void setProjection(mat4 projection){
                _projection = projection;
            }

            inline f32 getOrtho_size(){return _orthoData.orthoSize;}
            inline f32 getOrtho_aspect(){return _orthoData.aspect;}
            inline f32 getOrtho_near(){return _orthoData.near;}
            inline f32 getOrtho_far(){return _orthoData.far;}

            inline f32 getPersp_fov(){return _perspData.fov;}
            inline f32 getPersp_aspect(){return _perspData.aspect;}
            inline f32 getPersp_near(){return _perspData.near;}
            inline f32 getPersp_far(){return _perspData.far;}

            inline void setOrtho_aspect(f32 aspect){
                _orthoData.aspect = aspect; if(_type == Orthographic)recalc();}
            inline void setOrtho_size(f32 size){
                _orthoData.orthoSize = size; if(_type == Orthographic)recalc();}
            inline void setOrtho_near(f32 near){
                _orthoData.near = near; if(_type == Orthographic)recalc();}
            inline void setOrtho_far(f32 far){
                _orthoData.far = far; if(_type == Orthographic)recalc();}

            inline void setPersp_near(f32 near){
                _perspData.near = near; if(_type == Perspective)recalc();}
            inline void setPersp_aspect(f32 aspect){
                _perspData.aspect = aspect; if(_type == Perspective)recalc();}
            inline void setPersp_far(f32 far){
                _perspData.far = far; if(_type == Perspective)recalc();}
            inline void setPersp_fov(f32 fov){
                _perspData.fov = fov; if(_type == Perspective)recalc();}

            inline CameraType getType(){return _type;}
            inline void setType(CameraType type){_type = type; recalc();}

        protected:
            OrthographicData _orthoData;
            PerspectiveData _perspData;
        protected:
            mat4 _projection;
            CameraType _type = CameraType::Orthographic;

        private:
            inline void recalc(){
                if(_type == CameraType::Orthographic){
                    f32 lw = _orthoData.orthoSize * _orthoData.aspect;
                    _projection = glm::ortho(-lw, lw, -_orthoData.orthoSize, _orthoData.orthoSize,
                            _orthoData.near, _orthoData.far);
                } else {
                    _projection = glm::perspective(_perspData.fov, _perspData.aspect,
                            _perspData.near, _perspData.far);
                }
            }
    };

    /*
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
    */
}

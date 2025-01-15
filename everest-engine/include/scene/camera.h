/*
 * ============= camera ==============
 * Author: Sunil Sapkota
 * Description: camera_c system provides all the required functionalities required
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
        f32 orthoSize = 10.f, aspect = 1.f, near = -100.f, far=100.f;
    };

    struct PerspectiveData {
        f32 fov = 60.f, aspect = 16.f/9.f, near = 0.01f, far = 1000.f;
    };

    class camera_c {
        public:
            bool isPrimary = false;
            bool fixedAspect = false;


            camera_c();
            camera_c(CameraType type, mat4 projection):_projection(projection), _type(type){}
            camera_c(OrthographicData data);
            camera_c(PerspectiveData data);

            inline mat4 getProjection(){ return _projection;}
            inline void setProjection(mat4 projection){
                _projection = projection;
            }

            inline f32 getOrtho_size() const {return _orthoData.orthoSize;}
            inline f32 getOrtho_aspect() const {return _orthoData.aspect;}
            inline f32 getOrtho_near() const {return _orthoData.near;}
            inline f32 getOrtho_far() const {return _orthoData.far;}

            inline f32 getPersp_fov() const {return _perspData.fov;}
            inline f32 getPersp_aspect() const {return _perspData.aspect;}
            inline f32 getPersp_near() const {return _perspData.near;}
            inline f32 getPersp_far() const {return _perspData.far;}

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

            inline CameraType getType() const {return _type;}
            inline void setType(CameraType type){_type = type; recalc();}

            inline void setOrthographicData(OrthographicData data){_orthoData = data;}
            inline void setPerspectiveData(PerspectiveData data){_perspData = data;}

        protected:
            OrthographicData _orthoData;
            PerspectiveData _perspData;
            CameraType _type = CameraType::Orthographic;
        protected:
            mat4 _projection;

        private:
            inline void recalc(){
                if(_type == CameraType::Orthographic){
                    f32 lw = _orthoData.orthoSize * _orthoData.aspect;
                    _projection = glm::ortho(-lw, lw, -_orthoData.orthoSize, _orthoData.orthoSize,
                            _orthoData.near, _orthoData.far);
                } else {
                    _projection = glm::perspective(glm::radians(_perspData.fov), _perspData.aspect,
                            _perspData.near, _perspData.far);
                }
            }
    };
}

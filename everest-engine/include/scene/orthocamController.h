/*
 * =========== Orthographic camera controller ========
 * Author: Sunil Sapkota
 * Desrcription: Orthographic camera controller is a camera controller, that
 * wraps the orthographic camera and provides functionalities to control it and
 * provides abstracted functionalities to user.
 */

#pragma once

#include "math/types.h"
#include "camera.h"

namespace Everest {
    class OrthographicCameraController {
        public:
            OrthographicCameraController(f32 speed = 1.f, f32 rot_speed = 1.f,
                    f32 orthoSize = 10.f, f32 aspect = 16.f/9.f);

            void onUpdate();

            inline OrthographicCamera& getCamera(){ return _camera;}

        private:
            inline f32 getSpeed(){
                return _speed * _camera.getOrthoSize() * _speedZoomRatio;
            }
            inline f32 getAngularSpeed(){
                return _angularSpeed * _camera.getOrthoSize() * _speedZoomRatio;
            }
        private:
            OrthographicCamera _camera;
            f32 _speed, _angularSpeed;
            f32 _zoomSensitivity, _speedZoomRatio;
    };
}

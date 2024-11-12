/*
 * ======== perspective camera controller =======
 * Author: Sunil Sapkota
 * Description: Perspective camera controller wraps the perspective camera
 * class and provides the functionalities for controlling the camera. It is
 * generally concerned with providing first person controlling options along
 * with some added modes, if required.
 */

#pragma once
#include "pch.h"
#include "camera.h"

namespace Everest {
    class PerspectiveCameraController {
        public:
           PerspectiveCameraController(f32 speed = 1.f, f32 fov = 60.f,
                   f32 aspect = 16.f/9.f); 
           
           void onUpdate();

           inline PerspectiveCamera& getCamera(){return _camera;}

        private:
           vec3 dirFromDel(vec2 del);
           void panControl();
        private:
           PerspectiveCamera _camera;
           ivec2 _panStart;
           vec2 _panSpos;
           f32 _speed, _mouseSensitivity;
           bool _isPanning;
    };
}

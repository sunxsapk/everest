#pragma once

#include <everest.h>


namespace Everest {

    class CameraController : public Scriptable {
        f32 _speed = 10.f;
        f32 _zoomSensitivity = 0.5f, _speedZoomRatio = 1.f;

        void onCreate() override {
        }

        void onUpdate() override {
            transform_c&  transform = get<transform_c>();
            vec3& pos = transform.position;

            f32 disp = _speed * Time::getDeltatime();

            camera_c& cam = get<camera_c>();
            if(cam.camera.getType() == Orthographic){
                if(Input::getKeyDown(K_w)) pos.y += disp;
                if(Input::getKeyDown(K_s)) pos.y -= disp;
            }else{
                if(Input::getKeyDown(K_w)) pos.z += disp;
                if(Input::getKeyDown(K_s)) pos.z -= disp;
            }
            if(Input::getKeyDown(K_a)) pos.x -= disp;
            if(Input::getKeyDown(K_d)) pos.x += disp;
        }

        void onDestroy() override {
        }
    };
}

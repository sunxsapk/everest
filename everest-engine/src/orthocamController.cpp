#include <glm/gtx/rotate_vector.hpp>
#include "scene/orthocamController.h"
#include "core/input.h"
#include "core/time.h"


namespace Everest {
    OrthographicCameraController::OrthographicCameraController(f32 speed,
            f32 rot_speed, vec2 lensSize, f32 near, f32 far)
    :_camera(lensSize, near, far), _speed(speed), _angularSpeed(rot_speed),
     _zoomSensitivity(4.f), _speedZoomRatio(0.1f){
        _camera._position = vec3(0.f, 0.f, 10.f);
        _camera._vectors = { vec3(0.f, 0.f, -1.f), vec3(0.f, 1.f, 0.f)};
    }

    void OrthographicCameraController::onUpdate(){
        vec3 pos = _camera.getPosition();
        f32 disp = getSpeed() * Time::getDeltaTime();
        f32 angdisp = getAngularSpeed() * Time::getDeltaTime();

        if(Input::getKeyDown(K_w)) pos.y += disp;
        if(Input::getKeyDown(K_s)) pos.y -= disp;
        if(Input::getKeyDown(K_a)) pos.x -= disp;
        if(Input::getKeyDown(K_d)) pos.x += disp;

        if(Input::getKeyDown(K_q)) {
            _camera.setUpVector(glm::rotate(_camera.getUp(),
                        angdisp, _camera.getForward()));
        }
        if(Input::getKeyDown(K_e)) {
            _camera.setUpVector(glm::rotate(_camera.getUp(),
                        -angdisp, _camera.getForward()));
        }

        f32 scroll = Input::mouseScrollY();
        if(scroll != 0.f){
            _camera.setOrthoSize(_camera.getOrthoSize() *
                    (1.f - Time::getDeltaTime() * scroll * _zoomSensitivity));
        }

        _camera.setPosition(pos);
    }
}

#include "scene/perscamController.h"
#include "core/input.h"
#include "core/time.h"


namespace Everest {
    PerspectiveCameraController::PerspectiveCameraController(
            f32 speed, f32 fov, f32 aspect)
        :_camera(fov, aspect), _speed(speed), _mouseSensitivity(4.f),
        _isPanning(false){ }

    vec3 PerspectiveCameraController::dirFromDel(vec2 del){
        del *= Time::getDeltaTime() * _mouseSensitivity;
        vec3 fwd = glm::rotate(_camera.getForward(), glm::radians(del.x), _camera.getUp());
        //fwd = glm::rotate(fwd, glm::radians(del.y), _camera.getRight());
        return glm::normalize(fwd);
    }

    void PerspectiveCameraController::onUpdate(){
        vec3 pos = _camera.getPosition();
        f32 disp = _speed * Time::getDeltaTime();

        if(Input::getKeyDown(K_w)) pos += _camera.getForward() * disp;
        if(Input::getKeyDown(K_s)) pos -= _camera.getForward() * disp;

        if(Input::getKeyDown(K_a)) pos -= _camera.getRight() * disp;
        if(Input::getKeyDown(K_d)) pos += _camera.getRight() * disp;

        if(Input::getKeyDown(K_q)) pos += _camera.getUp() * disp;
        if(Input::getKeyDown(K_e)) pos -= _camera.getUp() * disp;

        panControl();

        _camera.setPosition(pos);
    }

    void PerspectiveCameraController::panControl(){
        if(!_isPanning){
            if(Input::mouseButtonDown(MouseButton_1)){
                _panStart = Input::mousePosition();
                _isPanning = true;
            }
        } else {
            if(Input::mouseButtonUp(MouseButton_1)){
                _isPanning = false;
            } else {
                ivec2 mp = Input::mousePosition();
                vec2 del = mp - _panStart;
                _panStart = mp;
                _camera.faceTo(dirFromDel(del));
            }
        }
    }
}

#include "editorcamera.h"



namespace Everest {

    EditorCamera::EditorCamera()
    :camera(PerspectiveData()){
        transform.position = vec3(0.f, 1.f, 4.f);
    }

    void EditorCamera::onUpdate(bool focused) {
        if(!focused){
            _mouseLastPos = Input::mousePosition();
        }
        if(camera.getType() ==  Orthographic) cam2d_ctrls();
        else cam3d_ctrls();
    }

    vec3 EditorCamera::getRight(){
        f32 beta = glm::radians(transform.rotation.y);
        return vec3(glm::cos(beta), 0.f, -glm::sin(beta));
    }

    vec3 EditorCamera::getUp(){
        f32 alpha = glm::radians(transform.rotation.x);
        f32 beta   = glm::radians(transform.rotation.y);
        f32 sinb = glm::sin(beta), cosb = glm::cos(beta),
            sina = glm::sin(alpha), cosa = glm::cos(alpha);
        vec3 up = vec3(sina*sinb, cosa, sina*cosb);
        return glm::normalize(up);
    }

    vec3 EditorCamera::getForward(){
        f32 alpha = glm::radians(transform.rotation.x);
        f32 beta   = glm::radians(transform.rotation.y);
        f32 sinb = glm::sin(beta), cosb = glm::cos(beta),
            sina = glm::sin(alpha), cosa = glm::cos(alpha);
        vec3 forward = vec3(-cosa * sinb, sina, -cosa * cosb);
        return glm::normalize(forward);
    }

    void EditorCamera::lookAt(vec3 target){
        target = glm::normalize(target - transform.position);
        f32 beta = glm::atan(-target.x, -target.z);
        f32 alpha = glm::asin(target.y);
        transform.rotation.y = glm::degrees(beta);
        transform.rotation.x = glm::degrees(alpha);
    }

    void EditorCamera::setType(CameraType type){
        camera.setType(type);
        if(type == Orthographic){
            camera.setOrtho_aspect(camera.getPersp_aspect());
            transform.position = vec3(0, 0, 10);
            transform.rotation = vec3(0);
        }
    }

    void EditorCamera::keyControls_2d(){
        f32 disp = speed * Time::getDeltatime();
        transform.position.x += Input::getHorizontal() * disp;
        transform.position.y += Input::getVertical() * disp;
    }

    bool EditorCamera::mouseControls_2d(){
        vec3 mdel(0);
        if(_mouseHeld){
            mdel.x = _mouseLastPos.x - Input::mousePositionX();
            mdel.y = Input::mousePositionY() - _mouseLastPos.y;

            _mouseLastPos = Input::mousePosition();
            if(Input::mouseButtonUp(MouseButton_0)) _mouseHeld = false;
        } else if(Input::mouseButtonDown(MouseButton_0)) {
            _mouseHeld = true;
            _mouseLastPos = Input::mousePosition(); 
        } else return false;

        mdel *= camera.getOrtho_size() * mouseSensitivity * Time::getUnscaledDeltatime();
        transform.position += mdel;

        f32 mulfct = Input::mouseScrollY() * scrollSensitivity * Time::getUnscaledDeltatime();
        camera.setOrtho_size(camera.getOrtho_size() * (1.f - mulfct));
        return true;
    }

    void EditorCamera::cam2d_ctrls(){
        if(mouseControls_2d()) keyControls_2d();
    }


    void EditorCamera::keyControls_3d(){
        f32 disp = speed * Time::getDeltatime();
        if(Input::getHorizontal() != 0.f){
            vec3 right = getRight() * disp;
            transform.position += right * Input::getHorizontal();
        }

        if(Input::getVertical() != 0.f){
            vec3 fwd = getForward() * disp;
            transform.position += fwd * Input::getVertical();
        }
    }

    bool EditorCamera::mouseControls_3d(){
        vec3 mdel(0);
        if(_mouseHeld){
            mdel.x = _mouseLastPos.x - Input::mousePositionX();
            mdel.y = Input::mousePositionY() - _mouseLastPos.y;

            _mouseLastPos = Input::mousePosition();
            if(Input::mouseButtonUp(MouseButton_0)) _mouseHeld = false;
        } else if(Input::mouseButtonDown(MouseButton_0)) {
            _mouseHeld = true;
            _mouseLastPos = Input::mousePosition(); 
        } else return false;

        mdel *= mouseSensitivity;
        transform.rotation.y -= mdel.x;
        transform.rotation.x += mdel.y;
        return true;
    }

    void EditorCamera::cam3d_ctrls(){
        if(mouseControls_3d()) keyControls_3d();
    }

    void EditorCamera::onViewportResize(uvec2 viewportSize){
        f32 aspect = (float)viewportSize.x / viewportSize.y;

        if(camera.getType() == Orthographic) camera.setOrtho_aspect(aspect);
        else camera.setPersp_aspect(aspect);
    }

}

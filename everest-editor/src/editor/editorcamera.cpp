#include "editorcamera.h"



namespace Everest {

    EditorCamera::EditorCamera()
    :camera(PerspectiveData()){
        transform.position = vec3(0.f, 1.f, 4.f);
    }

    void EditorCamera::onUpdate() {
         if(camera.getType() ==  Orthographic) cam2d_ctrls();
         else cam3d_ctrls();
    }

    vec3 EditorCamera::getForward(){
        f32 pitch = glm::radians(transform.rotation.x);
        f32 yaw   = glm::radians(transform.rotation.y-90.f);
        f32 cp = glm::cos(pitch), cy = glm::cos(yaw);
        glm::vec3 forward( cp * cy, glm::sin(pitch), cp * glm::sin(yaw));
        return glm::normalize(forward);
    }

    void EditorCamera::lookAt(vec3 target){
        target = glm::normalize(target - transform.position);
        f32 yaw = glm::atan(target.z, target.x);
        f32 pitch = glm::asin(target.y);
        transform.rotation.y = glm::degrees(yaw)+90.f;
        transform.rotation.x = glm::degrees(pitch);
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
        transform.position.x += Input::getHorizontal() * disp;
        transform.position.z -= Input::getVertical() * disp;
        transform.position.y += (Input::getKeyDown(K_q) - Input::getKeyDown(K_e)) * disp;
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

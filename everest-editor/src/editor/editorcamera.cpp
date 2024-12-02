#include "editorcamera.h"



namespace Everest {

    EditorCamera::EditorCamera()
    :camera(PerspectiveData()){
        transform.position.z = 10.f;
    }

    void EditorCamera::onUpdate() {
         if(camera.getType() ==  Orthographic) cam2d_ctrls();
         else cam3d_ctrls();
    }


    void EditorCamera::keyControls_2d(){
        f32 disp = speed * Time::getDeltatime();
        if(Input::getKeyDown(K_w)) transform.position.y += disp;
        if(Input::getKeyDown(K_s)) transform.position.y -= disp;
        if(Input::getKeyDown(K_a)) transform.position.x -= disp;
        if(Input::getKeyDown(K_d)) transform.position.x += disp;
    }

    void EditorCamera::mouseControls_2d(){
        vec3 mdel(0);
        if(_mouseHeld){
            mdel.x = _mouseLastPos.x - Input::mousePositionX();
            mdel.y = Input::mousePositionY() - _mouseLastPos.y;

            _mouseLastPos = Input::mousePosition();
            if(Input::mouseButtonUp(MouseButton_0)) _mouseHeld = false;
        } else if(Input::mouseButtonDown(MouseButton_0)) {
            _mouseHeld = true;
            _mouseLastPos = Input::mousePosition(); 
        }

        mdel *= camera.getOrtho_size() * mouseSensitivity * Time::getUnscaledDeltatime();
        transform.position += mdel;

        f32 mulfct = Input::mouseScrollY() * scrollSensitivity * Time::getUnscaledDeltatime();
        camera.setOrtho_size(camera.getOrtho_size() * (1.f - mulfct));
    }

    void EditorCamera::cam2d_ctrls(){
        mouseControls_2d();
        if(Input::mouseButtonDown(MouseButton_1)) keyControls_2d();
    }


    void EditorCamera::keyControls_3d(){
        f32 disp = speed * Time::getDeltatime();
        vec3 del(0);
        if(Input::getKeyDown(K_w)) del.z -= disp;
        if(Input::getKeyDown(K_s)) del.z += disp;
        if(Input::getKeyDown(K_q)) del.y += disp;
        if(Input::getKeyDown(K_e)) del.y -= disp;
        if(Input::getKeyDown(K_a)) del.x -= disp;
        if(Input::getKeyDown(K_d)) del.x += disp;

        transform.position += del;
    }

    void EditorCamera::mouseControls_3d(){
    }

    void EditorCamera::cam3d_ctrls(){
        mouseControls_3d();
        if(!Input::mouseButtonDown(MouseButton_1)) keyControls_3d();
    }

    void EditorCamera::onViewportResize(uvec2 viewportSize){
        f32 aspect = (float)viewportSize.x / viewportSize.y;

        if(camera.getType() == Orthographic) camera.setOrtho_aspect(aspect);
        else camera.setPersp_aspect(aspect);
    }

}

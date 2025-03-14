#include "editorcamera.h"
#include "scenepanel.h"



namespace Everest {

    EditorCamera::EditorCamera()
    :camera(){
        transform.position = vec3(0.f, 1.f, 4.f);
    }

    void EditorCamera::onUpdate(bool focused) {
        if(ScenePanel::getSceneState() == SceneState::PLAY && ScenePanel::gameView) return;
        if(!ScenePanel::isFocused()) return;
        if(!focused){
            _mouseLastPos = Input::mousePosition();
        }
        if(camera.is2d()) cam2d_ctrls();
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
        return Math::getCameraForward(transform);
    }

    vec3 EditorCamera::screenPointToDir(vec2 position){
        vec2 sz = ScenePanel::getSceneViewportSize();
        vec4 ndc(position.x/sz.x*2-1.f, 1.f-position.y/sz.y*2, 1.f, 1.f);
        vec4 ax = glm::inverse(camera.getProjection()) * ndc;
        ax /= ax.w;
        ax.w = 0.f;
        ax = glm::normalize((mat4)transform * ax);
        return ax;
    }

    vec3 EditorCamera::screenToWorldPos(vec2 position){
        vec2 sz = ScenePanel::getSceneViewportSize();
        vec3 clipc(position.x/sz.x*2-1.f, 1.f-position.y/sz.y*2, 
                camera.is2d()? -1.f : camera.get_near());
        vec4 pp = glm::inverse(getVPmat()) * vec4(clipc, 1.f);
        pp /= pp.w;
        return pp;
    }

    void EditorCamera::lookAt(vec3 target){
        target = glm::normalize(target - transform.position);
        f32 beta = glm::atan(-target.x, -target.z);
        f32 alpha = glm::asin(target.y);
        transform.rotation.y = glm::degrees(beta);
        transform.rotation.x = glm::degrees(alpha);
    }

    void EditorCamera::keyControls_2d(){
        f32 disp = speed * Time::getDeltatime();
        transform.position.x += Input::getHorizontal() * disp;
        transform.position.y += Input::getVertical() * disp;
    }

    void EditorCamera::mouseControls_2d(){
        vec3 mdel(0);

        if(Input::mouseScrollY()){
            f32 mulfct = Input::mouseScrollY() * scrollSensitivity * Time::getUnscaledDeltatime();
            camera.set_lenssize(camera.get_lenssize() * (1.f - mulfct));
        }

        if(_mouseHeld){
            mdel.x = _mouseLastPos.x - Input::mousePositionX();
            mdel.y = Input::mousePositionY() - _mouseLastPos.y;

            _mouseLastPos = Input::mousePosition();
            if(Input::mouseButtonUp(M_1)) _mouseHeld = false;
        } else if(Input::mouseButtonDown(M_1)) {
            _mouseHeld = true;
            _mouseLastPos = Input::mousePosition(); 
        } else return;

        mdel *= camera.get_lenssize() * mouseSensitivity * Time::getUnscaledDeltatime();
        transform.position += mdel;
    }

    void EditorCamera::cam2d_ctrls(){
        mouseControls_2d();
        if(Input::mouseButtonDown(M_1)) keyControls_2d();
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

    void EditorCamera::mouseControls_3d(){
        vec3 mdel(0);
        if(_mouseHeld){
            mdel.x = _mouseLastPos.x - Input::mousePositionX();
            mdel.y = Input::mousePositionY() - _mouseLastPos.y;

            _mouseLastPos = Input::mousePosition();
            if(Input::mouseButtonUp(M_1)) _mouseHeld = false;
        } else if(Input::mouseButtonDown(M_1)) {
            _mouseHeld = true;
            _mouseLastPos = Input::mousePosition(); 
        } else return;

        mdel *= mouseSensitivity;
        transform.rotation.y -= mdel.x;
        transform.rotation.x += mdel.y;
        return;
    }

    void EditorCamera::cam3d_ctrls(){
        mouseControls_3d();
        if(Input::mouseButtonDown(M_1)) keyControls_3d();
    }

    void EditorCamera::onViewportResize(uvec2 viewportSize){
        f32 aspect = (float)viewportSize.x / viewportSize.y;
        camera.set_aspect(aspect);
    }

}

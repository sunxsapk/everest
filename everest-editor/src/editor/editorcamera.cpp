#include "editorcamera.h"



namespace Everest {

    SceneCamera::SceneCamera()
    :camera(PerspectiveData()){
        transform.position.z = 10.f;
    }

    void SceneCamera::onUpdate() {
        vec3& pos = transform.position;

        f32 disp = _speed * Time::getDeltatime();

        if(camera.getType() == Orthographic){
            if(Input::getKeyDown(K_w)) pos.y += disp;
            if(Input::getKeyDown(K_s)) pos.y -= disp;
        }else{
            if(Input::getKeyDown(K_w)) pos.z -= disp;
            if(Input::getKeyDown(K_s)) pos.z += disp;
        }
        if(Input::getKeyDown(K_a)) pos.x -= disp;
        if(Input::getKeyDown(K_d)) pos.x += disp;
    }

    void SceneCamera::onViewportResize(uvec2 viewportSize){
        f32 aspect = (float)viewportSize.x / viewportSize.y;

        if(camera.getType() == Orthographic) camera.setOrtho_aspect(aspect);
        else camera.setPersp_aspect(aspect);
    }

}

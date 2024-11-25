#include "scene/camera.h"

namespace Everest {

    Camera::Camera()
    :_type(Orthographic){
        recalc();
    }


    Camera::Camera(OrthographicData data)
    :_orthoData(data), _type(Orthographic){
        recalc();
    }

    Camera::Camera(PerspectiveData data)
    :_perspData(data), _type(Perspective){
        recalc();
    }

    /*
    OrthographicCamera::OrthographicCamera(f32 orthosize, f32 aspect, f32 near, f32 far)
    : Camera(orthoFromLens(orthosize, aspect, near, far)){
        _type = Orthographic;
        _orthosize = orthosize, _aspect = aspect, _near = near, _far = far;
    }

    PerspectiveCamera::PerspectiveCamera(f32 fov, f32 aspect, f32 near, f32 far)
    :Camera(persFromLens(fov, aspect, near, far)){ 
        _type = Perspective;
        _fov = fov, _aspect = aspect, _near = near, _far = far;
    }
    */

}

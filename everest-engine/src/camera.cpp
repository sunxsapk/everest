#include "scene/camera.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Everest {
    Camera::Camera()
        :_position(0),
        _vectors({vec3(0.f,0.f,1.f),vec3(0.f,1.f,0.f), vec3(-1.f, 0.f, 0.f)}){ }


    void Camera::recalcView(){
        EV_profile_function();

        _viewMat = glm::lookAt(_position, _position+_vectors.forward, _vectors.up);
        _vpMat = _projMat * _viewMat;
    }

    OrthographicCamera::OrthographicCamera(f32 orthosize, f32 aspect,
            f32 near, f32 far){
        EV_profile_function();

        ASSERT(orthosize > 0.f, "Invalid lens size");
        ASSERT(far >= 0.f, "Camera's 'far' property should be positive");

        _props = {orthosize * aspect, orthosize, near, far};

        recalcView();
        recalcProj();
    }

    void OrthographicCamera::recalcProj(){
        EV_profile_function();

        f32 hw = _props.width/2.f, hh = _props.height/2.f;
        _projMat = glm::ortho(-hw, hw, -hh, hh, _props.near, _props.far);
        _vpMat = _projMat * _viewMat;
    }

    void OrthographicCamera::setAspectRatio(f32 aspect){
        EV_profile_function();

        ASSERT(aspect > 0.f, "Invalid aspect ratio for camera");
        _props.width = _props.height * aspect;
        recalcProj();
    }

    void OrthographicCamera::setOrthoSize(f32 size){
        EV_profile_function();

        size = std::max(0.00001f, size);
        _props.width = size * _props.width / _props.height;
        _props.height = size;
        recalcProj();
    }

    PerspectiveCamera::PerspectiveCamera(f32 fov, f32 aspect, f32 near, f32 far){
        EV_profile_function();

        ASSERT(fov >= 0.f, "Invalid field of view");
        ASSERT(aspect >= 0.f, "Invalid aspect ratio");
        ASSERT(near >= 0.f, "Invalid 'near' property");
        ASSERT(far >= 0.f, "Invalid 'far' property");

        _props = {fov, aspect, near, far};
        recalcView();
        recalcProj();
    }

    void PerspectiveCamera::recalcProj(){
        EV_profile_function();

        _projMat = glm::perspective(glm::radians(_props.fov), _props.aspect,
                _props.near, _props.far);
        _vpMat = _projMat * _viewMat;
    }
}

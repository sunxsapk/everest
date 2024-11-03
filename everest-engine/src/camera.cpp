#include "scene/camera.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

namespace Everest {
    Camera::Camera()
        :_position(0), _rotation(0),
        _vectors({{0.f,1.f,0.f},{1.f,0.f,0.f},{0.f,0.f,1}}){ }


    void Camera::recalcView(){
        // TODO : properly calculate the rotation of camera
        mat4 rot = 
            eulerAngleXYZ(radians(_rotation.x), radians(_rotation.y), radians(_rotation.z));
            //yawPitchRoll(radians(_rotation.y), radians(_rotation.z), radians(_rotation.x));

        _vectors.forward = rot * vec4(1.f,0.f,0.f,0.f);
        _vectors.up  = rot * vec4(0.f,1.f,0.f,0.f);
        _vectors.right = normalize(cross(_vectors.forward, _vectors.up));

        _viewMat = lookAt(_position, _position+_vectors.forward, _vectors.up);
        _vpMat = _projMat * _viewMat;
    }

    OrthographicCamera::OrthographicCamera(vec2 lensSize, f32 near, f32 far){
        ASSERT(lensSize.x > 0.f);
        ASSERT(lensSize.y > 0.f);
        ASSERT(far > 0.f);

        lensSize.x /= 2;
        lensSize.y /= 2;

        _props = {
            -lensSize.x, lensSize.x,
            -lensSize.y, lensSize.y,
             near,       far
        };

        recalcView();
        recalcProj();
    }

    void OrthographicCamera::recalcProj(){
        _projMat = ortho(_props.left, _props.right, _props.bottom,
                _props.top, _props.near, _props.far);
        _vpMat = _projMat * _viewMat;
    }

    void OrthographicCamera::setLensSize(vec2 lensSize){
        ASSERT(lensSize.x > 0.f);
        ASSERT(lensSize.y > 0.f);
        lensSize.x /= 2; lensSize.y /= 2;
        _props.left   = -lensSize.x, _props.right = lensSize.x;
        _props.bottom = -lensSize.y, _props.top   = lensSize.y;
        recalcProj();
    }

    PerspectiveCamera::PerspectiveCamera(f32 fov, f32 aspect, f32 near, f32 far){
        ASSERT(fov >= 0.f);
        ASSERT(aspect >= 0.f);
        ASSERT(near >= 0.f);
        ASSERT(far >= 0.f);

        _props = {fov, aspect, near, far};
        recalcView();
        recalcProj();
    }

    void PerspectiveCamera::recalcProj(){
        _projMat = perspective(radians(_props.fov), _props.aspect,
                _props.near, _props.far);
        _vpMat = _projMat * _viewMat;
    }
}

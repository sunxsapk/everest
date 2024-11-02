#include "scene/camera.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

namespace Everest {
    Camera::Camera()
        :_position(0), _rotation(0), _vectors({{0,1,0},{1,0,0},{0,0,1}}){ }


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

    void OrthographicCamera::recalcView(){
        mat4 rot = yawPitchRoll(_rotation.y, _rotation.z, _rotation.x);

        _vectors.forward = rot * vec4(1,0,0,0);
        _vectors.up  = rot * vec4(0,1,0,0);
        _vectors.right = normalize(cross(_vectors.forward, _vectors.up));

        _viewMat = lookAt(_position, _position+_vectors.forward, _vectors.up);
        _vpMat = _projMat * _viewMat;
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

    void OrthographicCamera::setViewRange(f32 range){
        ASSERT(range > 0.f);
        _props.far = range;
        recalcProj();
    }

    void OrthographicCamera::setNearClip(f32 near){
        ASSERT(near >= 0);
        _props.near = near;
        recalcProj();
    }
}

/*
 * ============= camera ==============
 * Author: Sunil Sapkota
 * Description: Camera system provides all the required functionalities required
 * for calculating view-frustum, projection, coordinates mapping, etc.
 */

#pragma once
#include "pch.h"

namespace Everest {
    class Camera {
        public:

            /*returns the projection * view matrix of camera*/
            inline mat4 getVPmatrix(){return _vpMat;}
            /* returns camera position */
            inline vec3 getPosition(){return _position;}
            /* returns the direction in which camera is facing */
            inline vec3 getForward(){return _vectors.forward;}
            /* returns the up-vector of the camera */
            inline vec3 getUp(){return _vectors.up;}
            /* returns the right-vector of the camera */
            inline vec3 getRight(){return _vectors.right;}

            /* sets the camera position */
            inline void setPosition(vec3 position){
                _position = position;
                recalcView();
            }
            /* makes the camera look towards the given point */
            inline void lookAt(vec3 point){
                _vectors.forward = point - _position;
                _vectors.right = glm::normalize(glm::cross(_vectors.forward, _vectors.up));
                _vectors.up = glm::normalize(glm::cross(_vectors.right, _vectors.forward));
                recalcView();
            }
            inline void faceTo(vec3 dir){
                _vectors.forward = dir;
                _vectors.right = glm::normalize(glm::cross(_vectors.forward, _vectors.up));
                _vectors.up = glm::normalize(glm::cross(_vectors.right, _vectors.forward));
                recalcView();
            }
            inline void setUpVector(vec3 up){
                _vectors.up = up;
                _vectors.right = glm::normalize(glm::cross(_vectors.forward, _vectors.up));
                _vectors.forward = glm::normalize(glm::cross(_vectors.up, _vectors.right));
                recalcView();
            }

        protected:
            Camera();

            void recalcView();
            virtual void recalcProj() = 0;

        protected:
            vec3 _position;
            mat4 _viewMat, _projMat, _vpMat;
            struct CamVectors {
                vec3 forward, up, right;
            } _vectors;
    };

    class OrthographicCamera : public Camera {
        public:
            OrthographicCamera(f32 orthosize, f32 aspect, f32 near = 0.f,
                    f32 far = 100.f);

            /* returns camera's aspect ratio : lens_width/lens_height */
            inline f32 getAspect(){return _props.width/_props.height;}
            /* returns camera's lens height */
            inline f32 getOrthoSize(){return _props.height;}

            /* sets the aspect ratio of the camera lens */
            void setAspectRatio(f32 aspect);
            /* sets the camera's lens height maintaining aspect ratio */
            void setOrthoSize(f32 size);
            /* sets the view range of the camera */
            inline void setViewRange(f32 range){
                ASSERT(range >= 0.f, "Invalid view-range value");
                _props.far = range;
                recalcProj();
            }
            /* sets the near clipping point of camera */
            inline void setNearClip(f32 near){
                ASSERT(near >= 0.f, "Invalid near clip point value");
                _props.near = near;
                recalcProj();
            }

        private:
            virtual void recalcProj() override;

        private:
            struct OrthoClipSpace {
                f32 width, height, near, far;
            } _props;
    };

    class PerspectiveCamera : public Camera {
        public:
            PerspectiveCamera(f32 fov = 60.f, f32 aspect = 16.f/9.f,
                    f32 near=0.f, f32 far=100.f);

            /* sets the field of view of camera */
            inline void setFOV(f32 fov){
                ASSERT(fov >= 0.f, "Invalid FOV");
                _props.fov = fov;
                recalcProj();
            }
            /* sets the view range of the camera */
            inline void setViewRange(f32 range){
                ASSERT(range > 0.f, "Invalid view range");
                _props.far = range;
                recalcProj();
            }
            /* sets the near clipping point of camera */
            inline void setNearClip(f32 near){
                ASSERT(near >= 0, "Invalid near-clip point");
                _props.near = near;
                recalcProj();
            }
        private:
            virtual void recalcProj() override;

        private:
            struct PerpectiveClipSpace {
                f32 fov, aspect, near, far;
            } _props;
    };
}

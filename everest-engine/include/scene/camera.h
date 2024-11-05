/*
 * ============= camera ==============
 * Author: Sunil Sapkota
 * Description: Camera system provides all the required functionalities required
 * for calculating view-frustum, projection, coordinates mapping, etc.
 */

#pragma once


namespace Everest {
    class Camera {
        public:

            /*returns the projection * view matrix of camera*/
            inline mat4 getVPmatrix(){return _vpMat;}
            /* returns camera position */
            inline vec3 getPosition(){return _position;}
            /* returns the direction in which camera is facing */
            inline vec3 getForward(){return _vectors.forward;}

            /* sets the camera position */
            inline void setPosition(vec3 position){
                _position = position;
                recalcView();
            }
            /* makes the camera look towards the given point */
            inline void lookAt(vec3 point){
                _vectors.forward = point - _position;
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
                vec3 forward, up;
            } _vectors;
    };

    class OrthographicCamera : public Camera {
        public:
            OrthographicCamera(vec2 lensSize = vec2(16.f, 9.f), f32 near=0.f,
                    f32 far=100.f);

            /* sets the camera's lens' width and height */
            void setLensSize(vec2 lenSize);
            /* sets the view range of the camera */
            inline void setViewRange(f32 range){
                ASSERT(range > 0.f, "Invalid view-range value");
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
                f32 left, right, bottom, top, near, far;
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

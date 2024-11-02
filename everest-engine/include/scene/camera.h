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
            /* returns the camera tilt */
            inline vec3 getRotation(){return _rotation;}
            /* returns camera position */
            inline vec3 getPosition(){return _position;}

            /* sets the camera position */
            inline void setRotation(vec3 rotation){
                _rotation = rotation;
                recalcView();
            }
            /* sets the camera rotation */
            inline void setPosition(vec3 position){
                _position = position;
                recalcView();
            }

        protected:
            Camera();

            virtual void recalcView() = 0;
            virtual void recalcProj() = 0;

        protected:
            vec3 _position, _rotation; // normally comes from transform, so not pur in constructor
            mat4 _viewMat, _projMat, _vpMat;
            struct CamVectors {
                vec3 forward, up, right;
            } _vectors;
    };

    class OrthographicCamera : public Camera {
        public:
            OrthographicCamera(vec2 lensSize, f32 near=0.f, f32 far=100.f);

            /* sets the camera's lens' width and height */
            void setLensSize(vec2 lenSize);
            /* sets the view range of the camera */
            void setViewRange(f32 range);
            /* sets the near clipping point of camera */
            void setNearClip(f32 near);

        private:
            virtual void recalcView() override;
            virtual void recalcProj() override;

        protected:
            struct OrthoClipSpace {
                f32 left, right, bottom, top, near, far;
            } _props;
    };
}

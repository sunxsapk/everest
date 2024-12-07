#pragma once

#include <everest.h>


namespace Everest {

    class EditorCamera {

        public:
            f32 speed = 10.f, mouseSensitivity = 0.1f, scrollSensitivity = 2.f;
            transform_c transform;
            Camera camera;

        public:
            EditorCamera();

            void onUpdate(bool focused = true);
            void onViewportResize(uvec2 viewportsize);

            vec3 getRight();
            vec3 getForward();
            vec3 getUp();

            void lookAt(vec3 point);

            void setType(CameraType type);

            inline mat4 getTransform(){return transform.getTransform();}
            inline mat4 getVPmat(){return camera.getProjection() *
                glm::inverse(transform.getTransform());}

        private:
            void cam2d_ctrls();
            void cam3d_ctrls();

            void keyControls_2d();
            void keyControls_3d();
            bool mouseControls_2d();
            bool mouseControls_3d();

        public:
            bool _mouseHeld = false;
            ivec2 _mouseLastPos;

    };
}

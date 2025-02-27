#pragma once

#include <everest.h>


namespace Everest {

    class EditorCamera {

        public:
            f32 speed = 10.f, mouseSensitivity = 0.1f, scrollSensitivity = 10.f;
            transform_c transform;
            camera_c camera;

        public:
            EditorCamera();

            void onUpdate(bool focused = true);
            void onViewportResize(uvec2 viewportsize);

            vec3 getRight();
            vec3 getForward();
            vec3 getUp();

            void lookAt(vec3 point);

            vec3 screenToWorldPos(vec2 screenPos);
            vec3 screenPointToDir(vec2 screenPos);

            inline mat4 getView(){return glm::inverse((mat4)transform);}
            inline mat4 getVPmat(){return camera.getProjection() *
                glm::inverse((mat4)transform);}

        private:
            void cam2d_ctrls();
            void cam3d_ctrls();

            void keyControls_2d();
            void keyControls_3d();
            void mouseControls_2d();
            void mouseControls_3d();

        public:
            bool _mouseHeld = false;
            ivec2 _mouseLastPos;

    };
}

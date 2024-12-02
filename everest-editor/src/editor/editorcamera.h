#pragma once

#include <everest.h>


namespace Everest {

    class EditorCamera {

        public:
            f32 speed = 10.f, mouseSensitivity = 0.1f, scrollSensitivity = 2.f;

        public:
            EditorCamera();

            void onUpdate();
            void onViewportResize(uvec2 viewportsize);

            inline mat4 getTransform(){return transform.getTransform();}
            inline Camera& getCamera(){return camera;}

        private:
            void cam2d_ctrls();
            void cam3d_ctrls();

            void keyControls_2d();
            void keyControls_3d();
            void mouseControls_2d();
            void mouseControls_3d();

        private:
            bool _mouseHeld = false;
            ivec2 _mouseLastPos;

        private:
            transform_c transform;
            Camera camera;
    };
}

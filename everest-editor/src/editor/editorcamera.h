#pragma once

#include <everest.h>


namespace Everest {

    class SceneCamera {

        public:
            SceneCamera();

            void onUpdate();
            void onViewportResize(uvec2 viewportsize);

            inline mat4 getTransform(){return transform.getTransform();}
            inline Camera& getCamera(){return camera;}

        private:
            f32 _speed = 10.f;
            f32 _zoomSensitivity = 0.5f, _speedZoomRatio = 1.f;

        private:
            transform_c transform;
            Camera camera;
    };
}

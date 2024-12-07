/*
 * ============ Scene Controls/Settings ==============
 * Author: Sunil Sapkota
 * Description: All of the settings/controls related to scene appearance and functionalities in the
 * editor are all handled here. It provides functionalities like scene camera settings, shading modes,
 * gizmos toggle, scene object manipulation ui, etc.
 */


#pragma once


#include <everest.h>
#include "editorcamera.h"


namespace Everest {

    class ScenePanel {
        public:
            static vec4 sceneBackgroundColor;
        public:
            static void onGUIrender(ref<Framebuffer>& sceneRender, EditorCamera& sceneCamera);
            static inline bool isFocused(){return _focused;}

        private:
            static bool renderSceneViewport(ref<Framebuffer>& sceneRender);
            static void renderSceneSettings(EditorCamera& sceneCamera);
        private:
            static uvec2 _sceneViewPortSize;
            static bool _focused;
    };
}

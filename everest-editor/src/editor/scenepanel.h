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


// TODO: key shortcuts for delete, duplicate, etc

namespace Everest {

    enum class SceneState {
        EDIT = 0,
        PLAY,
    };

    class ScenePanel {
        public:
            static vec4 sceneBackgroundColor;
            static bool gameView;
        public:
            static void onGUIrender(ref<Framebuffer>& sceneRender, EditorCamera& sceneCamera);
            static inline bool isFocused(){return _focused;}

            static void mousePickCheck(ref<Framebuffer>& sceneRenderBuffer);

            static inline vec2 getSceneOffset(){return _sceneOffset;}
            static inline vec2 getSceneViewportSize(){return _sceneViewPortSize;}

            static inline SceneState getSceneState(){ return _sceneState;}

            static inline void onSceneStateToggle(){
                if(_sceneState == SceneState::EDIT) onScenePlay();
                else onSceneStop();
            }

            static bool onKeyShortcuts(KeyDownEvent& event);
        private:
            static bool sceneViewport(ref<Framebuffer>& sceneRender);
            static void sceneSettings(EditorCamera& sceneCamera);
            static void gizmosSettings();

            static void onSceneStop();
            static void onScenePlay();
            friend class ContentBrowser;
        private:
            static SceneState _sceneState;
            static vec2 _sceneOffset;
            static uvec2 _sceneViewPortSize;
            static bool _focused;
    };
}

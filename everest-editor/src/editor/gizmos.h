/*
 * ============= Gizmos ===============
 * Author: Sunil Sapkota
 * Description: Contains the gizmos for the scene viewport
 */

#pragma once
#include <everest.h>
#include "editorcamera.h"


namespace Everest {

    class Gizmos {
        public:
            static bool showGizmos;
        public:
            static void init();
            static void quit();
            static void renderGrid(EditorCamera& camera, vec2 size = {1000.f, 1000.f});
            static void renderTransformationGizmo(EditorCamera& cam);

            static inline bool isUsing(){ return ImGuizmo::IsUsing() ||
                    (showGizmos && ImGuizmo::IsOver());}

        private:
            static void initGrid();
            static ImGuizmo::OPERATION operation;
            static bool isLocalTransform;
        private:
             ref<Shader> gridShader;
             ref<VAO> gridVertexArray;
             friend class ScenePanel;
        private:
             static Gizmos* _instance;
    };

}

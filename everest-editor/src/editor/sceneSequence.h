#pragma once

#include <everest.h>

namespace Everest {

    class SceneSequenceUI {
        public:
            static void onGUIRender();

        private:
            static void _scenesReorderUI(std::vector<std::filesystem::path> &scenes);
            static void _sceneDropper(std::vector<std::filesystem::path> &scenes);
    };
}

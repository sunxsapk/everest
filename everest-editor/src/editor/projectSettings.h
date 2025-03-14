#pragma once

#include <everest.h>

namespace Everest {

    class ProjectSettingsUI {
        public:
            static void onGUIRender(project_def_t& project);

        private:
            static void _scenesReorderUI(std::vector<std::filesystem::path> &scenes);
            static void _sceneDropper(std::vector<std::filesystem::path> &scenes);
    };
}

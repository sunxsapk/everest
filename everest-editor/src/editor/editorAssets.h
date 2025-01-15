/*
 * ========== Editor Assets ============
 * Author: Sunil Sapkota
 * Description: Editor assets sits above the assets manager provided by everest-engine and manages the
 * assets required inside editor.
 */



#pragma once

#include <everest.h>


namespace Everest {

    enum class IconType {
        DIRECTORY = 0,
        FILE,
        SCENE,
        PLAY,
        STOP,
    };

    class EditorAssets {

        public:

            static void init();
            static void quit();

            static spriteRenderer_c& getIcon(IconType icon);

        private:
            std::vector<spriteRenderer_c> icons;
            static EditorAssets* _instance;

            EditorAssets();
    };
}

/*
 * ========= Content Browser ==============
 * Author: Sunil Sapkota
 * Description: Panel to display content browser, for game assets
 */


#pragma once
#include <filesystem>
#include <everest.h>


namespace Everest {

    class ContentBrowser {
        public:

            static void onGUIrender();

        private:
            static std::filesystem::path curDir, assetsDir;

            static Sprite _getIconForEntry(const std::filesystem::directory_entry& entry);

    };

}

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

            static void init();
            static void onGUIrender();
            static void quit();

        private:
            std::filesystem::path curDir, assetsDir;
            Sprite i_directory, i_file;
        private:
            static ContentBrowser* _instance;

    };

}

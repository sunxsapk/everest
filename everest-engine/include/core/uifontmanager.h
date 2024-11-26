/*
 * ======= UI Font Manager ==============
 * Author: Sunil Sapkota
 * Description: Manages the fonts for UI. Provides an abstraction for loading and using fonts
 * for UI
 */

#pragma once


namespace Everest {
    using c_str_t = const char*;
    class UIFontManager {
        public:
            static void init();
            static ImFont* loadFont(c_str_t name, c_str_t filepath, f32 fontSize = 18.f);

            static inline ImFont* getDefaultRegular(){return _fonts[default_regular];}
            static inline ImFont* getDefaultBold(){return _fonts[default_bold];}

        private:
            static std::unordered_map<c_str_t, ImFont*> _fonts;

        private:
            static c_str_t default_regular;
            static c_str_t default_bold;
    };
}

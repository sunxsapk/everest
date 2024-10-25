/*
 * ======== window class =========
 * Author: Sunil Sapkota
 * Description: handles creation, manage and deletion of window
 */

#pragma once
#include "math/vec.h"

namespace Everest {
    class Window {
        public:
            Window(i32 width, i32 height, const char* title);
            ~Window();

            void convertFullScreen();
            void convertWindowedMode();
            void closeWindow();
            bool shouldClose();

            void clear();
            void update();

        protected:
            GLFWwindow* _window;
            std::string _title;
            ivec2 _position, _size;

            GLFWmonitor* getCurrentMonitor();

            static void onFBResize(GLFWwindow* window, int width, int height);
    };
}


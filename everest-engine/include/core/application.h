/*
 * ====== Application =======
 * Author: Sunil Sapkota
 * Description: the main application class's instance is what runs when a
 * project using everest engine is ran.
 */

#pragma once

#include "window.h"

namespace Everest {
    class Application {
        public:
            Application(const char* name = "Everest-App");
            ~Application();
            virtual void run();
        protected:
            Window* window;
    };
}

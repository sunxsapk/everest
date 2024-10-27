/*
 * ====== Application =======
 * Author: Sunil Sapkota
 * Description: the main application class's instance is what runs when a
 * project using everest engine is ran.
 */

#pragma once

#include "window.h"
#include "layerstack.h"
#include "events/windowevent.h"

namespace Everest {
    class Application {
        public:
            Application(const char *name = "Everest-App");
            ~Application();

            void pushLayer(Layer* layer);

            virtual void run();
            virtual void onEvent(Event& event);
        protected:
            Window *_window; 
            bool _running = true;
            LayerStack _layerStack;

            bool onClose(WindowCloseEvent& e);
    };
}

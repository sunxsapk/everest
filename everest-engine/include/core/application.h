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
#include "core/debuglayer.h"

namespace Everest {
    class Application {
        public:
            Application(const char *name = "Everest-App");
            ~Application();

            void pushLayer(Layer* layer);

            virtual void onStart();
            virtual void run();
            virtual void onEvent(Event& event);
            virtual void onClose(){}

            static inline Window& getAppWindow(){return *s_app->_window;}
        protected:
            std::unique_ptr<Window> _window; 
            bool _running = true;
            LayerStack _layerStack;

            bool onWindowClose(WindowCloseEvent& e);
        private:
            static Application* s_app;
            const char *_name;
            std::unique_ptr<DebugLayer> debugger;

            void initWindow();
            void attachDebugger();
    };
}

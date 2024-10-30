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
            /* initializes everything and creates a application instance*/
            Application(const char *name = "Everest-App");
            ~Application();

            /*runs the core application loop*/
            void run();
            /*pushes and attaches a new layer on the application layer stack*/
            void pushLayer(Layer* layer);
            /*called whenever the application recieves an event*/
            void onEvent(Event& event);

            /* returns the window wrapper used by the application*/
            static inline Window& getAppWindow(){return *s_app->_window;}
        protected:
            std::unique_ptr<Window> _window; 
            bool _running = true;
            LayerStack _layerStack;

            /*called when the window close event is recieved by the application*/
            bool onWindowClose(WindowCloseEvent& e);
        private:
            static Application* s_app;
            const char *_name;
            std::unique_ptr<DebugLayer> debugger;

            /* initializes the window of the application and binds the event
             * callbacks*/
            void initWindow();
            /*attaches a application debugger gui*/
            void attachDebugger();
    };
}

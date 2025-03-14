/*
 * ====== Application =======
 * Author: Sunil Sapkota
 * Description: the main application class's instance is what runs when a
 * project using everest engine is ran.
 */

#pragma once

#include "math/types.h"
#include "window.h"
#include "layerstack.h"
#include "events/windowevent.h"
#include "core/guilayer.h"

namespace Everest {
    using namespace Everest;

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

            /* closes the application */
            static inline void close(){s_app->_running = false;}
            /* returns the window wrapper used by the application*/
            static inline Window& getAppWindow(){return *(s_app->_window);}
        protected:
            scope<Window> _window; 
            bool _running = true;
            bool _minimized = false;
            LayerStack _layerStack;

            /*called when the window close event is recieved by the application*/
            bool onWindowClose(WindowCloseEvent& e);
            bool onWindowResize(WindowResizeEvent& e);

        private:
            static Application* s_app;
            const char *_name;
            scope<GUILayer> guilayer;

            /* initializes the window of the application and binds the event
             * callbacks*/
            void initWindow();
            /*attaches a application debugger gui*/
            void attachUILayer();
    };

    struct CommandLineArgs {
        i32 argc;
        char** argv;
    };
    Application* createApplication(CommandLineArgs args);
}

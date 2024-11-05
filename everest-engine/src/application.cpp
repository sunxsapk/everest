#include "events/mouseevent.h"
#include "core/application.h"
#include "core/core.h"
#include "core/input.h"
#include "core/time.h"

namespace Everest {
#define DEF_WIN_W 1024
#define DEF_WIN_H 768

    Application *Application::s_app = NULL;

    Application::Application(const char *name){
        ASSERT(Application::s_app == NULL, "Can't have multiple instances of application");
        s_app = this;
        ASSERT(name, "Application should have a name");
        this->_name = name;

        Core::initDependencies();
        this->initWindow();
        Core::init();
        atexit(Core::quit);

        this->attachDebugger();
    }

    void Application::attachDebugger(){
        this->debugger = scoped<DebugLayer>();
        this->debugger->onAttach();
    }

    void Application::initWindow(){
        this->_window = scoped<Window>( DEF_WIN_W, DEF_WIN_H, _name);
        this->_window->setEventCallback(BIND_EVENT_CB(Application::onEvent));
    }

    Application::~Application(){
        this->debugger->onDetach();
        this->_window.reset();
    }

    void Application::pushLayer(Layer* layer){
        this->_layerStack.pushLayer(layer);
        layer->onAttach();
    }

    void Application::run(){
        while(this->_running){
            Input::_clearPoll();
            this->_window->update();
            Time::tick();

            for(Layer* layer:this->_layerStack){
                layer->onUpdate();
            }

            this->debugger->begin();
            for(Layer* layer:this->_layerStack){
                layer->onDebugRender();
            }
#ifdef DEBUG
            this->debugger->onDebugRender();
#endif
            this->debugger->end();

            this->_window->swapBuffers();
        }
    }

    void Application::onEvent(Event& event){
        EventDispatcher dispatcher(event);
        dispatcher.dispatch<WindowCloseEvent>(
                BIND_EVENT_CB(Application::onWindowClose));
        dispatcher.dispatch<MouseScrolledEvent>(Input::_scrollPoll);

        for(auto it = this->_layerStack.rbegin();
                it != this->_layerStack.rend(); ++it){
            if(event._handled) break;
            (*it)->onEvent(event);
        }
    }

    bool Application::onWindowClose(WindowCloseEvent& e){
        this->_running = false;
        return true;
    }
}

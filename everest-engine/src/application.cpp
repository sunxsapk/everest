#include "core/application.h"
#include "core/core.h"

namespace Everest {
#define DEF_WIN_W 1024
#define DEF_WIN_H 768

    Application *Application::s_app = NULL;

    Application::Application(const char *name){
        ASSERT(Application::s_app == NULL);
        s_app = this;
        ASSERT(name);
        this->_name = name;

        Core::init();
        atexit(Core::quit);

        this->initWindow();
        this->attachDebugger();
        this->onStart();
    }

    void Application::attachDebugger(){
        this->debugger = std::make_unique<DebugLayer>();
        this->debugger->onAttach();
    }

    void Application::initWindow(){
        this->_window = std::make_unique<Window>( DEF_WIN_W, DEF_WIN_H, _name);
        this->_window->setEventCallback(BIND_EVENT_CB(Application::onEvent));
    }

    Application::~Application(){
        this->debugger->onDetach();
        this->onClose();
        this->_window.reset();
    }

    void Application::onStart(){
    }

    void Application::pushLayer(Layer* layer){
        this->_layerStack.pushLayer(layer);
        layer->onAttach();
    }

    void Application::run(){
        while(this->_running){
            this->_window->update();

            for(Layer* layer:this->_layerStack){
                layer->onUpdate();
            }

            this->_window->clear(0.f, .3f, .3f, .3f);

#ifdef DEBUG
            this->debugger->begin();
            for(Layer* layer:this->_layerStack){
                layer->onDebugRender();
            }
            this->debugger->onDebugRender();
            this->debugger->end();
#endif

            this->_window->swapBuffers();
        }
    }

    void Application::onEvent(Event& event){
        EventDispatcher dispatcher(event);
        dispatcher.dispatch<WindowCloseEvent>(
                BIND_EVENT_CB(Application::onWindowClose));

        for(auto it = this->_layerStack.rbegin();
                it != this->_layerStack.rend(); ++it){
            if(event._handled) break;
            (*it)->onEvent(event);
        }
        EVLog_Msg("%s", event.toString().c_str());
    }

    bool Application::onWindowClose(WindowCloseEvent& e){
        this->_running = false;
        return true;
    }
}

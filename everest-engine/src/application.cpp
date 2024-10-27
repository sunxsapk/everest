#include "core/application.h"

namespace Everest {
#define DEF_WINDOW_WIDTH 1024
#define DEF_WINDOW_HEIGHT 768

    Application::Application(const char *name){
        this->_window = new Window(DEF_WINDOW_WIDTH, DEF_WINDOW_HEIGHT, name);
        this->_window->setEventCallback(BIND_EVENT_CB(Application::onEvent));
    }

    Application::~Application(){
        delete this->_window;
    }

    void Application::pushLayer(Layer* layer){
        this->_layerStack.pushLayer(layer);
    }

    void Application::run(){
        while(this->_running){
            this->_window->clear(0.f, .3f, .3f, .3f);

            for(Layer* layer:this->_layerStack){
                layer->onUpdate();
            }

            this->_window->update();
        }
    }

    void Application::onEvent(Event& event){
        EventDispatcher dispatcher(event);
        dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_CB(Application::onClose));

        for(auto it = this->_layerStack.rbegin();
                it != this->_layerStack.rend(); ++it){
            if(event._handled) break;
            (*it)->onEvent(event);
        }

        EVLog_Msg("%s", event.toString().c_str());
    }

    bool Application::onClose(WindowCloseEvent& e){
        this->_running = false;
        return true;
    }
}

#include "core/application.h"

namespace Everest {
    Application::Application(const char *name){
        this->_window = new Window(1024, 768, name);
        this->_window->setEventCallback(BIND_EVENT_CB(Application::onEvent));
    }

    Application::~Application(){
        delete this->_window;
    }

    void Application::run(){
        while(this->_running){
            this->_window->clear(0.f, .3f, .3f, .3f);
            this->_window->update();
        }
    }

    void Application::onEvent(Event& event){
        EventDispatcher dispatcher(event);
        dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_CB(Application::onClose));
        EVLog_Msg("%s", event.toString().c_str());
    }

    bool Application::onClose(WindowCloseEvent& e){
        this->_running = false;
        return true;
    }
}

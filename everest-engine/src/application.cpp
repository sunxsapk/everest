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
        while(!this->_window->shouldClose()){
            this->_window->clear(0.f, 1.f, 1.f, .3f);
            this->_window->update();
        }
    }
    void Application::onEvent(Event& event){
        EVLog_Msg("%s", event.toString().c_str());
    }
}

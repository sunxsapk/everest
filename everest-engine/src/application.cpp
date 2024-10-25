#include "core/application.h"

namespace Everest {
    Application::Application(const char* name){
        this->window = new Window(1024, 768, name);
    }

    Application::~Application(){
    }

    void Application::run(){
        while(!this->window->shouldClose()){
            this->window->clear();
            this->window->update();
        }
    }
}

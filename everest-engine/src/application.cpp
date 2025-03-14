#include "core/application.h"
#include "events/mouseevent.h"
#include "core/core.h"
#include "core/input.h"
#include "core/time.h"
#include "renderer/renderer.h"

namespace Everest {
#define DEF_WIN_W 1024
#define DEF_WIN_H 768

    Application* Application::s_app = nullptr;

    Application::Application(const char *name){
        EV_profile_function();


        ASSERT(Application::s_app == NULL, "Can't have multiple instances of application");
        s_app = this;
        ASSERT(name, "Application should have a name");
        this->_name = name;

        Core::initDependencies();
        this->initWindow();
        Core::init();
        atexit(Core::quit);

        this->attachUILayer();
    }

    void Application::attachUILayer(){
        EV_profile_function();


        this->guilayer = createScope<GUILayer>();
        this->guilayer->onAttach();
    }

    void Application::initWindow(){
        EV_profile_function();


        this->_window = createScope<Window>( DEF_WIN_W, DEF_WIN_H, _name);
        this->_window->setEventCallback(BIND_EVENT_CB(Application::onEvent));
    }

    Application::~Application(){
        EV_profile_function();

        this->guilayer->onDetach();
        this->_window.reset();
    }

    void Application::pushLayer(Layer* layer){
        EV_profile_function();


        this->_layerStack.pushLayer(layer);
        layer->onAttach();
    }

    void Application::run(){
        EV_profile_function();


        Time::begin();
        while(this->_running){
            EV_profile_scope("Main Loop");


            Input::update();

            if(!_minimized){
                EV_profile_scope("Layer update");


                for(Layer* layer:this->_layerStack){
                    layer->onUpdate();
                }
            }

            {
                EV_profile_scope("GUI layer Render");


                this->guilayer->begin();
                for(Layer* layer:this->_layerStack){
                    layer->onGUIrender();
                }
                this->guilayer->end();
            }

            Input::clearScrollPoll();
            this->_window->update();
            Time::tick();
        }
    }

    void Application::onEvent(Event& event){
        EventDispatcher dispatcher(event);
        dispatcher.dispatch<WindowCloseEvent>(
                BIND_EVENT_CB(Application::onWindowClose));
        dispatcher.dispatch<MouseScrolledEvent>(Input::_scrollPoll);
        dispatcher.dispatch<WindowResizeEvent>(
                BIND_EVENT_CB(Application::onWindowResize));

        for(auto it = this->_layerStack.rbegin();
                it != this->_layerStack.rend(); ++it){
            if(event._handled) break;
            (*it)->onEvent(event);
        }
    }

    bool Application::onWindowResize(WindowResizeEvent& e){
        if(e.getWidth() == 0 || e.getHeight() == 0){
            _minimized = true;
        } else {
            _minimized = false;
            Renderer::setViewPort(0, 0, e.getWidth(), e.getHeight());
        }
        return false;
    }

    bool Application::onWindowClose(WindowCloseEvent& e){
        this->_running = false;
        return true;
    }
}

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "events/mouseevent.h"
#include "core/application.h"
#include "core/core.h"
#include "core/input.h"

#include "renderer/buffer.h"
#include "renderer/vao.h"
#include "renderer/shader.h"

namespace Everest {
#define DEF_WIN_W 1024
#define DEF_WIN_H 768

    Application *Application::s_app = NULL;

    Application::Application(const char *name){
        ASSERT(Application::s_app == NULL);
        s_app = this;
        ASSERT(name);
        this->_name = name;

        Core::initDependencies();
        this->initWindow();
        Core::init();
        atexit(Core::quit);

        this->attachDebugger();
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
        this->_window.reset();
    }

    void Application::pushLayer(Layer* layer){
        this->_layerStack.pushLayer(layer);
        layer->onAttach();
    }

    void Application::run(){

#define DEBUGTRIANGLE 1
#if DEBUGTRIANGLE
        f32 verts[] = {
             0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
             0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
        };
        u32 inds[3] = {
            0, 1, 2,
        };

        const char* vsh = R"(
            #version 330 core
            layout (location = 0) in vec3 pos;
            layout (location = 1) in vec3 colorv;
            out vec3 colorp;

            void main() {
                gl_Position = vec4(pos, 1.0);
                colorp = colorv;
            }
        )";

        const char* fsh = R"(
            #version 330 core
            out vec4 FragColor;  
            in vec3 colorp;
              
            void main() {
                FragColor = vec4(colorp, 1.0);
            }
        )";

        VAO vao;
        vao.bind();

        VertexBuffer vb(verts, sizeof(verts));
        IndexBuffer ib(inds, sizeof(inds));

        vao.beginLayout();
        vao.layout(3, GL_FLOAT, 6*sizeof(float));
        vao.layout(3, GL_FLOAT, 6*sizeof(float));
        vao.endLayout();

        vb.unbind();
        ib.unbind();

        Shader sh(vsh, fsh);
#endif

        while(this->_running){
            Input::_clearPoll();
            this->_window->update();
            Time::tick();

            for(Layer* layer:this->_layerStack){
                layer->onUpdate();
            }

            this->_window->clear(.1f, .1f, .1f);

#if DEBUGTRIANGLE
            vao.bind();
            sh.bind();
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);
            sh.unbind();
            vao.unbind();
#endif

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

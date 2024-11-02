#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "events/mouseevent.h"
#include "core/application.h"
#include "core/core.h"
#include "core/input.h"

#include "renderer/buffer.h"
#include "renderer/vao.h"
#include "renderer/shader.h"
#include "renderer/renderer.h"

#include "scene/camera.h"

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
            -1.0f, -1.0f, 1.0f,  0.f, 0.f, 0.f,
             1.0f, -1.0f, 1.0f,  1.f, 0.f, 0.f,
             1.0f,  1.0f, 1.0f,  0.f, 1.f, 0.f,
            -1.0f,  1.0f, 1.0f,  0.f, 0.f, 1.f,
        };
        u32 inds[] = {
            0, 1, 2,
            2, 3, 0
        };

        BufferLayout layout {
            {T_vec3, "aPos"},
            {T_vec3, "aColor"},
        };

        const char* vsh = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aColor;

            uniform mat4 u_viewProjMat;
            out vec3 fColor;

            void main() {
                gl_Position = u_viewProjMat * vec4(aPos, 1.0);
                fColor = aColor;
            }
        )";

        const char* fsh = R"(
            #version 330 core
            out vec4 FragColor;
            in vec3 fColor;
            
            void main() {
                FragColor = vec4(fColor, 1.0);
            }
        )";

        p_shared(VAO) vao = std::make_shared<VAO>();
        p_shared(VertexBuffer) vb = std::make_shared<VertexBuffer>
            (verts, sizeof(verts));
        vb->setLayout(layout);

        p_shared(IndexBuffer) ib = std::make_shared<IndexBuffer>
            (inds, sizeof(inds)); 

        vao->addVertexBuffer(vb);
        vao->addIndexBuffer(ib);

        Shader sh(vsh, fsh);
        OrthographicCamera cam({16.f,8.f});
        cam.setRotation(vec3(0, -pi<f32>()/2, 0));
#endif

        Renderer::issue_setClearColor({.1f, .1f, .1f, 1.f});
        while(this->_running){
            Input::_clearPoll();
            this->_window->update();
            Time::tick();

            for(Layer* layer:this->_layerStack){
                layer->onUpdate();
            }

            Renderer::issue_clear();
            Renderer::beginScene(&cam);
#if DEBUGTRIANGLE
            sh.bind();
            sh.setUniform_Mat4("u_viewProjMat", cam.getVPmatrix());
            Renderer::submit(vao);
#endif
            Renderer::endScene();

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

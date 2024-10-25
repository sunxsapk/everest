#include "core/window.h"

namespace Everest{
    Window::Window(i32 width, i32 height, const char* title){
        this->_title = title;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
        glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE);
        glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

        this->_window = glfwCreateWindow(width, height, title, NULL, NULL);
        ASSERT(this->_window != NULL);

        glfwMakeContextCurrent(this->_window);

        int _glLoaded = gladLoadGL(glfwGetProcAddress);
        ASSERT(_glLoaded);

        glfwGetWindowPos(this->_window, &this->_position.x, &this->_position.y);
        glfwGetWindowSize(this->_window, &this->_size.x, &this->_size.y);
        glfwSwapInterval(1);

        glfwSetFramebufferSizeCallback(this->_window, Window::onFBResize);
    }

    Window::~Window(){
        ASSERT(this->_window);
        glfwDestroyWindow(this->_window);
        EVLog_Msg("Window destroyed");
    }

    void Window::onFBResize(GLFWwindow* window, int width, int height){
        glViewport(0, 0, width, height);
    }

    void Window::convertFullScreen(){
        ASSERT(this->_window);
        GLFWmonitor* monitor = this->getCurrentMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        ASSERT(mode);
        glfwSetWindowMonitor(this->_window, monitor, 0, 0, mode->width,
                mode->height, mode->refreshRate);
    }

    void Window::convertWindowedMode(){
        ASSERT(this->_window);
        GLFWmonitor* monitor = this->getCurrentMonitor();
        glfwSetWindowMonitor(this->_window, NULL, this->_position.x,
                this->_position.y, this->_size.x, this->_size.y, 0);
    }

    void Window::closeWindow(){
        ASSERT(this->_window);
        glfwSetWindowShouldClose(this->_window, GLFW_TRUE);
    }

    void Window::clear(){
        glClearColor(0.0f, 0.2f, 0.2f, 0.2f);
        glClear(GL_COLOR_BUFFER_BIT);

    }

    void Window::update(){
        glfwSwapBuffers(this->_window);
        glfwPollEvents();
    }


    bool Window::shouldClose(){
        ASSERT(this->_window);
        return glfwWindowShouldClose(this->_window);
    }

    GLFWmonitor* Window::getCurrentMonitor(){
        // TODO : get current monitor
        GLFWmonitor* _monitor = glfwGetPrimaryMonitor();
        ASSERT(_monitor);
        return _monitor;
    }
}

#include "core/window.h"
#include "events/keyevent.h"
#include "events/mouseevent.h"
#include "events/windowevent.h"

namespace Everest{
    Window::Window(i32 width, i32 height, std::string title){
        this->_winData.position = {0, 0};
        this->_winData.size = {width, height};
        this->_winData.title = title;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
        glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE);
        glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

        this->_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        ASSERT(this->_window != NULL);

        glfwMakeContextCurrent(this->_window);

        i32 _glLoaded = gladLoadGL(glfwGetProcAddress);
        ASSERT(_glLoaded);

        glfwSwapInterval(1);
        glfwSetWindowUserPointer(this->_window, &this->_winData);

        glfwSetErrorCallback(Window::onError);
        glfwSetFramebufferSizeCallback(this->_window, Window::onFBResize);
        glfwSetWindowSizeCallback(this->_window, Window::onResize);
        glfwSetWindowPosCallback(this->_window, Window::onMove);
        glfwSetWindowCloseCallback(this->_window, Window::onClose);
        glfwSetMouseButtonCallback(this->_window, Window::onMouseBtn);
        glfwSetCursorPosCallback(this->_window, Window::onMouseMove);
        glfwSetScrollCallback(this->_window, Window::onScroll);
        glfwSetKeyCallback(this->_window, Window::onKeyEvent);

    }

    Window::~Window(){
        ASSERT(this->_window != NULL);
        glfwDestroyWindow(this->_window);
    }

    void Window::convertFullScreen(){
        ASSERT(this->_window != NULL);
        GLFWmonitor* monitor = this->getCurrentMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        ASSERT(mode != NULL);
        glfwSetWindowMonitor(this->_window, monitor, 0, 0, mode->width,
                mode->height, mode->refreshRate);
    }

    void Window::convertWindowedMode(){
        ASSERT(this->_window != NULL);
        GLFWmonitor* monitor = this->getCurrentMonitor();
        glfwSetWindowMonitor(this->_window, NULL,
                this->_winData.position.x, this->_winData.position.y,
                this->_winData.size.x, this->_winData.size.y, 0);
    }

    void Window::clear(f32 r, f32 g, f32 b, f32 a){
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Window::setTitle(const char* title){
        ASSERT(this->_window != NULL && title != NULL);
        this->_winData.title = title;
        glfwSetWindowTitle(this->_window, title);
    }

    void Window::update(){
        glfwSwapBuffers(this->_window);
        glfwPollEvents();
    }

    GLFWmonitor* Window::getCurrentMonitor(){
        // TODO : get current monitor
        GLFWmonitor* _monitor = glfwGetPrimaryMonitor();
        ASSERT(_monitor);
        return _monitor;
    }

    void Window::onFBResize(GLFWwindow* window, i32 width, i32 height){
        glViewport(0, 0, width, height);
    }

    void Window::onKeyEvent(GLFWwindow *window, i32 key, i32 scancode, i32 action, i32 mods){
        WindowData _data = *(WindowData*)glfwGetWindowUserPointer(window);
        if(action == GLFW_PRESS){
            KeyDownEvent evt(key);
            _data.eventCallback(evt);
        } else if(action == GLFW_REPEAT){
            KeyRepeatEvent evt(key);
            _data.eventCallback(evt);
        } else {
            KeyUpEvent evt(key);
            _data.eventCallback(evt);
        }
    }

    void Window::onScroll(GLFWwindow* window, f64 dx, f64 dy){
        WindowData _data = *(WindowData*)glfwGetWindowUserPointer(window);
        MouseScrolledEvent evt(dx, dy);
        _data.eventCallback(evt);
    }

    void Window::onMouseMove(GLFWwindow* window, f64 x, f64 y){
        WindowData _data = *(WindowData*)glfwGetWindowUserPointer(window);
        MouseMovedEvent evt(x, y);
        _data.eventCallback(evt);
    }
    void Window::onResize(GLFWwindow* window, i32 width, i32 height){
        WindowData _data = *(WindowData*)glfwGetWindowUserPointer(window);
        _data.size = {width, height};
        WindowResizeEvent evt(width, height);
        _data.eventCallback(evt);
    }

    void Window::onMove(GLFWwindow* window, i32 x, i32 y){
        WindowData _data = *(WindowData*)glfwGetWindowUserPointer(window);
        _data.size = {x, y};
        WindowMovedEvent evt(x, y);
        _data.eventCallback(evt);
    }

    void Window::onClose(GLFWwindow *window){
        WindowData _data = *(WindowData*)glfwGetWindowUserPointer(window);
        WindowCloseEvent evt;
        _data.eventCallback(evt);
    }

    void Window::onMouseBtn(GLFWwindow* window, i32 button, i32 action, i32 mods){
        WindowData _data = *(WindowData*)glfwGetWindowUserPointer(window);
        if(action == GLFW_PRESS){
            MouseButtonDownEvent evt(button);
            _data.eventCallback(evt);
        }else {
            MouseButtonUpEvent evt(button);
            _data.eventCallback(evt);
        }
    }

    void Window::onError(i32 errcode, const char *err){
        EVLog_Err("Opengl Error (%d): %s", errcode, err);
    }
}

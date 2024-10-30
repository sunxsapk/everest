#include "core/input.h"
#include "core/application.h"

namespace Everest {
    GLFWwindow* Input::s_window = NULL;
    vec2 Input::s_scroll = {0,0};

    void Input::init(){
        s_window = Application::getAppWindow().getWindow();
        ASSERT(s_window != NULL);
        setStickyKeys(false);
    }

    void Input::setStickyKeys(bool mode){
        glfwSetInputMode(s_window, GLFW_STICKY_KEYS, mode);
    }

    bool Input::keyDown(Keycode keycode){
        int st = glfwGetKey(s_window, keycode);
        return st == GLFW_PRESS || st == GLFW_REPEAT;
    }

    bool Input::keyUp(Keycode keycode){
        return glfwGetKey(s_window, keycode) == GLFW_RELEASE;
    }

    bool Input::keyRepeat(Keycode keycode){
        return glfwGetKey(s_window, keycode) == GLFW_REPEAT;
    }

    bool Input::mouseButtonDown(MouseButton button){
        return glfwGetMouseButton(s_window, button) == GLFW_PRESS;
    }

    bool Input::mouseButtonUp(MouseButton button){
        return glfwGetMouseButton(s_window, button) == GLFW_RELEASE;
    }

    ivec2 Input::mousePosition(){
        f64 x, y;
        glfwGetCursorPos(s_window, &x, &y);
        return {.x = (int)round(x), .y = (int)round(y)};
    }

    i32 Input::mousePositionX(){
        f64 x, y;
        glfwGetCursorPos(s_window, &x, &y);
        return round(x);
    }
    i32 Input::mousePositionY(){
        f64 x, y;
        glfwGetCursorPos(s_window, &x, &y);
        return round(y);
    }

    bool Input::_scrollPoll(MouseScrolledEvent& event){
        s_scroll = event.getDel();
        return false;
    }

    void Input::_clearPoll(){
        s_scroll = {0,0};
    }
}

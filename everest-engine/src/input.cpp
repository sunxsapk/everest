#include "core/input.h"

namespace Everest {

    Keycode InputAxisKeys::horizontal_p = Keycode::K_d, InputAxisKeys::horizontal_n = Keycode::K_a;
    Keycode InputAxisKeys::vertical_p = Keycode::K_w, InputAxisKeys::vertical_n = Keycode::K_s;

    Keycode InputAxisKeys::horizontal_ap = Keycode::K_right, InputAxisKeys::horizontal_an = Keycode::K_left;
    Keycode InputAxisKeys::vertical_ap = Keycode::K_up, InputAxisKeys::vertical_an = Keycode::K_down;

    GLFWwindow* Input::s_window = NULL;
    std::set<Keycode> Input::s_keyStates, Input::s_tempKeyStates;
    vec2 Input::s_scroll(0);
    vec2 Input::s_axis(0);
    vec2 Input::s_mousePosition(0);

    void Input::update(){
        //s_scroll = {0,0};
        dvec2 mp;
        glfwGetCursorPos(s_window, &mp.x, &mp.y);
        s_mousePosition = glm::round(mp);

        s_axis.x = (getKey(InputAxisKeys::horizontal_p) | getKey(InputAxisKeys::horizontal_ap))
            - (getKey(InputAxisKeys::horizontal_n) | getKey(InputAxisKeys::horizontal_an));
        s_axis.y = (getKey(InputAxisKeys::vertical_p) | getKey(InputAxisKeys::vertical_ap))
            - (getKey(InputAxisKeys::vertical_n) | getKey(InputAxisKeys::vertical_an));

        if(s_keyStates.empty()) return;

        s_tempKeyStates.clear();
        for(auto key : s_keyStates){
            if(glfwGetKey(s_window, key) == GLFW_PRESS){
                s_tempKeyStates.insert(key);
            }
        }
        std::swap(s_keyStates, s_tempKeyStates);
    }
}

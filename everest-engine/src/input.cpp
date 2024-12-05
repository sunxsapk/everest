#include "core/input.h"

namespace Everest {

    Keycode InputAxisKeys::horizontal_p = Keycode::K_d, InputAxisKeys::horizontal_n = Keycode::K_a;
    Keycode InputAxisKeys::vertical_p = Keycode::K_w, InputAxisKeys::vertical_n = Keycode::K_s;

    Keycode InputAxisKeys::horizontal_ap = Keycode::K_right, InputAxisKeys::horizontal_an = Keycode::K_left;
    Keycode InputAxisKeys::vertical_ap = Keycode::K_up, InputAxisKeys::vertical_an = Keycode::K_down;

    GLFWwindow* Input::s_window = NULL;
    vec2 Input::s_scroll(0);
    vec2 Input::s_axis(0);
    vec2 Input::s_mousePosition(0);


    bool Input::_scrollPoll(MouseScrolledEvent& event){
        s_scroll += event.getDel();
        return false;
    }
}

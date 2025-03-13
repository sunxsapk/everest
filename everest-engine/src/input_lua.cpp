#include "scripting/input_lua.h"
#include "core/input.h"


namespace Everest {
    namespace Scripting {

        void reg_input(luastate_t& lua) {
            lua.new_enum("Keycode",
                "K_unknown", K_unknown,
                "K_space", K_space,
                "K_apostrophe", K_apostrophe,
                "K_comma", K_comma,
                "K_minus", K_minus,
                "K_period", K_period,
                "K_slash", K_slash,
                "K_0", K_0,
                "K_1", K_1,
                "K_2", K_2,
                "K_3", K_3,
                "K_4", K_4,
                "K_5", K_5,
                "K_6", K_6,
                "K_7", K_7,
                "K_8", K_8,
                "K_9", K_9,
                "K_semicolon", K_semicolon,
                "K_equal", K_equal,
                "K_a", K_a,
                "K_b", K_b,
                "K_c", K_c,
                "K_d", K_d,
                "K_e", K_e,
                "K_f", K_f,
                "K_g", K_g,
                "K_h", K_h,
                "K_i", K_i,
                "K_j", K_j,
                "K_k", K_k,
                "K_l", K_l,
                "K_m", K_m,
                "K_n", K_n,
                "K_o", K_o,
                "K_p", K_p,
                "K_q", K_q,
                "K_r", K_r,
                "K_s", K_s,
                "K_t", K_t,
                "K_u", K_u,
                "K_v", K_v,
                "K_w", K_w,
                "K_x", K_x,
                "K_y", K_y,
                "K_z", K_z,
                "K_left_bracket", K_left_bracket,
                "K_backslash", K_backslash,
                "K_right_bracket", K_right_bracket,
                "K_tilde", K_tilde,
                "K_escape", K_escape,
                "K_enter", K_enter,
                "K_tab", K_tab,
                "K_backspace", K_backspace,
                "K_insert", K_insert,
                "K_delete", K_delete,
                "K_right", K_right,
                "K_left", K_left,
                "K_down", K_down,
                "K_up", K_up,
                "K_page_up", K_page_up,
                "K_page_down", K_page_down,
                "K_home", K_home,
                "K_end", K_end,
                "K_caps_lock", K_caps_lock,
                "K_scroll_lock", K_scroll_lock,
                "K_num_lock", K_num_lock,
                "K_print_screen", K_print_screen,
                "K_pause", K_pause,
                "K_f1", K_f1,
                "K_f2", K_f2,
                "K_f3", K_f3,
                "K_f4", K_f4,
                "K_f5", K_f5,
                "K_f6", K_f6,
                "K_f7", K_f7,
                "K_f8", K_f8,
                "K_f9", K_f9,
                "K_f10", K_f10,
                "K_f11", K_f11,
                "K_f12", K_f12,
                "K_f13", K_f13,
                "K_f14", K_f14,
                "K_f15", K_f15,
                "K_f16", K_f16,
                "K_f17", K_f17,
                "K_f18", K_f18,
                "K_f19", K_f19,
                "K_f20", K_f20,
                "K_f21", K_f21,
                "K_f22", K_f22,
                "K_f23", K_f23,
                "K_f24", K_f24,
                "K_f25", K_f25,
                "K_kp_0", K_kp_0,
                "K_kp_1", K_kp_1,
                "K_kp_2", K_kp_2,
                "K_kp_3", K_kp_3,
                "K_kp_4", K_kp_4,
                "K_kp_5", K_kp_5,
                "K_kp_6", K_kp_6,
                "K_kp_7", K_kp_7,
                "K_kp_8", K_kp_8,
                "K_kp_9", K_kp_9,
                "K_kp_decimal", K_kp_decimal,
                "K_kp_divide", K_kp_divide,
                "K_kp_multiply", K_kp_multiply,
                "K_kp_subtract", K_kp_subtract,
                "K_kp_add", K_kp_add,
                "K_kp_enter", K_kp_enter,
                "K_kp_equal", K_kp_equal,
                "K_left_shift", K_left_shift,
                "K_left_control", K_left_control,
                "K_left_alt", K_left_alt,
                "K_left_super", K_left_super,
                "K_right_shift", K_right_shift,
                "K_right_control", K_right_control,
                "K_right_alt", K_right_alt,
                "K_right_super", K_right_super,
                "K_menu", K_menu,
                "K_last", K_last
            );

            lua.new_enum("MouseButton",
                "M_0", M_0,
                "M_left", M_left,
                "M_1", M_1,
                "M_right", M_right,
                "M_2", M_2,
                "M_middle", M_middle,
                "M_3", M_3,
                "M_4", M_4,
                "M_5", M_5,
                "M_6", M_6,
                "M_7", M_7
            );

            sol::table it = lua.create_table();

            it["setStickyKeys"] = Input::setStickyKeys;
            it["getKey"] = Input::getKey;
            it["getKeyUp"] = Input::getKeyUp;
            it["getKeyDown"] = Input::getKeyDown;

            it["getAxis"] = Input::getAxis;
            it["getHorizontal"] = Input::getHorizontal;
            it["getVertical"] = Input::getVertical;

            it["getMouseButtonDown"] = Input::mouseButtonDown;
            it["getMouseButtonUp"] = Input::mouseButtonUp;

            it["getMousePosition"] = Input::mousePosition;
            it["getMousePositionX"] = Input::mousePositionX;
            it["getMousePositionY"] = Input::mousePositionY;

            it["getMouseScroll"] = Input::mouseScroll;
            it["getMouseScrollX"] = Input::mouseScrollX;
            it["getMouseScrollY"] = Input::mouseScrollY;

            lua["Input"] = it;
        }
    }
}

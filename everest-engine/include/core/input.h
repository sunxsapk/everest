/*
 * ============== input ===============
 * Author: Sunil Sapkota
 * Description: Input manager is the most important thing an engine needs. It
 * provides static functionality for getting input-states like: mouse, keyboard,
 * etc. It provides an abstraction to GLFW's input system.
 */

#pragma once
#include "keycode.h"
#include "math/vec.h"
#include "events/mouseevent.h"

namespace Everest {
    class Input {
        public:
            static void init();

            static void setStickyKeys(bool mode);
            static bool keyDown(Keycode keycode);
            static bool keyUp(Keycode keycode);
            static bool keyRepeat(Keycode keycode);

            static bool mouseButtonDown(MouseButton button);
            static bool mouseButtonUp(MouseButton button);

            static ivec2 mousePosition();
            static i32 mousePositionX();
            static i32 mousePositionY();

            static inline vec2 mouseScroll(){return s_scroll;}
            static inline f32 mouseScrollX(){return s_scroll.x;}
            static inline f32 mouseScrollY(){return s_scroll.y;}

            static bool _scrollPoll(MouseScrolledEvent& event);
            static void _clearPoll();
        private:
            static GLFWwindow* s_window;
            static vec2 s_scroll;
    };
}


/*
 * ============== input ===============
 * Author: Sunil Sapkota
 * Description: Input manager is the most important thing an engine needs. It
 * provides static functionality for getting input-states like: mouse, keyboard,
 * etc. It provides an abstraction to GLFW's input system.
 */

#pragma once
#include "pch.h"
#include "keycode.h"
#include "events/mouseevent.h"

namespace Everest {
    class Input {
        public:
            /*-------NOT TO BE USED OUTSIDE ENGINE--------
             * initializes and binds input system with current window*/
            static void init();

            /*sets key-input mode to sticky*/
            static void setStickyKeys(bool mode);
            /*returns true if the key is currently pressed down*/
            static bool getKeyDown(Keycode keycode);
            /*returns true if the pressed key is released*/
            static bool getKeyUp(Keycode keycode);
            /*returns true if the key is held down*/
            static bool getKeyRepeat(Keycode keycode);

            /*returns true if the mouse button is pressed*/
            static bool mouseButtonDown(MouseButton button);
            /*returns true if the mouse button is released*/
            static bool mouseButtonUp(MouseButton button);

            /*returns the current position of the cursor on the screen*/
            static ivec2 mousePosition();
            /*returns the current x-position of the cursor*/
            static i32 mousePositionX();
            /*returns the current y-position of the cursor*/
            static i32 mousePositionY();

            /*returns the mouse scroll direction*/
            static inline vec2 mouseScroll(){return s_scroll;}
            /*returns the mouse scroll direction in x-axis*/
            static inline f32 mouseScrollX(){return s_scroll.x;}
            /*returns the mouse scroll direction in y-axis*/
            static inline f32 mouseScrollY(){return s_scroll.y;}

            static bool _scrollPoll(MouseScrolledEvent& event);
            static void _clearPoll();
        private:
            static GLFWwindow* s_window;
            static vec2 s_scroll;
    };
}


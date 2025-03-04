/*
 * ============== input ===============
 * Author: Sunil Sapkota
 * Description: Input manager is the most important thing an engine needs. It
 * provides static functionality for getting input-states like: mouse, keyboard,
 * etc. It provides an abstraction to GLFW's input system.
 */

#pragma once
#include "keycode.h"
#include "events/mouseevent.h"
#include "core/application.h"

namespace Everest {
    struct InputAxisKeys {
        static Keycode horizontal_p, horizontal_n;
        static Keycode vertical_p, vertical_n;

        static Keycode horizontal_ap, horizontal_an;
        static Keycode vertical_ap, vertical_an;
    };

    class Input {
        public:
            /*sets key-input mode to sticky*/
            static inline void setStickyKeys(bool mode){
                glfwSetInputMode(s_window, GLFW_STICKY_KEYS, mode); }
            /*returns true if the key is currently pressed down*/
            static inline bool getKeyDown(Keycode keycode){
                bool s = !s_keyStates.contains(keycode) && glfwGetKey(s_window, keycode) == GLFW_PRESS;
                if(s) s_keyStates.insert(keycode);
                return s;
            }
            /*returns true if the pressed key is released*/
            static inline bool getKeyUp(Keycode keycode){
                return glfwGetKey(s_window, keycode) == GLFW_RELEASE;}
            /*returns true if the key is held down*/
            static inline bool getKey(Keycode keycode){
                return glfwGetKey(s_window, keycode) == GLFW_PRESS;}
            
            /*returns the input axis values*/
            static inline vec2 getAxis(){return s_axis;}
            /*returns the horizontal axis input*/
            static inline f32 getHorizontal(){return s_axis.x;}
            /*returns the vertical axis input*/
            static inline f32 getVertical(){return s_axis.y;}

            /*returns true if the mouse button is pressed*/
            static bool mouseButtonDown(MouseButton button){
                return glfwGetMouseButton(s_window, button) == GLFW_PRESS; }
            /*returns true if the mouse button is released*/
            static bool mouseButtonUp(MouseButton button){
                return glfwGetMouseButton(s_window, button) == GLFW_RELEASE; }

            /*returns the current position of the cursor on the screen*/
            static inline vec2 mousePosition(){return s_mousePosition;}
            /*returns the current x-position of the cursor*/
            static inline f32 mousePositionX(){return s_mousePosition.x;}
            /*returns the current y-position of the cursor*/
            static inline f32 mousePositionY(){return s_mousePosition.y;}

            /*returns the mouse scroll direction*/
            static inline vec2 mouseScroll(){return s_scroll;}
            /*returns the mouse scroll direction in x-axis*/
            static inline f32 mouseScrollX(){return s_scroll.x;}
            /*returns the mouse scroll direction in y-axis*/
            static inline f32 mouseScrollY(){return s_scroll.y;}

        private:
            static std::set<Keycode> s_keyStates, s_tempKeyStates;
            static GLFWwindow* s_window;
            static vec2 s_axis;
            static vec2 s_mousePosition;
            static vec2 s_scroll;

            /*-------NOT TO BE USED OUTSIDE ENGINE--------
             * initializes and binds input system with current window*/
            static inline void init(){
                s_window = Application::getAppWindow().getWindow();
                ASSERT(s_window != NULL, "No window set for input");
                setStickyKeys(false);
            }

            static void update();

            static inline void clearScrollPoll(){ s_scroll = {0,0};}
            static inline bool _scrollPoll(MouseScrolledEvent& event){
                s_scroll += event.getDel();
                return false;
            }

            friend class Application;
            friend class Core;
    };
}


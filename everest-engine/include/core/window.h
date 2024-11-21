/*
 * ======== window class =========
 * Author: Sunil Sapkota
 * Description: handles creation, manage and deletion of window
 */

#pragma once
#include "events/event.h"
#include "pch.h"

namespace Everest {

    class Window {
        using EventCallback = std::function<void(Event&)>;

        public:
            /*creates a window instance with given params*/
            Window(i32 width, i32 height, std::string title, bool transparent = false);
            ~Window();

            /*swaps the front and back buffers*/
            void update();

            /*sets the title of the window to the one provided*/
            void setTitle(const char* title);
            /*set the window's event callback*/
            inline void setEventCallback(EventCallback callback){
                this->_winData.eventCallback = callback;
            }

            /*returns the vector containing window's width and height*/
            inline ivec2 getSize(){return this->_winData.size;}
            /*returns the width of the window*/
            inline i32 getWidth(){return this->_winData.size.x;}
            /*returns the height of the window*/
            inline i32 getHeight(){return this->_winData.size.y;}
            /*returns the pointer to current GLFWwindow*/
            inline GLFWwindow* getWindow(){ return this->_window;}

        protected:
            struct WindowData {
                std::string title ="";
                ivec2 position, size;

                EventCallback eventCallback;
            } _winData;
            GLFWwindow* _window;

            static void onResize(GLFWwindow* window, i32 width, i32 height);
            static void onMove(GLFWwindow* window, i32 x, i32 y);
            static void onClose(GLFWwindow *window);
            static void onMouseBtn(GLFWwindow* window, i32 button, i32 action,
                    i32 mods);
            static void onMouseMove(GLFWwindow* window, f64 x, f64 y);
            static void onScroll(GLFWwindow* window, f64 dx, f64 dy);
            static void onKeyEvent(GLFWwindow *window, i32 key, i32 scancode,
                    i32 action, i32 mods);
            static void onError(i32 errcode, const char *err);
    };
}


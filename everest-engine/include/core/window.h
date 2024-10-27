/*
 * ======== window class =========
 * Author: Sunil Sapkota
 * Description: handles creation, manage and deletion of window
 */

#pragma once
#include "math/vec.h"
#include "events/event.h"

namespace Everest {

    class Window {
        using EventCallback = std::function<void(Event&)>;

        public:
            Window(i32 width, i32 height, std::string title);
            ~Window();

            void convertFullScreen();
            void convertWindowedMode();

            void clear(f32 r=0.f,f32 g=0.f,f32 b=0.f, f32 a=1.0f);
            void update();

            void setTitle(const char* title);
            inline void setEventCallback(EventCallback callback){
                this->_winData.eventCallback = callback;
            }

            inline ivec2 getSize(){return this->_winData.size;}
            inline i32 getWidth(){return this->_winData.size.x;}
            inline i32 getHeight(){return this->_winData.size.y;}

        protected:
            struct WindowData {
                std::string title ="";
                ivec2 position, size;

                EventCallback eventCallback;
            } _winData;
            GLFWwindow* _window;

            GLFWmonitor* getCurrentMonitor();

            static void onFBResize(GLFWwindow* window, i32 width, i32 height);
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


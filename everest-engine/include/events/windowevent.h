/*
 * ===== window events =======
 * Author : Sunil Sapkota
 * Description : API for window events like resize, close and move
 */

#pragma once

#include "event.h"
#include "math/vec.h"

namespace Everest {
    class WindowCloseEvent : Event {
        public:
            EventClassType(WindowClose)
            EventClassCategory(Event_Window)
    };

    class WindowResizeEvent : Event {
        public:
            WindowResizeEvent(i32 width, i32 height);

            inline i32 getWidth(){return this->_size.x;}
            inline i32 getHeight(){return this->_size.y;}
            inline ivec2 getSize(){return this->_size;}

            EventClassType(WindowResize)
            EventClassCategory(Event_Window)
        protected:
            ivec2 _size;
    };

    class WindowMovedEvent : Event {
        public:
            WindowMovedEvent(int x, int y);

            inline i32 getPositionX(){return this->_position.x;}
            inline i32 getPositionY(){return this->_position.y;}
            inline ivec2 getPosition(){return this->_position;}

            EventClassType(WindowMoved)
            EventClassCategory(Event_Window)
        protected:
            ivec2 _position;
    };
}

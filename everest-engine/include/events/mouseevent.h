/*
 * ===== Event =====
 * Author : Sunil Sapkota
 * Description : Mouse Events API: move, scroll, button down & button up
 */

#pragma once

#include "event.h"
#include "math/vec.h"

namespace Everest {
    class MouseMovedEvent : Event {
        public:
            MouseMovedEvent(i32 x, i32 y);

            inline i32 getPositionX(){return this->_position.x;}
            inline i32 getPositionY(){return this->_position.y;}
            inline ivec2 getPosition(){return this->_position;}

            EventClassType(MouseMoved)
            EventClassCategory(Event_Mouse | Event_Input)
        private:
            ivec2 _position;
    };

    class MouseScrolledEvent : Event {
        public:
            MouseScrolledEvent(f32 delX, f32 delY);

            inline f32 getDelX(){return this->_del.x;}
            inline f32 getDelY(){return this->_del.y;}
            inline vec2 getDel(){return this->_del;}

            EventClassType(MouseScrolled)
            EventClassCategory(Event_Mouse | Event_Input)
        private:
            vec2 _del;
    };

    class MouseButtonEvent : Event {
        public:
            inline i32 getButton(){return this->_button;}
            EventClassCategory(Event_Mouse | Event_Input)
        protected:
            MouseButtonEvent(i32 btn);
            i32 _button;
    };

    class MouseButtonDownEvent : MouseButtonEvent {
        public:
            MouseButtonDownEvent(i32 btn);
            EventClassType(MouseButtonDown)
    };

    class MouseButtonUpEvent : MouseButtonEvent {
        public:
            MouseButtonUpEvent(i32 btn);
            EventClassType(MouseButtonUp)
    };
}

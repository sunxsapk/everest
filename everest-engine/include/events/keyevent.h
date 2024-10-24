/*
 * ======= KeyEvent =======
 * Author : Sunil Sapkota
 * Description : KeyEvents implementation : KeyDown, KeyUp
 */

#pragma once

#include "event.h"

namespace Everest {
    class KeyEvent : Event {
        public:
            inline i32 getKey(){return this->_keyCode;}
            EventClassCategory(Event_Keyboard | Event_Input)

        protected:
            KeyEvent(i32 keyCode);
            i32 _keyCode = 0;
    };

    class KeyDownEvent : KeyEvent {
        public:
            KeyDownEvent(i32 keyCode);
            EventClassType(KeyDown)
    };

    class KeyUpEvent : KeyEvent {
        public:
            KeyUpEvent(i32 keyCode);
            EventClassType(KeyUp)
    };
}

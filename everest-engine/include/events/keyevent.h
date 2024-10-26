/*
 * ======= KeyEvent =======
 * Author : Sunil Sapkota
 * Description : KeyEvents implementation : KeyDown, KeyUp
 */

#pragma once

#include "event.h"

namespace Everest {
    class KeyEvent :  public Event {
        public:
            inline i32 getKey(){return this->_keyCode;}
            virtual std::string toString() const override;
            EventClassCategory(Event_Keyboard | Event_Input)

        protected:
            KeyEvent(i32 keyCode);
            i32 _keyCode = 0;
    };

    class KeyDownEvent :  public KeyEvent {
        public:
            KeyDownEvent(i32 keyCode);
            EventClassType(KeyDown)
    };

    class KeyRepeatEvent :  public KeyEvent {
        public:
            KeyRepeatEvent(i32 keyCode);
            EventClassType(KeyRepeat)
    };

    class KeyUpEvent :  public KeyEvent {
        public:
            KeyUpEvent(i32 keyCode);
            EventClassType(KeyUp)
    };
}

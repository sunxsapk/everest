#include "events/keyevent.h"

namespace Everest {
    KeyEvent::KeyEvent(i32 keyCode)
        : _keyCode(keyCode) { }

    KeyDownEvent::KeyDownEvent(i32 keyCode)
        : KeyEvent(keyCode) { }

    std::string KeyEvent::toString() const {
        std::ostringstream oss;
        oss << this->getName() << " key: " << this->_keyCode;
        return  oss.str();
    }

    KeyRepeatEvent::KeyRepeatEvent(i32 keyCode)
        : KeyEvent(keyCode) { }

    KeyUpEvent::KeyUpEvent(i32 keyCode)
        : KeyEvent(keyCode) { }
}

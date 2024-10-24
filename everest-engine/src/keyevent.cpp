#include "events/keyevent.h"

Everest::KeyEvent::KeyEvent(i32 keyCode)
    : _keyCode(keyCode) { }

Everest::KeyDownEvent::KeyDownEvent(i32 keyCode)
    : Everest::KeyEvent(keyCode) { }

Everest::KeyUpEvent::KeyUpEvent(i32 keyCode)
    : Everest::KeyEvent(keyCode) { }


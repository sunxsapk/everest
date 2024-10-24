#include "events/mouseevent.h"

namespace Everest{
    MouseMovedEvent :: MouseMovedEvent(i32 x, i32 y)
        : _position{x, y} { }

    MouseScrolledEvent :: MouseScrolledEvent(f32 delX, f32 delY)
        : _del{delX, delY} { }

    MouseButtonEvent :: MouseButtonEvent(i32 btn)
        : _button(btn) { }

    MouseButtonDownEvent :: MouseButtonDownEvent(i32 btn)
        : MouseButtonEvent(btn) { }

    MouseButtonUpEvent :: MouseButtonUpEvent(i32 btn)
        : MouseButtonEvent(btn) { }
}

#include "events/windowevent.h"

namespace Everest{

    WindowResizeEvent :: WindowResizeEvent(i32 width, i32 height)
        : _size{width, height}{ }

    WindowMovedEvent :: WindowMovedEvent(int x, int y)
        : _position{x, y}{ }
}

#include "events/mouseevent.h"

namespace Everest{
    MouseMovedEvent :: MouseMovedEvent(i32 x, i32 y)
        : _position{x, y} { }

    std::string MouseMovedEvent :: toString() const{
        std::ostringstream oss;
        oss << this->getName() << " x:" << this->_position.x
            << ", y:" << this->_position.y;
        return  oss.str();
    }

    MouseScrolledEvent :: MouseScrolledEvent(f32 delX, f32 delY)
        : _del{delX, delY} { }

    std::string MouseScrolledEvent :: toString() const{
        std::ostringstream oss;
        oss << this->getName() << " x:" << this->_del.x
            << ", y:" << this->_del.y;
        return  oss.str();
    }

    MouseButtonEvent :: MouseButtonEvent(i32 btn)
        : _button(btn) { }

    std::string MouseButtonEvent :: toString() const{
        std::ostringstream oss;
        oss << this->getName() << " button:" << this->_button;
        return  oss.str();
    }

    MouseButtonDownEvent :: MouseButtonDownEvent(i32 btn)
        : MouseButtonEvent(btn) { }

    MouseButtonUpEvent :: MouseButtonUpEvent(i32 btn)
        : MouseButtonEvent(btn) { }
}

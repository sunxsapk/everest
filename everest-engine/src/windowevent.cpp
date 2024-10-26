#include "events/windowevent.h"

namespace Everest{

    WindowResizeEvent :: WindowResizeEvent(i32 width, i32 height)
        : _size{width, height}{ }

    std::string WindowResizeEvent :: toString() const{
        std::ostringstream oss;
        oss << this->getName() << " width:" << this->_size.x
            << ", height:" << this->_size.y;
        return  oss.str();
    }

    WindowMovedEvent :: WindowMovedEvent(int x, int y)
        : _position{x, y}{ }

    std::string WindowMovedEvent :: toString() const{
        std::ostringstream oss;
        oss << this->getName() << " x:" << this->_position.x
            << ", y:" << this->_position.y;
        return  oss.str();
    }

}

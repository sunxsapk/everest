/*
 * ========== Event Base Class ==========
 * Author : Sunil Sapkota
 * Description : event-types, event-list, base class, etc for 
 * everest engine's event system.
 */
#pragma once

#include "math/utils.h"

namespace Everest {
    #define BIND_EVENT_CB(fn) [this](auto&&... args) -> decltype(auto) {\
        return this->fn(std::forward<decltype(args)>(args)...);\
    }

    enum class EventType{
        None=0,
        WindowClose, WindowResize, WindowMoved,
        KeyDown, KeyRepeat, KeyUp,
        MouseButtonDown, MouseButtonUp,
        MouseMoved, MouseScrolled
    };

    enum EventCategory {
        None                = 0,
        Event_Window        = BIT(1),
        Event_Input         = BIT(2),
        Event_Keyboard      = BIT(3),
        Event_Mouse         = BIT(4),
    };

#define EventClassType(x) \
    static EventType getStaticType(){return EventType::x;}\
    virtual EventType getEventType() const override { return getStaticType();}\
    virtual const char* getName() const override {return #x;}
#define EventClassCategory(x) virtual int getCategoryFlags() const override {return x;}

    class Event {
        public:
            virtual EventType getEventType() const = 0;
            virtual const char* getName() const = 0;
            virtual int getCategoryFlags() const = 0;

            virtual std::string toString() const;
            inline bool isInCategory(EventCategory category){
                return getCategoryFlags() & category;
            }

        protected:
            bool _handled = false;
    };
}

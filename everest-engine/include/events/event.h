/*
 * ========== Enhanced Event System ==========
 * Original Author: Sunil Sapkota
 * Enhanced by: Grok 3 (xAI)
 * Description: Advanced event system with type safety, priority handling,
 * event queuing, and serialization support for Everest Engine
 */
#pragma once

#include <functional>
#include <string>
#include <sstream>
#include <type_traits>
#include <cstdint>
#include <chrono>
#include <memory>
#include "math/utils.h"
#include "core/utils.h"

namespace Everest {
    // Macro for binding member functions to event callbacks
    #define BIND_EVENT_CB(fn) [this](auto&&... args) -> decltype(auto) {\
        return this->fn(std::forward<decltype(args)>(args)...);\
    }

    // Event priority levels
    enum class EventPriority : uint8_t {
        Immediate = 0,
        High = 1,
        Normal = 2,
        Low = 3
    };

    // Comprehensive event type enumeration
    enum class EventType : uint32_t {
        None = 0,
        // Window Events
        WindowClose, WindowResize, WindowMoved, WindowFocus, WindowLostFocus,
        // Keyboard Events
        KeyDown, KeyRepeat, KeyUp, KeyTyped,
        // Mouse Events
        MouseButtonDown, MouseButtonUp, MouseMoved, MouseScrolled, MouseEntered, MouseLeft,
        // Application Events
        AppTick, AppUpdate, AppRender
    };

    // Bit-flag based event categories
    enum EventCategory : uint32_t {
        CategoryNone        = 0,
        CategoryWindow      = BIT(0),
        CategoryInput       = BIT(1),
        CategoryKeyboard    = BIT(2),
        CategoryMouse       = BIT(3),
        CategoryApplication = BIT(4)
    };

    // Event class macros
    #define EVENT_CLASS_TYPE(type) \
        static EventType GetStaticType() { return EventType::type; } \
        virtual EventType GetEventType() const override { return GetStaticType(); } \
        virtual const char* GetName() const override { return #type; }

    #define EVENT_CLASS_CATEGORY(category) \
        virtual int GetCategoryFlags() const override { return category; }

    // Base Event class with enhanced features
    class Event {
    public:
        virtual ~Event() = default;

        bool Handled() const { return m_Handled; }
        void SetHandled(bool handled) { m_Handled = handled; }

        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const {
            return GetName();
        }

        bool IsInCategory(EventCategory category) const {
            return GetCategoryFlags() & category;
        }

        EventPriority GetPriority() const { return m_Priority; }
        auto GetTimestamp() const { return m_Timestamp; }

    protected:
        bool m_Handled = false;
        EventPriority m_Priority = EventPriority::Normal;
        std::chrono::system_clock::time_point m_Timestamp = std::chrono::system_clock::now();
    };

    // Event Dispatcher with type checking and priority handling
    class EventDispatcher {
    public:
        using EventCallback = std::function<bool(Event&)>;

        explicit EventDispatcher(std::shared_ptr<Event> event)
            : m_Event(std::move(event)) {}

        template<typename T, typename F>
        bool Dispatch(const F& func) {
            static_assert(std::is_base_of<Event, T>::value, "T must derive from Event");
            
            if (m_Event->GetEventType() == T::GetStaticType()) {
                m_Event->SetHandled(func(static_cast<T&>(*m_Event)));
                return true;
            }
            return false;
        }

        void SetCallback(EventCallback callback) { m_Callback = std::move(callback); }
        void Dispatch() {
            if (m_Callback) {
                m_Event->SetHandled(m_Callback(*m_Event));
            }
        }

    private:
        std::shared_ptr<Event> m_Event;
        EventCallback m_Callback;
    };

    // Event Queue utility class
    class EventQueue {
    public:
        void Push(std::shared_ptr<Event> event) {
            m_Queue.push_back(std::move(event));
            Sort();
        }

        std::shared_ptr<Event> Pop() {
            if (m_Queue.empty()) return nullptr;
            auto event = m_Queue.front();
            m_Queue.erase(m_Queue.begin());
            return event;
        }

        void Clear() { m_Queue.clear(); }
        bool Empty() const { return m_Queue.empty(); }
        size_t Size() const { return m_Queue.size(); }

    private:
        void Sort() {
            std::sort(m_Queue.begin(), m_Queue.end(),
                [](const auto& a, const auto& b) {
                    return static_cast<uint8_t>(a->GetPriority()) <
                           static_cast<uint8_t>(b->GetPriority());
                });
        }

        std::vector<std::shared_ptr<Event>> m_Queue;
    };

    // Utility function to convert event to string
    inline std::ostream& operator<<(std::ostream& os, const Event& e) {
        return os << e.ToString();
    }
}

// Example usage:
/*
class WindowResizeEvent : public Everest::Event {
public:
    WindowResizeEvent(int w, int h) : width(w), height(h) {}
    
    EVENT_CLASS_TYPE(WindowResize)
    EVENT_CLASS_CATEGORY(Everest::CategoryWindow)
    
    std::string ToString() const override {
        std::stringstream ss;
        ss << "WindowResizeEvent: " << width << ", " << height;
        return ss.str();
    }

private:
    int width, height;
};
*/

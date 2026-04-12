#pragma once
#include <string>

#include "Core.h"

namespace Leaf {
enum class EventType {
    None = 0,
    WindowClose,
    WindowResize,
    WindowFocus,
    WindowLostFocus,
    WindowMoved,
    AppTick,
    AppUpdate,
    AppRender,
    KeyPressed,
    KeyReleased,
    KeyTyped,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseScrolled
};

enum EventCategory {
    None = 0,
    EventCategoryApplication = BIT(0),
    EventCategoryInput = BIT(1),
    EventCategoryKeyboard = BIT(2),
    EventCategoryMouse = BIT(3),
    EventCategoryMouseButton = BIT(4)
};

#define EVENT_CLASS_TYPE(type)                                                 \
    static EventType StaticGetEventType() {                                    \
        return EventType::type;                                                \
    }                                                                          \
    virtual EventType GetEventType() const override {                          \
        return StaticGetEventType();                                           \
    }                                                                          \
    virtual const char *GetName() const override {                             \
        return #type;                                                          \
    }

#define EVENT_CLASS_CATEGORY(category)                                         \
    virtual int GetCategoryFlags() const override {                            \
        return category;                                                       \
    }

class Event {
    friend class EventDispatcher;

public:
    virtual EventType GetEventType() const = 0;
    virtual char const *GetName() const = 0;
    virtual int GetCategoryFlags() const = 0;
    virtual std::string ToString() const {
        return GetName();
    }

    virtual bool IsHandled() const {
        return mHandled;
    }

    inline bool IsInCategory(EventCategory category) {
        return GetCategoryFlags() & category;
    }

protected:
    bool mHandled {false};
};

class EventDispatcher {
public:
    explicit EventDispatcher(Event &event) : mEvent(event) {
    }
    template<typename T, typename F>
    bool Dispatch(const F &func) {
        if (mEvent.GetEventType() == T::StaticGetEventType()) {
            mEvent.mHandled = func(static_cast<T &>(mEvent));
            return true;
        }
        return false;
    }

private:
    Event &mEvent;
};
inline std::ostream &operator<<(std::ostream &os, const Event &e) {
    return os << e.ToString();
}

}  // namespace Leaf
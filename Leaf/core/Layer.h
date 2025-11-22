#pragma once
#include "events/Event.h"

namespace Leaf {
class LEAF_API Layer {
public:
    Layer(const std::string &name = "Layer") : mDebugName(name) {
    }
    virtual ~Layer();
    virtual void OnAttach() {
    }
    virtual void OnDetach() {
    }
    virtual void OnUpdate() {
    }
    virtual void OnImGuiRender() {
    }
    virtual void OnEvent(Event &e) {
    }
    inline const std::string &GetName() const {
        return mDebugName;
    }

protected:
    std::string mDebugName;
};
}  // namespace Leaf

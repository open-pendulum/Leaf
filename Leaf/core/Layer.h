#pragma once
#include "Timestep.h"
#include "events/Event.h"

namespace Leaf {
class Layer {
public:
    explicit Layer(const std::string &name = "Layer") : mDebugName(name) {
    }
    virtual ~Layer();
    virtual void OnAttach() {
    }
    virtual void OnDetach() {
    }
    virtual void OnUpdate(Timestep ts) {
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

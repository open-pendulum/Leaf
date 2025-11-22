#pragma once

#include "Layer.h"
#include "events/ApplicationEvent.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

namespace Leaf {
class LEAF_API ImGuiLayer : public Layer {
public:
    ImGuiLayer();
    ~ImGuiLayer();
    void OnAttach() override;
    void OnDetach() override;

    void OnImGuiRender() override;
    void Begin();
    void End();

private:
    float mTime = 0.0f;
};

}  // namespace Leaf

#pragma once
#include "Input.h"
#include "WindowsWindow.h"

namespace Leaf {
class WindowsInput : public Input {
public:
    WindowsInput();
    WindowsInput(WindowsWindow *window);
    virtual ~WindowsInput() override;
    bool IsKeyPressedImpl(int keycode) override;
    bool IsMouseButtonPressedImpl(int button) override;
    std::pair<float, float> GetMousePositionImpl() override;
    float GetMouseXImpl() override;
    float GetMouseYImpl() override;
};
}  // namespace Leaf

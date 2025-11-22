#pragma once

#include <glad/glad.h>

#include "Window.h"
class GLFWwindow;
namespace Leaf {
class GraphicsContext;

class WindowsWindow : public Window {
public:
    WindowsWindow(const WindowProps &props);
    virtual ~WindowsWindow() override;
    void OnUpdate() override;
    inline unsigned int GetWidth() const override {
        return mData.Width;
    }
    inline unsigned int GetHeight() const override {
        return mData.Height;
    }

    inline void SetEventCallback(const EventCallbackFn &callback) override;
    void SetVSync(bool enabled) override;
    bool IsVSync() const override;

    virtual void *GetNativeWindow() const override {
        return mWindow;
    }

private:
    virtual void Init(const WindowProps &props);
    virtual void Shutdown();

private:
    GLFWwindow *mWindow;
    GraphicsContext *mContext;
    struct WindowData {
        std::string Title;
        unsigned int Width, Height;
        bool VSync;
        EventCallbackFn EventCallback;
    };
    WindowData mData;
};
}  // namespace Leaf
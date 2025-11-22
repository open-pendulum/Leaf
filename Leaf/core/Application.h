#pragma once
#include "Core.h"
#include "LayerStack.h"
#include "Window.h"
#include "events/ApplicationEvent.h"
#include "imgui/ImGuiLayer.h"
#include "renderer/Buffer.h"
#include "renderer/Shader.h"
#include "renderer/VertexArray.h"

namespace Leaf {
class LEAF_API Application {
public:
    Application();
    virtual ~Application();
    void Run();
    void OnEvent(Event &e);
    void PushLayer(Layer *layer);
    void PushOverlay(Layer *overlay);
    inline Window &GetWindow() {
        return *mWindow;
    }
    inline static Application &Get() {
        return *sInstance;
    }

private:
    bool OnWindowClose(WindowCloseEvent &e);
    std::unique_ptr<Window> mWindow;
    ImGuiLayer *mImGuiLayer;
    bool mRunning = true;
    LayerStack mLayerStack;
    std::unique_ptr<Shader> mShader;
    std::shared_ptr<VertexArray> mVertexArray;

private:
    static Application *sInstance;
};

Application *CreateApplication();
}  // namespace Leaf

//
// Created by Saber on 2025/2/27.
//

#include "Application.h"

#include <GLFW/glfw3.h>

#include "Input.h"
#include "Window.h"
#include "events/ApplicationEvent.h"
#include "renderer/Renderer.h"

namespace Leaf {

// 静态单例指针初始化
Application *Application::sInstance = nullptr;

Application::Application() {
    LEAF_CORE_ASSERT(!sInstance, "Application already exists!")

    // 创建底层窗口（平台相关封装在 Window::Create 内）
    mWindow = Window::Create();
    // 将 Application::OnEvent 注册为窗口事件回调入口
    mWindow->SetEventCallback(LEAF_BIND_EVENT_FN(Application::OnEvent));

    // 设置全局 Application 单例
    sInstance = this;

    Renderer::Init();
    // 创建并压入 ImGui 层作为 overlay，这样所有 Layer 都可以在其上绘制 ImGui
    mImGuiLayer = new ImGuiLayer();
    PushOverlay(mImGuiLayer);
}

Application::~Application() {
    Renderer::Shutdown();
}

void Application::OnEvent(Event &e) {
    // 先使用 EventDispatcher 做事件类型分发，例如窗口关闭
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(
        LEAF_BIND_EVENT_FN(Application::OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(
        LEAF_BIND_EVENT_FN(Application::OnWindowResize));

    //    LEAF_CORE_TRACE("{0}", e.ToString());

    for (auto it = mLayerStack.end(); it != mLayerStack.begin();) {
        (*--it)->OnEvent(e);
        if (e.IsHandled()) {
            break;
        }
    }
}

void Application::Run() {
    // 用一个简单的计数器来驱动相机旋转，便于观察 View 矩阵生效的效果
    static int i = 0;
    while (mRunning) {
        // 1. 渲染前的清屏

        float time = (float)glfwGetTime();
        Timestep timestep = time - mLastFrameTime;
        mLastFrameTime = time;
        // 4. 更新所有 Layer 的逻辑
        if (!mIsMinimized) {
            for (Layer *layer : mLayerStack) {
                layer->OnUpdate(timestep);
            }
        }

        // 5. ImGui 渲染阶段：先 Begin，再让每个 Layer 画自己的 ImGui，再 End
        mImGuiLayer->Begin();
        for (Layer *layer : mLayerStack) {
            layer->OnImGuiRender();
        }

        mImGuiLayer->End();

        // 6. 让窗口执行平台相关的更新逻辑（事件轮询 + 缓冲区交换等）
        mWindow->OnUpdate();
    }
}

bool Application::OnWindowClose(WindowCloseEvent &e) {
    mRunning = false;
    return true;
}

bool Application::OnWindowResize(WindowResizeEvent &e) {
    if (e.GetWidth() == 0 || e.GetHeight() == 0) {
        LEAF_INFO("Window resized to 0x0, minimizing...");
        mIsMinimized = true;
        return false;
    }
    mIsMinimized = false;
    Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
    return true;
}
void Application::PushLayer(Leaf::Layer *layer) {
    mLayerStack.PushLayer(layer);
    layer->OnAttach();
}
void Application::PushOverlay(Leaf::Layer *overlay) {
    mLayerStack.PushOverlay(overlay);
    overlay->OnAttach();
}

}  // namespace Leaf

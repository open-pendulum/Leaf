#pragma once
#include "Core.h"
#include "LayerStack.h"
#include "Window.h"
#include "events/ApplicationEvent.h"
#include "imgui/ImGuiLayer.h"
#include "renderer/Buffer.h"
#include "renderer/OrthographicCamera.h"
#include "renderer/Shader.h"
#include "renderer/VertexArray.h"

namespace Leaf {

// Application 是整个引擎运行时的“宿主应用”：
// - 负责创建窗口、驱动主循环（Run）
// - 维护 LayerStack（游戏/编辑器逻辑层）
// - 管理 ImGuiLayer 与渲染资源（Shader、VertexArray、Camera 等）
class LEAF_API Application {
public:
    // 创建应用实例：会初始化窗口、ImGui 层、基础渲染数据
    Application();
    virtual ~Application();

    // 主循环入口：
    // - 处理渲染（包括清屏、BeginScene/EndScene）
    // - 驱动所有 Layer 的 OnUpdate / OnImGuiRender
    // - 调用 Window::OnUpdate 进行事件轮询与缓冲区交换
    void Run();

    // 外部事件入口（Window 通过回调把事件传进来）：
    // 通过 EventDispatcher 分发给具体的处理函数，
    // 并从顶到底遍历 LayerStack 让每个 Layer 有机会响应事件。
    void OnEvent(Event &e);

    // 往栈中压入普通 Layer（游戏逻辑层），位于叠加层之下
    void PushLayer(Layer *layer);
    // 往栈中压入 Overlay（通常用于 UI/调试层），位于最上层
    void PushOverlay(Layer *overlay);
    // 获取窗口引用（例如用于查询尺寸/输入等）
    inline Window &GetWindow() {
        return *mWindow;
    }

    // Application 全局单例访问接口
    inline static Application &Get() {
        return *sInstance;
    }

private:
    // 窗口关闭事件处理：返回 true 表示事件已经被处理
    bool OnWindowClose(WindowCloseEvent &e);
    bool OnWindowResize(WindowResizeEvent &e);

private:
    // 底层平台窗口（当前由 WindowsWindow/OpenGL 实现）
    std::unique_ptr<Window> mWindow;
    // 全局 ImGui 层（作为 overlay 存在）
    ImGuiLayer *mImGuiLayer;
    // 主循环是否继续运行
    bool mRunning = true;
    // Layer 栈：包含普通层与 overlay
    LayerStack mLayerStack;
    float mLastFrameTime = 0.0f;

    bool mIsMinimized = false;

private:
    // Application 单例指针（在构造函数中赋值）
    static Application *sInstance;
};

Application *CreateApplication();
}  // namespace Leaf

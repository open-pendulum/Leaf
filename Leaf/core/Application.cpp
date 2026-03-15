//
// Created by Saber on 2025/2/27.
//

#include "Application.h"

#include <glad/glad.h>

#include "Input.h"
#include "Window.h"
#include "events/ApplicationEvent.h"
#include "renderer/Buffer.h"
#include "renderer/Renderer.h"

//#define LEAF_BIND_EVENT_FN(x)                                                  \
//    std::bind(&Application::x, this, std::placeholders::_1)

namespace Leaf {

// 静态单例指针初始化
Application *Application::sInstance = nullptr;

Application::Application()
    // 初始化一个固定大小的正交相机：
    // X 方向 [-1.6, 1.6]，Y 方向 [-0.9, 0.9]，大致对应 16:9 的视口比例。
    : mCamera(-1.6f, 1.6f, -0.9f, 0.9f) {
    LEAF_CORE_ASSERT(!sInstance, "Application already exists!")

    // 创建底层窗口（平台相关封装在 Window::Create 内）
    mWindow = std::unique_ptr<Window>(Window::Create());
    // 将 Application::OnEvent 注册为窗口事件回调入口
    mWindow->SetEventCallback(LEAF_BIND_EVENT_FN(Application::OnEvent));

    // 设置全局 Application 单例
    sInstance = this;

    // 创建并压入 ImGui 层作为 overlay，这样所有 Layer 都可以在其上绘制 ImGui
    mImGuiLayer = new ImGuiLayer();
    PushOverlay(mImGuiLayer);

    // ------------------------------
    // 以下为当前 Demo 的渲染数据初始化
    // ------------------------------

    // 顶点数组对象（封装了顶点缓冲、索引缓冲以及布局）
    mVertexArray.reset(VertexArray::Create());

    // 一个简单的矩形顶点（四个点），Z=0，使用正交相机绘制
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f,  -0.5f, 0.0f,
        0.5f,  0.5f,  0.0f,
        -0.5f, 0.5f,  0.0f,
    };
    std::shared_ptr<VertexBuffer> vertexBuffer;
    vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

    // 顶点布局：当前位置属性 aPos，三个 float 组成
    BufferLayout layout = {
        {ShaderDataType::Float3, "aPos"},
    };

    vertexBuffer->SetLayout(layout);
    mVertexArray->AddVertexBuffer(vertexBuffer);

    // 矩形索引（两个三角形拼成一个四边形）
    unsigned int indices[] = {0, 1, 2, 2, 3, 0};
    std::shared_ptr<IndexBuffer> indexBuffer;
    indexBuffer.reset(
        IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
    mVertexArray->SetIndexBuffer(indexBuffer);

    // 一个最简单的着色器：只接收一个投影矩阵并输出纯色片元
    std::string vertexSrc = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        uniform mat4 projectionMatrix;
        void main() {
            gl_Position = projectionMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
        }
    )";
    std::string fragmentSrc = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
        }
    )";
    // 当前实现中 Shader 的构造函数直接编译传入的 GLSL 源码
    mShader = std::unique_ptr<Shader>(new Shader(vertexSrc, fragmentSrc));
}

Application::~Application() {
}

void Application::OnEvent(Event &e) {
    // 先使用 EventDispatcher 做事件类型分发，例如窗口关闭
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(
        LEAF_BIND_EVENT_FN(Application::OnWindowClose));
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
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::Clear();

        // 2. 更新相机状态（位置 + 旋转），这里作为 Demo 写死
        mCamera.SetPosition({0.5f, 0.5f, 0.0f});
        mCamera.SetRotation((45 + i++) % 360);

        // 3. 开始渲染一个场景：
        //    - BeginScene 会把当前相机的 ViewProjection 矩阵缓存到 Renderer 内部
        //    - Submit 会使用该矩阵更新 Shader 的 uniform，然后提交绘制命令
        Renderer::BeginScene(mCamera);
        mShader->Bind();
        Renderer::Submit(mShader, mVertexArray);
        Renderer::EndScene();

        // 4. 更新所有 Layer 的逻辑
        for (Layer *layer : mLayerStack) {
            layer->OnUpdate();
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

void Application::PushLayer(Leaf::Layer *layer) {
    mLayerStack.PushLayer(layer);
    layer->OnAttach();
}
void Application::PushOverlay(Leaf::Layer *overlay) {
    mLayerStack.PushOverlay(overlay);
    overlay->OnAttach();
}

}  // namespace Leaf

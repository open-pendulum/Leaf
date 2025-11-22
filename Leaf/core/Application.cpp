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
Application *Application::sInstance = nullptr;

Application::Application() {
    LEAF_CORE_ASSERT(!sInstance, "Application already exists!")
    mWindow = std::unique_ptr<Window>(Window::Create());
    mWindow->SetEventCallback(LEAF_BIND_EVENT_FN(Application::OnEvent));
    sInstance = this;
    mImGuiLayer = new ImGuiLayer();
    PushOverlay(mImGuiLayer);

    mVertexArray.reset(VertexArray::Create());

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 0.5f,  -0.5f, 0.0f,
        0.5f,  0.5f,  0.0f, -0.5f, 0.5f,  0.0f,
    };
    std::shared_ptr<VertexBuffer> vertexBuffer;
    vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

    BufferLayout layout = {
        {ShaderDataType::Float3, "aPos"},
    };

    vertexBuffer->SetLayout(layout);
    mVertexArray->AddVertexBuffer(vertexBuffer);

    unsigned int indices[] = {0, 1, 2, 2, 3, 0};
    std::shared_ptr<IndexBuffer> indexBuffer;
    indexBuffer.reset(
        IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
    mVertexArray->SetIndexBuffer(indexBuffer);
    std::string vertexSrc = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        void main() {
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
        }
    )";
    std::string fragmentSrc = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
        }
    )";
    mShader = std::unique_ptr<Shader>(new Shader(vertexSrc, fragmentSrc));
}

Application::~Application() {
}

void Application::OnEvent(Event &e) {
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
    while (mRunning) {
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::Clear();
        Renderer::BeginScene();
        mShader->Bind();
        Renderer::Submit(mVertexArray);
        Renderer::EndScene();

        for (Layer *layer : mLayerStack) {
            layer->OnUpdate();
        }

        mImGuiLayer->Begin();
        for (Layer *layer : mLayerStack) {
            layer->OnImGuiRender();
        }

        mImGuiLayer->End();
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

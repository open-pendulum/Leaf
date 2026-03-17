#include <Leaf.h>
#include <imgui.h>

#include <glm/ext/matrix_clip_space.hpp>  // glm::perspective
#include <glm/gtc/matrix_transform.hpp>   // glm::translate, glm::rotate
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec4.hpp>  // glm::vec4
#include <memory>

#include "platform/opengl/OpenGLShader.h"
// ExampleLayer 作为本节教程的演示层：
// - 持有一个正交相机和一个简单的矩形网格
// - 通过键盘控制“相机”的平移与旋转，从而观察到屏幕上矩形的反向运动
class ExampleLayer : public Leaf::Layer {
public:
    ExampleLayer() :
        Layer("Example"),
        mCamera(-1.6f, 1.6f, -0.9f, 0.9f),
        mCameraPosition(0.0f) {
        // 顶点数组对象（封装了顶点缓冲、索引缓冲以及布局）
        mVertexArray.reset(Leaf::VertexArray::Create());

        // 一个简单的矩形顶点（四个点），Z=0，使用正交相机绘制
        float vertices[3 * 7] = {-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
                                 0.5f,  -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
                                 0.0f,  0.5f,  0.0f, 0.8f, 0.8f, 0.2f, 1.0f};
        std::shared_ptr<Leaf::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(
            Leaf::VertexBuffer::Create(vertices, sizeof(vertices)));

        // 顶点布局：当前位置属性 aPos，三个 float 组成
        Leaf::BufferLayout layout = {
            {Leaf::ShaderDataType::Float3, "a_Position"},
            {Leaf::ShaderDataType::Float4, "a_Color"},
        };

        vertexBuffer->SetLayout(layout);
        mVertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[3] = {0, 1, 2};
        std::shared_ptr<Leaf::IndexBuffer> indexBuffer;
        indexBuffer.reset(Leaf::IndexBuffer::Create(
            indices, sizeof(indices) / sizeof(uint32_t)));
        mVertexArray->SetIndexBuffer(indexBuffer);

        mSquareVertexArray.reset(Leaf::VertexArray::Create());
        float squareVertices[3 * 4] = {

            -0.5f, -0.5f, 0.0f, 0.5f,  -0.5f, 0.0f,
            0.5f,  0.5f,  0.0f, -0.5f, 0.5f,  0.0f};
        std::shared_ptr<Leaf::VertexBuffer> squareVB;
        squareVB.reset(
            Leaf::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout({
            {Leaf::ShaderDataType::Float3, "a_Position"},
        });
        mSquareVertexArray->AddVertexBuffer(squareVB);

        unsigned int squareIndices[6] = {0, 1, 2, 2, 3, 0};
        std::shared_ptr<Leaf::IndexBuffer> squareIndexBuffer;
        squareIndexBuffer.reset(Leaf::IndexBuffer::Create(
            squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        mSquareVertexArray->SetIndexBuffer(squareIndexBuffer);

        // 一个最简单的着色器：只接收一个投影矩阵（这里实际上传的是
        std::string vertexSrc = R"(
        #version 330 core

        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec4 a_Color;

        uniform mat4 u_ViewProjection;
        uniform mat4 u_Transform;

        out vec3 v_Position;
        out vec4 v_Color;

        void main() {
            v_Position = a_Position;
            v_Color = a_Color;
            gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
        }
    )";
        // ViewProjection） 并输出纯色片元，用来观察相机运动对最终图像的影响
        std::string fragmentSrc = R"(
        #version 330 core

        layout(location = 0) out vec4 color;

        in vec3 v_Position;
        in vec4 v_Color;

        void main()
        {
            color = vec4(v_Position * 0.5 + 0.5, 1.0);
            color = v_Color;
        }
    )";

        // 当前实现中 Shader 的构造函数直接编译传入的 GLSL 源码
        mShader.reset(Leaf::Shader::Create(vertexSrc, fragmentSrc));

        // 用于批量绘制小方块（网格）的纯色 Shader：
        // - uniform 命名需要与 Renderer::Submit 里上传的名字保持一致
        //   当前 Renderer 会上传：
        //   - "projectionMatrix"：实际为 ViewProjection（相机矩阵）
        //   - "transformMatrix"：每个物体的模型变换矩阵（位移/缩放等）
        std::string flatColorShaderVertexSrc = R"(
        #version 330 core
        layout(location = 0) in vec3 a_Position;

        uniform mat4 u_ViewProjection;
        uniform mat4 u_Transform;

        out vec3 v_Position;

        void main()
        {
            v_Position = a_Position;
            gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
		}
    )";

        std::string flatColorShaderFragmentSrc = R"(
	#version 330 core

        layout(location = 0) out vec4 color;

        in vec3 v_Position;

        uniform vec3 u_Color;
        void main()
        {
            color = vec4(u_Color, 1.0);
        }
        )";

        mFlatColorShader.reset(Leaf::Shader::Create(
            flatColorShaderVertexSrc, flatColorShaderFragmentSrc));
    }
    ~ExampleLayer() override = default;
    void OnUpdate(Leaf::Timestep ts) override {
        // 基于键盘输入更新相机的位置与旋转（与 Hazel 教程保持同样的方向约定）：
        // - 按右键：相机在世界中向 +X 方向移动 → 世界在屏幕上看起来向左移动
        // - 按上键：相机向 +Y 方向移动 → 世界在屏幕上看起来向下移动
        // - 按 A/D：相机逆/顺时针旋转 → 世界在屏幕上表现为相反方向旋转
        if (Leaf::Input::IsKeyPressed(LEAF_KEY_LEFT)) {
            LEAF_TRACE("Left arrow pressed");
            mCameraPosition.x -= mCameraMoveSpeed * ts;
        } else if (Leaf::Input::IsKeyPressed(LEAF_KEY_RIGHT)) {
            mCameraPosition.x += mCameraMoveSpeed * ts;
        }

        if (Leaf::Input::IsKeyPressed(LEAF_KEY_UP)) {
            mCameraPosition.y += mCameraMoveSpeed * ts;
        } else if (Leaf::Input::IsKeyPressed(LEAF_KEY_DOWN)) {
            mCameraPosition.y -= mCameraMoveSpeed * ts;
        }
        if (Leaf::Input::IsKeyPressed(LEAF_KEY_A)) {
            mCameraRotation += mCameraRotationSpeed * ts;
        } else if (Leaf::Input::IsKeyPressed(LEAF_KEY_D)) {
            mCameraRotation -= mCameraRotationSpeed * ts;
        }

        Leaf::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Leaf::RenderCommand::Clear();

        // 将累积下来的相机位置与旋转写回 OrthographicCamera，
        // 内部会通过 RecalculateViewMatrix() 生成对应的 View / ViewProjection
        // 矩阵
        mCamera.SetPosition(mCameraPosition);
        mCamera.SetRotation(mCameraRotation);

        // 场景渲染流程：
        // - BeginScene：缓存当前相机的 ViewProjection 到 Renderer::SceneData
        // - Submit：上传矩阵到 Shader 的 uniform 并绘制当前的 VertexArray
        // - EndScene：预留后续扩展用（当前为空实现）
        Leaf::Renderer::BeginScene(mCamera);

        // 这里用 transformMatrix 来模拟“物体的模型矩阵（Model）”：
        // - scale：每个小方块的统一缩放
        // - translate：每个小方块在世界坐标中的位置
        // 最终 transform = T(pos) * S(scale)
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

        std::dynamic_pointer_cast<Leaf::OpenGLShader>(mFlatColorShader)->Bind();
        std::dynamic_pointer_cast<Leaf::OpenGLShader>(mFlatColorShader)
            ->UploadUniformFloat3("u_Color", mSquareColor);

        for (int y = 0; y < 20; y++) {
            for (int x = 0; x < 20; x++) {
                glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
                glm::mat4 transform =
                    glm::translate(glm::mat4(1.0f), pos) * scale;
                Leaf::Renderer::Submit(mFlatColorShader, mSquareVertexArray,
                                       transform);
            }
        }
        Leaf::Renderer::Submit(mShader, mVertexArray);
        Leaf::Renderer::EndScene();
    }
    void OnEvent(Leaf::Event &event) override {
    }
    void OnImGuiRender() override {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Square Color", glm::value_ptr(mSquareColor));
        ImGui::End();
    }

private:
    glm::vec3 mCameraPosition {};
    float mCameraMoveSpeed = 5.0f;

    float mCameraRotation = 0.0f;
    float mCameraRotationSpeed = 180.0f;
    Leaf::OrthographicCamera mCamera;
    // 非正式 Demo 用的 Shader 与 VAO（当前示例只画一个简单三角形）
    std::shared_ptr<Leaf::Shader> mShader {nullptr};
    std::shared_ptr<Leaf::VertexArray> mVertexArray {nullptr};

    std::shared_ptr<Leaf::Shader> mFlatColorShader {nullptr};
    std::shared_ptr<Leaf::VertexArray> mSquareVertexArray {nullptr};
    glm::vec3 mSquareColor = {0.2f, 0.3f, 0.8f};
};

class Sandbox : public Leaf::Application {
public:
    Sandbox() {
        PushLayer(new ExampleLayer());
    };
    ~Sandbox() override = default;
};

Leaf::Application *Leaf::CreateApplication() {
    return new Sandbox();
}
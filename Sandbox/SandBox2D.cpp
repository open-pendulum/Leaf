#include "Sandbox2D.h"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "debug/Instrumentor.h"

Sandbox2D::Sandbox2D() :
    Layer("Sandbox2D"), mCameraController(1280.0f / 720.0f) {
}

void Sandbox2D::OnAttach() {
    LEAF_PROFILE_FUNCTION();

    mCheckerboardTexture = Leaf::Texture2D::Create(
        "D:\\workspace\\Leaf\\Sandbox\\assets\\textures\\Checkerboard.png");
}

void Sandbox2D::OnDetach() {
    LEAF_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Leaf::Timestep ts) {
    LEAF_PROFILE_FUNCTION();
    // Update

    mCameraController.OnUpdate(ts);

    {
        LEAF_PROFILE_SCOPE("Renderer Prep");
        Leaf::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Leaf::RenderCommand::Clear();
    }

    {
        LEAF_PROFILE_SCOPE("Renderer Draw");
        Leaf::Renderer2D::BeginScene(mCameraController.GetCamera());
        Leaf::Renderer2D::DrawQuad({-1.0f, 0.0f}, {0.8f, 0.8f},
                                   {0.8f, 0.2f, 0.3f, 1.0f});  // 红色四边形
        Leaf::Renderer2D::DrawQuad({0.5f, -0.5f}, {0.5f, 0.75f},
                                   {0.0f, 0.0f, 0.0f, 1.0f});  // 黑色四边形
        Leaf::Renderer2D::DrawQuad({0.0f, 0.0f, -0.1f}, {10.0f, 10.0f},
                                   mCheckerboardTexture);  // 棋盘格纹理背景
        Leaf::Renderer2D::EndScene();
    }
}

void Sandbox2D::OnImGuiRender() {
    LEAF_PROFILE_FUNCTION();

    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(mSquareColor));

    ImGui::End();
}

void Sandbox2D::OnEvent(Leaf::Event &e) {
    mCameraController.OnEvent(e);
}

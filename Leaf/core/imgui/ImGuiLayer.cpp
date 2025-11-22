//
// Created by Saber on 2025/3/15.
//

#include "ImGuiLayer.h"

#include "Application.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

// tmp
#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace Leaf {
ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {
}

ImGuiLayer::~ImGuiLayer() {
}
void ImGuiLayer::OnAttach() {
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGuiIO &io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    ImGui_ImplOpenGL3_Init("#version 410");
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
#ifdef USE_IMGUI_DOCKING
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
#endif
    ImGui_ImplGlfw_InitForOpenGL(
        static_cast<GLFWwindow *>(
            Application::Get().GetWindow().GetNativeWindow()),
        true);
}

void ImGuiLayer::OnDetach() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::OnImGuiRender() {
    static bool show = true;
    ImGui::ShowDemoWindow(&show);
}

void ImGuiLayer::Begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::End() {
    ImGuiIO &io = ImGui::GetIO();
    Application &app = Application::Get();
    io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(),
                            (float)app.GetWindow().GetHeight());

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#ifdef USE_IMGUI_DOCKING
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
#endif
}
}  // namespace Leaf
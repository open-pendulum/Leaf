

#include <LeafPCH.h>

#include <glm/ext/matrix_clip_space.hpp>  // glm::perspective
#include <glm/ext/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale
#include <glm/ext/scalar_constants.hpp>  // glm::pi
#include <glm/mat4x4.hpp>                // glm::mat4
#include <glm/vec3.hpp>                  // glm::vec3
#include <glm/vec4.hpp>                  // glm::vec4

#include "Leaf.h"
#include "imgui.h"

glm::mat4 camera(float Translate, glm::vec2 const &Rotate) {
    glm::mat4 Projection =
        glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
    glm::mat4 View =
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
    View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
    View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, -1.0f, 0.0f));
    glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    return Projection * View * Model;
}

class ExampleLayer : public Leaf::Layer {
public:
    ExampleLayer() : Layer("Example") {
        auto came = camera(0.3, {0.1, 0.2});
    }
    ~ExampleLayer() override = default;
    void OnUpdate() override {
        if (Leaf::Input::IsKeyPressed(LEAF_KEY_TAB)) {
            LEAF_INFO("Tab key is pressed (poll)!");
        }
    }
    void OnEvent(Leaf::Event &event) override {
        if (event.GetEventType() == Leaf::EventType::KeyPressed) {
            Leaf::KeyPressedEvent &e = (Leaf::KeyPressedEvent &)event;
            LEAF_TRACE("{0}", (char)e.GetKeyCode());
        }
    }
    void OnImGuiRender() override {
        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text.");
        ImGui::End();
    }
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
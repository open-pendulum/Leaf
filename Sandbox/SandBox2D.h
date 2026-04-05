#pragma once

#include "Leaf.h"

class Sandbox2D : public Leaf::Layer
{
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(Leaf::Timestep ts) override;
    virtual void OnImGuiRender() override;
    void OnEvent(Leaf::Event& e) override;
private:
    Leaf::OrthographicCameraController m_CameraController;

    // Temp
    Leaf::Ref<Leaf::VertexArray> m_SquareVA;
    Leaf::Ref<Leaf::Shader> m_FlatColorShader;

    glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};
#pragma once

#include "Leaf.h"

class Sandbox2D : public Leaf::Layer {
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(Leaf::Timestep ts) override;
    virtual void OnImGuiRender() override;
    void OnEvent(Leaf::Event &e) override;

private:
    Leaf::OrthographicCameraController mCameraController;

    // Temp
    Leaf::Ref<Leaf::VertexArray> mSquareVA;
    Leaf::Ref<Leaf::Shader> mFlatColorShader;
    Leaf::Ref<Leaf::Texture2D> mCheckerboardTexture;
    glm::vec4 mSquareColor = {0.2f, 0.3f, 0.8f, 1.0f};
};
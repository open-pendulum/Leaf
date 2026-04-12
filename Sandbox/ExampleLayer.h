#pragma once
#include <Leaf.h>


#include <glm/gtc/matrix_transform.hpp>  // glm::translate, glm::rotate
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include "platform/opengl/OpenGLShader.h"

// ExampleLayer 作为本节教程的演示层：
// - 持有一个正交相机和一个简单的矩形网格
// - 通过键盘控制“相机”的平移与旋转，从而观察到屏幕上矩形的反向运动
class ExampleLayer : public Leaf::Layer {
public:
    ExampleLayer();
    ~ExampleLayer() override = default;
    void OnUpdate(Leaf::Timestep ts) override;

    void OnEvent(Leaf::Event &event) override;

    void OnImGuiRender() override;

private:
    Leaf::OrthographicCameraController mCameraController;
    // 非正式 Demo 用的 Shader 与 VAO（当前示例只画一个简单三角形）
    Leaf::Ref<Leaf::Shader> mShader {nullptr};
    Leaf::Ref<Leaf::VertexArray> mVertexArray {nullptr};

    Leaf::Ref<Leaf::Shader> mFlatColorShader {nullptr};
    Leaf::Ref<Leaf::VertexArray> mSquareVertexArray {nullptr};
    glm::vec3 mSquareColor = {0.2f, 0.3f, 0.8f};
    Leaf::Ref<Leaf::Texture2D> mTexture {nullptr};
    Leaf::Ref<Leaf::Texture2D> mLogoTexture {nullptr};

    Leaf::ShaderLibrary mShaderLibrary;
};

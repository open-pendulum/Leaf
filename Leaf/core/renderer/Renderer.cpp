//
// Created by Saber on 2025/4/11.
//

#include "Renderer.h"

#include "Renderer2D.h"
#include "platform/opengl/OpenGLShader.h"
namespace Leaf {

// 保存当前场景通用数据的静态缓冲区：
// - 目前只存 ViewProjectionMatrix，后续可以扩展更多（光照、环境参数等）
Scope<Renderer::SceneData> Renderer::sSceneData =
    CreateScope<Renderer::SceneData>();

void Renderer::Init() {
    RenderCommand::Init();
    Renderer2D::Init();
}

void Renderer::Shutdown() {
    Renderer2D::Shutdown();
}

void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
    RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::BeginScene(OrthographicCamera &camera) {
    // 从相机中取出当前的 ViewProjection 矩阵缓存下来，
    // 之后所有 Submit 调用都会使用这一帧的矩阵进行渲染。
    sSceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}
void Renderer::EndScene() {
}
void Renderer::Submit(const Ref<Shader> &shader,
                      const Ref<VertexArray> &vertexArray,
                      const glm::mat4 &transform) {
    // 1. 确保使用指定的 Shader
    shader->Bind();
    // 2. 将当前场景缓存的 ViewProjection 矩阵上传到着色器中
    // 这个矩阵是相机空间到裁剪空间的变换（Projection × View）
    shader->SetMat4("u_ViewProjection", sSceneData->ViewProjectionMatrix);

    // 3. 将”物体的模型变换矩阵（Model）”上传到着色器中
    // 这个矩阵表示物体在世界空间中的变换（平移、旋转、缩放）
    shader->SetMat4("u_Transform", transform);
    // 4. 绑定顶点数组对象并发出绘制命令
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}

}  // namespace Leaf
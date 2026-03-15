#pragma once

#include "RenderCommand.h"

#include "OrthographicCamera.h"
#include "Shader.h"

namespace Leaf {

// Renderer 是一个高层渲染门面（facade）：
// - 屏蔽底层 RendererAPI / RenderCommand 的细节
// - 管理当前场景的公共数据（例如 ViewProjection 矩阵）
// - 提供 BeginScene / Submit / EndScene 这套简单接口给上层调用
class Renderer {
public:
    // 开始一帧场景渲染：
    // - 从传入的相机中缓存 ViewProjection 矩阵
    //   之后 Submit 时会使用它更新 Shader 的 uniform
    static void BeginScene(OrthographicCamera &camera);

    // 当前 Demo 中 EndScene 还没有具体逻辑，
    // 预留给后续批渲染 / 后处理等功能使用
    static void EndScene();

    // 提交一个绘制请求：
    // - 绑定 Shader，并把当前场景的 ViewProjection 矩阵上传为 uniform
    // - 绑定 VertexArray
    // - 调用 RenderCommand::DrawIndexed 走到底层 RendererAPI
    static void Submit(const std::shared_ptr<Shader> &shader,
                       const std::shared_ptr<VertexArray> &vertexArray);

    static RendererAPI::API GetAPI() {
        return RendererAPI::GetAPI();
    }

private:
    struct SceneData {
        glm::mat4 ViewProjectionMatrix;
    };
    static SceneData* sSceneData;
};

}  // namespace Leaf
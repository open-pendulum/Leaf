#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"

namespace Leaf {

// Renderer2D：2D 渲染抽象层
// 提供高效的 2D 图元绘制接口，内部管理四边形顶点和着色器资源
class Renderer2D {
public:
    // 初始化 Renderer2D 系统，创建四边形顶点和着色器资源
    static void Init();

    // 销毁 Renderer2D 系统，释放所有资源
    static void Shutdown();

    // 开始一个新的 2D 渲染场景，上传相机矩阵
    static void BeginScene(const OrthographicCamera &camera);

    // 结束当前渲染场景
    static void EndScene();

    // 绘制纯色四边形
    static void DrawQuad(const glm::vec2 &position, const glm::vec2 &size,
                         const glm::vec4 &color);

    static void DrawQuad(const glm::vec3 &position, const glm::vec2 &size,
                         const glm::vec4 &color);

    // 绘制纹理四边形
    static void DrawQuad(const glm::vec2 &position, const glm::vec2 &size,
                         const Ref<Texture2D> &texture);
    static void DrawQuad(const glm::vec3 &position, const glm::vec2 &size,
                         const Ref<Texture2D> &texture);
};
};  // namespace Leaf

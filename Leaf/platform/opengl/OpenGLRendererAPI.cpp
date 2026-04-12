//
// Created by Saber on 2025/4/12.
//

#include "OpenGLRendererAPI.h"

#include "debug/Instrumentor.h"
#include "glad/glad.h"
namespace Leaf {

void OpenGLMessageCallback(
        unsigned source,
        unsigned type,
        unsigned id,
        unsigned severity,
        int length,
        const char* message,
        const void* userParam)
{
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         LEAF_CORE_FATAL(message); return;
    case GL_DEBUG_SEVERITY_MEDIUM:       LEAF_CORE_ERROR(message); return;
    case GL_DEBUG_SEVERITY_LOW:          LEAF_CORE_WARN(message); return;
    case GL_DEBUG_SEVERITY_NOTIFICATION: LEAF_CORE_TRACE(message); return;
    }

    LEAF_CORE_ASSERT(false, "Unknown severity level!");
}
void OpenGLRendererAPI::Init() {
    LEAF_PROFILE_FUNCTION();

#define LEAF_DEBUG
#ifdef LEAF_DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLMessageCallback, nullptr);

    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif
    // 启用混合功能（Blending），用于透明渲染
    // 混合功能允许新绘制的像素与已存在帧缓冲区中的像素进行混合计算
    glEnable(GL_BLEND);

    // 设置混合函数参数：
    // GL_SRC_ALPHA: 使用源片段（新绘制）的alpha值作为源因子
    // GL_ONE_MINUS_SRC_ALPHA: 使用 (1 - 源片段alpha值) 作为目标因子
    //
    // 混合公式：result = source * source_alpha + dest * (1 - source_alpha)
    //
    // 这个配置实现了标准的alpha混合，使透明物体能够正确显示：
    // - alpha=1.0（不透明）：完全覆盖背景
    // - alpha=0.5（半透明）：与背景混合，显示为50%透明度
    // - alpha=0.0（完全透明）：完全显示背景
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 启用深度测试，用于 3D 场景中的深度缓冲计算
    glEnable(GL_DEPTH_TEST);
}

void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width,
                                    uint32_t height) {
    glViewport(x, y, width, height);
}
void OpenGLRendererAPI::SetClearColor(const glm::vec4 &color) {
    glClearColor(color.r, color.g, color.b, color.a);
}
void OpenGLRendererAPI::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray> &vertexArray) {
    glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(),
                   GL_UNSIGNED_INT, nullptr);
    // 解除纹理绑定，避免影响后续绘制
    glBindTextureUnit(GL_TEXTURE_2D, 0);
}

}  // namespace Leaf
#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"
#include "platform/opengl/OpenGLShader.h"

namespace Leaf {
Shader *Shader::Create(const std::string &vertexSrc,
                       const std::string &fragmentSrc) {
    // 工厂方法：根据当前渲染API创建对应的着色器实现
    // 使用策略模式，支持多种渲染后端扩展
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return new OpenGLShader(vertexSrc, fragmentSrc);
    default:
        LEAF_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}

}  // namespace Leaf
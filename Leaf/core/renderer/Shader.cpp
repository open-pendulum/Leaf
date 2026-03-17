#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"
#include "platform/opengl/OpenGLShader.h"

namespace Leaf {
Shader *Shader::Create(const std::string &vertexSrc,
                       const std::string &fragmentSrc) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return new OpenGLShader(vertexSrc, fragmentSrc);
    default:
        LEAF_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}

}  // namespace Leaf
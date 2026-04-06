#include "GraphicsContext.h"

#include "Renderer.h"
#include "platform/opengl/OpenGLContext.h"

namespace Leaf {

Scope<GraphicsContext> GraphicsContext::Create(void *window) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::None: {
        LEAF_CORE_ASSERT(false,
                         "RendererAPI::None is currently not supported!");
        return nullptr;
    }
    case RendererAPI::API::OpenGL: {
        return CreateScope<OpenGLContext>(static_cast<GLFWwindow *>(window));
    }
    }

    LEAF_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

}  // namespace Leaf
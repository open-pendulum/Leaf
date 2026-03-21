#include "Texture.h"

#include "Renderer.h"
#include "platform/opengl/OpenGLTexture.h"

namespace Leaf {

Ref<Texture2D> Texture2D::Create(const std::string &path) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::None: {
        LEAF_CORE_ASSERT(false,
                         "RendererAPI::None is currently not supported!");
        return nullptr;
    }
    case RendererAPI::API::OpenGL: {
        return std::make_shared<OpenGLTexture2D>(path);
    }
    default:
        LEAF_CORE_ASSERT(false, "Unknown RendererAPI!");
    }

    LEAF_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}
}  // namespace Leaf
#include "VertexArray.h"

#include "Renderer.h"
#include "platform/opengl/OpenGLVertexArray.h"
namespace Leaf {

Ref<VertexArray> VertexArray::Create() {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL: {
        return CreateRef<OpenGLVertexArray>();
    }
    default: {
        LEAF_CORE_ASSERT(false,
                         "RendererAPI::None is currently not supported!");
    }
    }

    LEAF_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}

}  // namespace Leaf
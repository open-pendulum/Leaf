#include "Buffer.h"

#include "Renderer.h"
#include "platform/opengl/OpenGLBuffer.h"

namespace Leaf {
VertexBuffer *VertexBuffer::Create(float *vertices, uint32_t size) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
        LEAF_CORE_ASSERT(false, "RendererAPI::None is currently not "
                                "supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return new OpenGLVertexBuffer(vertices, size);
    default:
        LEAF_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    };
}

IndexBuffer *IndexBuffer::Create(uint32_t *indices, uint32_t size) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
        LEAF_CORE_ASSERT(false, "RendererAPI::None is currently not "
                                "supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return new OpenGLIndexBuffer(indices, size);
    default:
        LEAF_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
}  // namespace Leaf
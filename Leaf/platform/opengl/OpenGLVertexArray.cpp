//
// Created by Saber on 2025/4/12.
//

#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace Leaf {
static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
    switch (type) {
    case ShaderDataType::Float:
        return GL_FLOAT;
    case ShaderDataType::Float2:
        return GL_FLOAT;
    case ShaderDataType::Float3:
        return GL_FLOAT;
    case ShaderDataType::Float4:
        return GL_FLOAT;
    case ShaderDataType::Mat3:
        return GL_FLOAT;
    case ShaderDataType::Mat4:
        return GL_FLOAT;
    case ShaderDataType::Int:
        return GL_INT;
    case ShaderDataType::Int2:
        return GL_INT;
    case ShaderDataType::Int3:
        return GL_INT;
    case ShaderDataType::Int4:
        return GL_INT;
    case ShaderDataType::Bool:
        return GL_BOOL;
    }
    LEAF_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
}

OpenGLVertexArray::OpenGLVertexArray() {
    glGenVertexArrays(1, &mRendererID);
}

OpenGLVertexArray::~OpenGLVertexArray() {
    glDeleteVertexArrays(1, &mRendererID);
}

void OpenGLVertexArray::Bind() const {
    glBindVertexArray(mRendererID);
}

void OpenGLVertexArray::Unbind() const {
    glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(
    const std::shared_ptr<VertexBuffer> &vertexBuffer) {
    LEAF_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(),
                     "Vertex Buffer has no layout!");

    glBindVertexArray(mRendererID);
    vertexBuffer->Bind();
    const auto &layout = vertexBuffer->GetLayout();
    uint32_t index = 0;
    for (auto &element : layout) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, element.GetComponentCount(),
                              ShaderDataTypeToOpenGLBaseType(element.type),
                              element.normalized ? GL_TRUE : GL_FALSE,
                              layout.GetStride(), (const void *)element.offset);
    }
    mVertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::SetIndexBuffer(
    const std::shared_ptr<IndexBuffer> &indexBuffer) {
    glBindVertexArray(mRendererID);
    indexBuffer->Bind();
    mIndexBuffer = indexBuffer;
}

}  // namespace Leaf
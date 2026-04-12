//
// Created by Saber on 2025/4/12.
//

#include "OpenGLVertexArray.h"

#include "debug/Instrumentor.h"
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
    LEAF_PROFILE_FUNCTION();

    glGenVertexArrays(1, &mRendererID);
}

OpenGLVertexArray::~OpenGLVertexArray() {
    LEAF_PROFILE_FUNCTION();

    glDeleteVertexArrays(1, &mRendererID);
}

void OpenGLVertexArray::Bind() const {
    LEAF_PROFILE_FUNCTION();

    glBindVertexArray(mRendererID);
}

void OpenGLVertexArray::Unbind() const {
    glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) {
    LEAF_PROFILE_FUNCTION();

    LEAF_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(),
                     "Vertex Buffer has no layout!");

    glBindVertexArray(mRendererID);
    vertexBuffer->Bind();
    const auto &layout = vertexBuffer->GetLayout();
    uint32_t index = 0;
    // 遍历所有顶点属性布局元素
    for (auto &element : layout) {
        // 启用顶点属性数组
        glEnableVertexAttribArray(mVertexBufferIndex);
        // 指定顶点属性的数据格式和位置
        // 参数：属性索引、分量数量、数据类型、是否归一化、步长、偏移量
        glVertexAttribPointer(mVertexBufferIndex, element.GetComponentCount(),
                              ShaderDataTypeToOpenGLBaseType(element.type),
                              element.normalized ? GL_TRUE : GL_FALSE,
                              layout.GetStride(),
                              reinterpret_cast<const void *>(element.offset));
        // 关键修复：递增属性索引，确保每个属性分配到正确的location
        // 否则所有属性都会被分配到location 0，导致顶点数据错位
        mVertexBufferIndex++;
    }
    mVertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer) {
    LEAF_PROFILE_FUNCTION();

    glBindVertexArray(mRendererID);
    indexBuffer->Bind();
    mIndexBuffer = indexBuffer;
}

}  // namespace Leaf
//
// Created by Saber on 2025/4/11.
//

#include "OpenGLBuffer.h"

#include "glad/glad.h"

namespace Leaf {
OpenGLVertexBuffer::OpenGLVertexBuffer(float *vertices, uint32_t size) {
    glCreateBuffers(1, &mRendererID);
    glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}
OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    glDeleteBuffers(1, &mRendererID);
}

void OpenGLVertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
}

void OpenGLVertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::SetLayout(const Leaf::BufferLayout &layout) {
    mLayout = layout;
}

const Leaf::BufferLayout &OpenGLVertexBuffer::GetLayout() const {
    return mLayout;
}

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t *indices, uint32_t count) :
    mCount(count) {
    glCreateBuffers(1, &mRendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, indices,
                 GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
    glDeleteBuffers(1, &mRendererID);
}

void OpenGLIndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
}

void OpenGLIndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t OpenGLIndexBuffer::GetCount() const {
    return mCount;
}

}  // namespace Leaf
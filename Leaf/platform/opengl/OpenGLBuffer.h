#pragma once

#include "renderer/Buffer.h"

namespace Leaf {
class OpenGLVertexBuffer : public VertexBuffer {
public:
    OpenGLVertexBuffer(float *vertices, uint32_t size);
    virtual ~OpenGLVertexBuffer();
    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual void SetLayout(const BufferLayout &layout) override;
    virtual const BufferLayout &GetLayout() const override;

private:
    uint32_t mRendererID;
    BufferLayout mLayout;
};

class OpenGLIndexBuffer : public IndexBuffer {
public:
    OpenGLIndexBuffer(uint32_t *indices, uint32_t size);
    virtual ~OpenGLIndexBuffer();
    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual uint32_t GetCount() const override;

private:
    uint32_t mRendererID;
    uint32_t mCount;
};

}  // namespace Leaf

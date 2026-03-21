#pragma once

#include "renderer/VertexArray.h"

namespace Leaf {
class OpenGLVertexArray : public VertexArray {
public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray();
    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual void AddVertexBuffer(
        const std::shared_ptr<VertexBuffer> &vertexBuffer) override;
    virtual void SetIndexBuffer(
        const std::shared_ptr<IndexBuffer> &indexBuffer) override;
    virtual const std::vector<std::shared_ptr<VertexBuffer>> &GetVertexBuffers()
        const override {
        return mVertexBuffers;
    }
    virtual const std::shared_ptr<IndexBuffer> &GetIndexBuffer()
        const override {
        return mIndexBuffer;
    }

private:
    uint32_t mRendererID;                                    // OpenGL VAO对象ID
    uint32_t mVertexBufferIndex = 0;                         // 当前可用的顶点属性索引（location）
    // 这个变量确保多个VertexBuffer添加时，每个属性都能分配到唯一的location
    // 避免所有属性都被分配到location 0的问题
    std::vector<std::shared_ptr<VertexBuffer>> mVertexBuffers;  // 关联的顶点缓冲区列表
    std::shared_ptr<IndexBuffer> mIndexBuffer;                // 关联的索引缓冲区
};
}  // namespace Leaf

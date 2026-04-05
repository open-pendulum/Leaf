#pragma once

#include "OpenGLShader.h"
#include "core/renderer/Texture.h"

namespace Leaf {
class OpenGLTexture2D : public Texture2D {
public:
    explicit OpenGLTexture2D(uint32_t width, uint32_t height);
    explicit OpenGLTexture2D(const std::string &path);
    ~OpenGLTexture2D() override;

    [[nodiscard]] uint32_t GetWidth() const override {
        return mWidth;
    }
    [[nodiscard]] uint32_t GetHeight() const override {
        return mHeight;
    }

    void Bind(uint32_t slot = 0) const override;

    void SetData(const void *data, uint32_t size) override;

private:
    std::string mPath;
    uint32_t mWidth, mHeight;
    uint32_t mRendererID;
    GLenum mInternalFormat;
    GLenum mDataFormat;
};
}  // namespace Leaf

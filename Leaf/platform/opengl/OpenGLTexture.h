#pragma once

#include "core/renderer/Texture.h"

namespace Leaf {
class OpenGLTexture2D : public Texture2D {
public:
    explicit OpenGLTexture2D(const std::string &path);
    ~OpenGLTexture2D() override;

    [[nodiscard]] uint32_t GetWidth() const override {
        return mWidth;
    }
    [[nodiscard]] uint32_t GetHeight() const override {
        return mHeight;
    }

    void Bind(uint32_t slot = 0) const override;

private:
    std::string mPath;
    uint32_t mWidth, mHeight;
    uint32_t mRendererID;
};
}  // namespace Leaf

//
// Created by pantasun on 2026/3/21.
//

#include "OpenGLTexture.h"
#include "third_party/stb_image/stb_image.h"
#include <glad/glad.h>

namespace Leaf {

OpenGLTexture2D::OpenGLTexture2D(const std::string &path) : mPath(path) {
    int width, height, channels;
    // 启用纹理垂直翻转，因为图片的坐标系和OpenGL纹理坐标系相反
    stbi_set_flip_vertically_on_load(1);
    // 使用stb_image加载图片
    stbi_uc *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    LEAF_CORE_ASSERT(data, "Failed to load image!");
    mWidth = width;
    mHeight = height;

    // 创建2D纹理对象
    glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
    // 为纹理分配存储空间，只分配1个mipmap级别（基本级别）
    glTextureStorage2D(mRendererID, 1, GL_RGB8, mWidth, mHeight);

    // 设置纹理参数
    // 缩小过滤：使用线性插值，产生平滑的缩小效果
    glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // 放大过滤：使用最近邻采样，保持像素清晰度（可能导致锯齿）
    glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // 上传纹理数据到GPU
    glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, GL_RGB,
                        GL_UNSIGNED_BYTE, data);

    // 释放CPU上的图像数据
    stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D() {
    glDeleteTextures(1, &mRendererID);
}

void OpenGLTexture2D::Bind(uint32_t slot) const {
    glBindTextureUnit(slot, mRendererID);
}
}  // namespace Leaf

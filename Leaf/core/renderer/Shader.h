#pragma once

#include <glm/glm.hpp>

namespace Leaf {

// Shader 封装了一组顶点/片元着色器程序：
// - 负责在 CPU 侧管理 OpenGL program 对象（mRendererID）
// - 提供 Bind/Unbind 接口切换当前使用的着色器
// - 提供上传 uniform 的帮助函数（例如 UploadUniformMat4）
class Shader {
public:
    // 从 GLSL 源码构造一个着色器程序：
    // - 内部会完成顶点/片元 shader 的编译与 program 的链接
    virtual ~Shader() = default;

    // 使当前 shader 成为 OpenGL 的活跃 program，
    // 之后的绘制调用都会使用它
    virtual void Bind() const = 0;

    // 取消使用当前 shader，相当于绑定 program 0
    virtual void Unbind() const = 0;

    static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);
};
}  // namespace Leaf
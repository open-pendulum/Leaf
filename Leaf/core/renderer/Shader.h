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
    Shader(const std::string &vertexSrc, const std::string &fragmentSrc);
    ~Shader();

    // 使当前 shader 成为 OpenGL 的活跃 program，
    // 之后的绘制调用都会使用它
    void Bind() const;

    // 取消使用当前 shader，相当于绑定 program 0
    void Unbind() const;

    // 上传一个 mat4 uniform：
    // - name：着色器中 uniform 变量名
    // - mat：要传入 GPU 的 4x4 矩阵（例如 ViewProjection）
    void UploadUniformMat4(const std::string &name, const glm::mat4 &mat);

private:
    uint32_t mRendererID;
};
}  // namespace Leaf
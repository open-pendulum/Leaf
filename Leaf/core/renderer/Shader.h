#pragma once

#include <glm/glm.hpp>
#include <unordered_map>
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

    virtual const std::string GetName() const = 0;

    static Ref<Shader> Create(const std::string &filePath);
    static Ref<Shader> Create(const std::string &name,
                              const std::string &vertexSrc,
                              const std::string &fragmentSrc);
};

class ShaderLibrary {
public:
    void Add(const std::string &name, const Ref<Shader> &shader);
    void Add(const Ref<Shader> &shader);
    Ref<Shader> Load(const std::string &filepath);
    Ref<Shader> Load(const std::string &name, const std::string &filepath);
    Ref<Shader> Get(const std::string &name);

    bool Exists(const std::string &name) const;

private:
    std::unordered_map<std::string, Ref<Shader>> mShaders;
};
}  // namespace Leaf
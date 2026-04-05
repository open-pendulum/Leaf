#pragma once

#include <glm/glm.hpp>
#include <unordered_map>

namespace Leaf {

// Shader：GPU 着色器程序封装
// 负责管理 OpenGL program 对象，提供 uniform 上传接口
class Shader {
public:
    virtual ~Shader() = default;

    // 绑定着色器程序，使其成为当前活跃程序
    virtual void Bind() const = 0;

    // 解除绑定，等效于绑定到 program 0
    virtual void Unbind() const = 0;

    // 获取着色器名称
    virtual const std::string GetName() const = 0;

    // uniform 上传接口
    virtual void SetInt(const std::string &name, int value) = 0;
    virtual void SetFloat3(const std::string &name, const glm::vec3 &value) = 0;
    virtual void SetFloat4(const std::string &name, const glm::vec4 &value) = 0;
    virtual void SetMat4(const std::string &name, const glm::mat4 &value) = 0;

    // 从文件创建着色器
    static Ref<Shader> Create(const std::string &filePath);
    // 从源码创建着色器
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
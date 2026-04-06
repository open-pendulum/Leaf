#include "Shader.h"

#include "Renderer.h"
#include "platform/opengl/OpenGLShader.h"

namespace Leaf {
Ref<Shader> Shader::Create(const std::string &name,
                           const std::string &vertexSrc,
                           const std::string &fragmentSrc) {
    // 工厂方法：根据当前渲染API创建对应的着色器实现
    // 使用策略模式，支持多种渲染后端扩展
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL: {
        return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
    }
    default:
        LEAF_CORE_ASSERT(false, "Unknown RendererAPI!");
    }
    return nullptr;
}

Ref<Shader> Shader::Create(const std::string &filePath) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL: {
        return CreateRef<OpenGLShader>(filePath);
    }
    default: {
        LEAF_CORE_ASSERT(false, "Unknown RendererAPI!");
    }
    }
    return nullptr;
}

void ShaderLibrary::Add(const std::string &name, const Ref<Shader> &shader) {
    LEAF_CORE_ASSERT(!Exists(name), "Shader already exists!");
    mShaders[name] = shader;
}

void ShaderLibrary::Add(const Ref<Shader> &shader) {
    auto &name = shader->GetName();
    Add(name, shader);
}

Ref<Shader> ShaderLibrary::Load(const std::string &filepath) {
    auto shader = Shader::Create(filepath);
    Add(shader);
    return shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string &name,
                                const std::string &filepath) {
    auto shader = Shader::Create(filepath);
    Add(name, shader);
    return shader;
}

Ref<Shader> ShaderLibrary::Get(const std::string &name) {
    LEAF_CORE_ASSERT(Exists(name), "Shader not found!");
    return mShaders[name];
}

bool ShaderLibrary::Exists(const std::string &name) const {
    return mShaders.find(name) != mShaders.end();
}

}  // namespace Leaf
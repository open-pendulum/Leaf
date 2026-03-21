#pragma once

#include <glm/glm.hpp>

#include "core/renderer/Shader.h"

typedef unsigned int GLenum;

namespace Leaf {

class OpenGLShader : public Shader {
public:
    explicit OpenGLShader(const std::string &name, const std::string &vertexSrc,
                          const std::string &fragmentSrc);
    explicit OpenGLShader(const std::string &filePath);
    virtual ~OpenGLShader() override;

    void Bind() const override;
    void Unbind() const override;
    const std::string GetName() const override;

    void UploadUniformInt(const std::string &name, int value);

    void UploadUniformFloat(const std::string &name, float value);
    void UploadUniformFloat2(const std::string &name, const glm::vec2 &value);
    void UploadUniformFloat3(const std::string &name, const glm::vec3 &value);
    void UploadUniformFloat4(const std::string &name, const glm::vec4 &value);

    void UploadUniformMat3(const std::string &name, const glm::mat3 &matrix);
    void UploadUniformMat4(const std::string &name, const glm::mat4 &matrix);

private:
    std::string ReadFile(const std::string &filePath);
    std::unordered_map<GLenum, std::string> PreProcess(
        const std::string &soruce);
    void Compile(const std::unordered_map<GLenum, std::string> &shaderSources);

private:
    uint32_t mRendererId;
    std::string mName;
};

}  // namespace Leaf

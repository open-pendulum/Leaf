#pragma once

#include <glm/glm.hpp>

#include "core/renderer//Shader.h"

namespace Leaf {

class OpenGLShader : public Shader {
public:
    OpenGLShader(const std::string &vertexSrc, const std::string &fragmentSrc);
    virtual ~OpenGLShader() override;

    void Bind() const override;
    void Unbind() const override;

    void UploadUniformInt(const std::string &name, int value);

    void UploadUniformFloat(const std::string &name, float value);
    void UploadUniformFloat2(const std::string &name, const glm::vec2 &value);
    void UploadUniformFloat3(const std::string &name, const glm::vec3 &value);
    void UploadUniformFloat4(const std::string &name, const glm::vec4 &value);

    void UploadUniformMat3(const std::string &name, const glm::mat3 &matrix);
    void UploadUniformMat4(const std::string &name, const glm::mat4 &matrix);

private:
    uint32_t mRendererId;
};

}  // namespace Leaf

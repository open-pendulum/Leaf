#include "Shader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Leaf {

Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc) {
    // ----------------------------
    // 1. 编译顶点着色器
    // ----------------------------
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *src = vertexSrc.c_str();
    glShaderSource(vertexShader, 1, &src, 0);

    glCompileShader(vertexShader);

    // 查询编译是否成功
    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
        glDeleteShader(vertexShader);
        LEAF_CORE_ERROR("{}", infoLog.data());
        LEAF_CORE_ASSERT(false, "Vertex shader failed to compile!");
        return;
    }
    // ----------------------------
    // 2. 编译片元着色器
    // ----------------------------
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    src = fragmentSrc.c_str();
    glShaderSource(fragmentShader, 1, &src, 0);

    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);
        LEAF_CORE_ERROR("{}", infoLog.data());
        LEAF_CORE_ASSERT(false, "Fragment shader failed to compile!");
        return;
    }
    // ----------------------------
    // 3. 链接 program
    // ----------------------------
    mRendererID = glCreateProgram();
    glAttachShader(mRendererID, vertexShader);
    glAttachShader(mRendererID, fragmentShader);
    glLinkProgram(mRendererID);
    GLint isLinked = 0;
    glGetProgramiv(mRendererID, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(mRendererID, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(mRendererID, maxLength, &maxLength, &infoLog[0]);
        glDeleteProgram(mRendererID);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        LEAF_CORE_ERROR("{}", infoLog.data());
        LEAF_CORE_ASSERT(false, "Shader program failed to link!");
        return;
    }
    glDetachShader(mRendererID, vertexShader);
    glDetachShader(mRendererID, fragmentShader);
}

Shader::~Shader() {
    // 释放 OpenGL program 对象
    glDeleteProgram(mRendererID);
}

void Shader::Bind() const {
    glUseProgram(mRendererID);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

void Shader::UploadUniformMat4(const std::string &name, const glm::mat4 &mat) {
    // 根据 uniform 名字查询在当前 program 中的位置，
    // 然后以列主序的方式把 4x4 矩阵数据上传到 GPU。
    GLint location = glGetUniformLocation(mRendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}


}  // namespace Leaf
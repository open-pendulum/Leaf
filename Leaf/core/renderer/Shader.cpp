#include "Shader.h"

#include "glad/glad.h"

namespace Leaf {

Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *src = vertexSrc.c_str();
    glShaderSource(vertexShader, 1, &src, 0);

    glCompileShader(vertexShader);

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
    glDeleteProgram(mRendererID);
}

void Shader::Bind() const {
    glUseProgram(mRendererID);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

}  // namespace Leaf
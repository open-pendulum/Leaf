//
// Created by Saber on 2025/4/6.
//
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>

#include "glad/glad.h"

namespace Leaf {
OpenGLContext::OpenGLContext(GLFWwindow *windowHandle) :
    mWindowHandle(windowHandle) {
    LEAF_ASSERT(windowHandle, "Window handle is null!");
}

OpenGLContext::~OpenGLContext() {
}
void OpenGLContext::Init() {
    glfwMakeContextCurrent(mWindowHandle);
    int stats = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    LEAF_CORE_ASSERT(stats, "Failed to initialize Glad!");
    LEAF_CORE_INFO("OpenGL Info:");

    LEAF_CORE_INFO("  Vendor: {}",
                   reinterpret_cast<const char *>(glGetString(GL_VENDOR)));
    LEAF_CORE_INFO("  Renderer: {}",
                   reinterpret_cast<const char *>(glGetString(GL_RENDERER)));
    LEAF_CORE_INFO("  Version: {}",
                   reinterpret_cast<const char *>(glGetString(GL_VERSION)));
}
void OpenGLContext::SwapBuffers() {
    glfwSwapBuffers(mWindowHandle);
}

}  // namespace Leaf
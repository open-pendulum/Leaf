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

#ifdef HZ_ENABLE_ASSERTS
    int versionMajor;
    int versionMinor;
    glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
    glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

    HZ_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Hazel requires at least OpenGL version 4.5!");
#endif
}
void OpenGLContext::SwapBuffers() {
    glfwSwapBuffers(mWindowHandle);
}

}  // namespace Leaf
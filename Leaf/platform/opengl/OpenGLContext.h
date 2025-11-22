#pragma once

#include "renderer/GraphicsContext.h"
class GLFWwindow;
namespace Leaf {

class OpenGLContext : public GraphicsContext {
public:
    OpenGLContext(GLFWwindow *windowHandle);
    virtual ~OpenGLContext() override;
    virtual void Init() override;
    virtual void SwapBuffers() override;

private:
    GLFWwindow *mWindowHandle;
};
}  // namespace Leaf
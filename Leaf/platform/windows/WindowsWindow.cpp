//
// Created by Saber on 2025/3/8.
//

#include "WindowsWindow.h"

#include <GLFW/glfw3.h>

#include "debug/Instrumentor.h"
#include "events/ApplicationEvent.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "platform/opengl/OpenGLContext.h"
#include "utils/Logger.h"

namespace Leaf {
static uint8_t s_GLFWInitializedCount = 0;
Scope<Window> Window::Create(const WindowProps &props) {
    return CreateScope<WindowsWindow>(props);
}

WindowsWindow::WindowsWindow(const WindowProps &props) {
    LEAF_PROFILE_FUNCTION();

    Init(props);
}
WindowsWindow::~WindowsWindow() {
    LEAF_PROFILE_FUNCTION();

    Shutdown();
}
void WindowsWindow::Init(const WindowProps &props) {
    LEAF_PROFILE_FUNCTION();

    mData.Title = props.Title;
    mData.Width = props.Width;
    mData.Height = props.Height;
    LEAF_INFO("Creating window {} ({}, {})", props.Title, props.Width,
              props.Height);
    if (s_GLFWInitializedCount == 0) {
        LEAF_PROFILE_SCOPE("glfw initialization");
        int success = glfwInit();
        LEAF_CORE_ASSERT(success, "Could not initialize GLFW!");
        glfwSetErrorCallback([](int error, const char *description) {
            LEAF_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
        });
    }
    mWindow = glfwCreateWindow((int)props.Width, (int)props.Height,
                               mData.Title.c_str(), nullptr, nullptr);
    {
        LEAF_PROFILE_SCOPE("glfwCreateWindow");
        mContext = GraphicsContext::Create(mWindow);
        ++s_GLFWInitializedCount;
    }

    mContext->Init();

    glfwSetWindowUserPointer(mWindow, &mData);
    SetVSync(true);

    // set glfw callback
    glfwSetWindowSizeCallback(
        mWindow, [](GLFWwindow *window, int width, int height) {
            WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;
            WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });

    glfwSetWindowCloseCallback(mWindow, [](GLFWwindow *window) {
        WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
        WindowCloseEvent event;
        data.EventCallback(event);
    });

    glfwSetKeyCallback(mWindow, [](GLFWwindow *window, int key, int scancode,
                                   int action, int mods) {
        WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
        switch (action) {
        case GLFW_PRESS: {
            KeyPressedEvent event(key, 0);
            data.EventCallback(event);
            break;
        }
        case GLFW_RELEASE: {
            KeyReleasedEvent event(key);
            data.EventCallback(event);
            break;
        }
        case GLFW_REPEAT: {
            KeyPressedEvent event(key, 1);
            data.EventCallback(event);
            break;
        }
        default:
            break;
        }
    });

    glfwSetCharCallback(mWindow, [](GLFWwindow *window, unsigned int keycode) {
        WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
        KeyTypedEvent event(keycode);
        data.EventCallback(event);
    });

    glfwSetMouseButtonCallback(
        mWindow, [](GLFWwindow *window, int button, int action, int mods) {
            WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
            switch (action) {
            case GLFW_PRESS: {
                MouseButtonPressedEvent event(button);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                MouseButtonReleasedEvent event(button);
                data.EventCallback(event);
                break;
            }
            default:
                break;
            }
        });

    glfwSetScrollCallback(
        mWindow, [](GLFWwindow *window, double xOffset, double yOffset) {
            WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            data.EventCallback(event);
        });

    glfwSetCursorPosCallback(
        mWindow, [](GLFWwindow *window, double xPos, double yPos) {
            WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
            MouseMovedEvent event((float)xPos, (float)yPos);
            data.EventCallback(event);
        });
}

void WindowsWindow::Shutdown() {
    LEAF_PROFILE_FUNCTION();

    glfwDestroyWindow(mWindow);
    if (--s_GLFWInitializedCount == 0) {
        LEAF_CORE_INFO("Terminating GLFW");
        glfwTerminate();
    }
}
void WindowsWindow::OnUpdate() {
    LEAF_PROFILE_FUNCTION();

    glfwPollEvents();
    mContext->SwapBuffers();
}

void WindowsWindow::SetVSync(bool enabled) {
    LEAF_PROFILE_FUNCTION();

    if (enabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);
    mData.VSync = enabled;
}

bool WindowsWindow::IsVSync() const {
    return mData.VSync;
}
void WindowsWindow::SetEventCallback(const EventCallbackFn &callback) {
    mData.EventCallback = callback;
}
}  // namespace Leaf
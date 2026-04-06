//
// Created by Saber on 2025/4/12.
//

#include "RendererAPI.h"

#include "platform/opengl/OpenGLRendererAPI.h"

namespace Leaf {
RendererAPI::API RendererAPI::sAPI = RendererAPI::API::OpenGL;

Scope<RendererAPI> RendererAPI::Create() {
    switch (sAPI) {
    case RendererAPI::API::OpenGL: {
        return CreateScope<OpenGLRendererAPI>();
    }
    default: {
        LEAF_CORE_ASSERT(false,
                         "RendererAPI::None is currently not supported!");
    }
    }

    LEAF_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

}  // namespace Leaf
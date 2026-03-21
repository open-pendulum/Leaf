//
// Created by Saber on 2025/4/12.
//

#include "RenderCommand.h"

#include "platform/opengl/OpenGLRendererAPI.h"

namespace Leaf {

Scope<RendererAPI> RenderCommand::sAPI = CreateScope<OpenGLRendererAPI>();

void RenderCommand::Init() {
    sAPI->Init();
}
void RenderCommand::Clear() {
    sAPI->Clear();
}

void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width,
                                uint32_t height) {
    sAPI->SetViewport(x, y, width, height);
}

void RenderCommand::SetClearColor(const glm::vec4 &color) {
    sAPI->SetClearColor(color);
}

void RenderCommand::DrawIndexed(
    const std::shared_ptr<VertexArray> &vertexArray) {
    sAPI->DrawIndexed(vertexArray);
}

}  // namespace Leaf
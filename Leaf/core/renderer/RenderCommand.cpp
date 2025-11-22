//
// Created by Saber on 2025/4/12.
//

#include "RenderCommand.h"

#include "platform/opengl/OpenGLRendererAPI.h"

namespace Leaf {

RendererAPI *RenderCommand::sAPI = new OpenGLRendererAPI();

void RenderCommand::Clear() {
    sAPI->Clear();
}

void RenderCommand::SetClearColor(const glm::vec4 &color) {
    sAPI->SetClearColor(color);
}

void RenderCommand::DrawIndexed(
    const std::shared_ptr<VertexArray> &vertexArray) {
    sAPI->DrawIndexed(vertexArray);
}

}  // namespace Leaf
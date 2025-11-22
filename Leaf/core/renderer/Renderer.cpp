//
// Created by Saber on 2025/4/11.
//

#include "Renderer.h"

namespace Leaf {

void Renderer::BeginScene() {
}
void Renderer::EndScene() {
}
void Renderer::Submit(const std::shared_ptr<VertexArray> &vertexArray) {
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}

}  // namespace Leaf
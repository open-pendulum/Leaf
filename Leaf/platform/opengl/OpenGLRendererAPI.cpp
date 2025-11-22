//
// Created by Saber on 2025/4/12.
//

#include "OpenGLRendererAPI.h"

#include "glad/glad.h"
namespace Leaf {

void OpenGLRendererAPI::SetClearColor(const glm::vec4 &color) {
    glClearColor(color.r, color.g, color.b, color.a);
}
void OpenGLRendererAPI::Clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(
    const std::shared_ptr<VertexArray> &vertexArray) {
    glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(),
                   GL_UNSIGNED_INT, nullptr);
}

}  // namespace Leaf
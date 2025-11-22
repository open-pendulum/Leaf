#pragma once

#include "glm/glm.hpp"
#include "renderer/RendererAPI.h"
#include "renderer/VertexArray.h"

namespace Leaf {

class RenderCommand {
public:
    static void Init();
    static void SetClearColor(const glm::vec4 &color);
    static void Clear();
    static void DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray);

private:
    static RendererAPI *sAPI;
};
}  // namespace Leaf

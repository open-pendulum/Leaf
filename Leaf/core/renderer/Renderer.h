#pragma

#include "RenderCommand.h"
namespace Leaf {

class Renderer {
public:
    static void BeginScene();
    static void EndScene();

    static void Submit(const std::shared_ptr<VertexArray> &vertexArray);

    static RendererAPI::API GetAPI() {
        return RendererAPI::GetAPI();
    }
};

}  // namespace Leaf
#include "VertexArray.h"

#include "platform/opengl/OpenGLVertexArray.h"
namespace Leaf {

Ref<VertexArray> VertexArray::Create() {
    return CreateRef<OpenGLVertexArray>();
}

}  // namespace Leaf
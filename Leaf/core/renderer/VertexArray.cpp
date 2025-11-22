#include "VertexArray.h"

#include "platform/opengl/OpenGLVertexArray.h"
namespace Leaf {

VertexArray *VertexArray::Create() {
    return new OpenGLVertexArray();
}

}  // namespace Leaf
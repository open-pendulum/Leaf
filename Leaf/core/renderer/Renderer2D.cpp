
#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"
#include "debug/Instrumentor.h"

namespace Leaf {

// Renderer2D 内部存储结构，管理所有渲染资源
struct Renderer2DStorage {
    Ref<VertexArray> quad_vertex_array;  // 四边形顶点数组
    Ref<Shader> texture_color_shader;    // 纹理着色器
    Ref<Texture2D> white_texture;        // 白色默认纹理（用于纯色绘制）
};

static Renderer2DStorage *s_Data;

void Renderer2D::Init() {
    LEAF_PROFILE_FUNCTION();
    s_Data = new Renderer2DStorage();
    s_Data->quad_vertex_array = VertexArray::Create();

    // 四边形顶点数据：位置(x,y,z) + 纹理坐标(u,v)
    float squareVertices[5 * 4] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // 左下
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,  // 右下
        0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  // 右上
        -0.5f, 0.5f,  0.0f, 0.0f, 1.0f   // 左上
    };

    Ref<VertexBuffer> squareVB =
        VertexBuffer::Create(squareVertices, sizeof(squareVertices));
    squareVB->SetLayout({
        {ShaderDataType::Float3, "a_Position"},  // 顶点位置
        {ShaderDataType::Float2, "a_TexCoord"},  // 纹理坐标
    });
    s_Data->quad_vertex_array->AddVertexBuffer(squareVB);

    uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
    Ref<IndexBuffer> squareIB = IndexBuffer::Create(
        squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
    s_Data->quad_vertex_array->SetIndexBuffer(squareIB);

    // 创建白色默认纹理，用于纯色四边形绘制
    s_Data->white_texture = Texture2D::Create(1, 1);
    uint32_t white_texture_data = 0xffffffff;
    s_Data->white_texture->SetData(&white_texture_data, sizeof(uint32_t));

    // 加载纹理着色器并绑定纹理单元
    s_Data->texture_color_shader = Shader::Create(
        "D:\\workspace\\Leaf\\Sandbox\\assets\\shaders\\Texture.glsl");
    s_Data->texture_color_shader->Bind();
    s_Data->texture_color_shader->SetInt("u_Texture", 0);
}

void Renderer2D::Shutdown() {
    LEAF_PROFILE_FUNCTION();
    delete s_Data;
}

void Renderer2D::BeginScene(const OrthographicCamera &camera) {
    LEAF_PROFILE_FUNCTION();
    // 上传视图投影矩阵，应用于所有后续绘制
    s_Data->texture_color_shader->Bind();
    s_Data->texture_color_shader->SetMat4("u_ViewProjection",
                                          camera.GetViewProjectionMatrix());
}

void Renderer2D::EndScene() {
    LEAF_PROFILE_FUNCTION();
    // 目前为空，留给后续批量渲染扩展
}

void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size,
                          const glm::vec4 &color) {
    DrawQuad({position.x, position.y, 0.0f}, size, color);
}

void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size,
                          const glm::vec4 &color) {
    LEAF_PROFILE_FUNCTION();
    s_Data->texture_color_shader->SetFloat4("u_Color", color);
    s_Data->white_texture->Bind();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                          glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
    s_Data->texture_color_shader->SetMat4("u_Transform", transform);

    s_Data->quad_vertex_array->Bind();
    RenderCommand::DrawIndexed(s_Data->quad_vertex_array);
}

void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size,
                          const Ref<Texture2D> &texture) {
    DrawQuad({position.x, position.y, 0.0f}, size, texture);
}

void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size,
                          const Ref<Texture2D> &texture) {
    LEAF_PROFILE_FUNCTION();
    // 纹理绘制：颜色设为白色(1,1,1,1)使纹理原色显示
    s_Data->texture_color_shader->SetFloat4("u_Color", glm::vec4(1.0f));
    texture->Bind();

    // 计算模型变换矩阵
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                          glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
    s_Data->texture_color_shader->SetMat4("u_Transform", transform);

    s_Data->quad_vertex_array->Bind();
    RenderCommand::DrawIndexed(s_Data->quad_vertex_array);
}

}  // namespace Leaf

#include "Sprite.h"

#include <glm/gtx/transform.hpp>
#include "VertexBufferLayout.h"

glm::mat4 Sprite::m_projection;

Sprite::Sprite(const std::string &file, float width, float height) : m_height(height), m_width(width)
{
    m_Texture = std::make_unique<Texture>(file);

    float positions[] = {
        0,
        0,
        1,
        0,
        0,
        1,
        1,
        0,
        0,
        1,
        1,
        1,
    };

    int fSize = 6 * 2;

    // Vertex array.
    m_VAO = std::make_unique<VertexArray>();

    // Vertex buffer
    m_VertexBuffer = std::make_unique<VertexBuffer>(positions, fSize);

    // Set attributes.
    VertexBufferLayout lay;
    lay.Push<float>(2);

    m_VAO->AddBuffer(*m_VertexBuffer, lay);

    // Unbind Vertex Array Object.
    m_VAO->Unbind();

    m_Shader = std::make_unique<Shader>("res/shaders/Sprite.shader");
}

Sprite::~Sprite() {}

void Sprite::Draw(Renderer r, glm::vec2 pos, bool inverted)
{
    DrawPartial(r, pos, 0, 0, m_Texture->GetWidth(), m_Texture->GetHeight(), inverted);
}

void Sprite::DrawPartial(Renderer r, glm::vec2 pos, int startx, int starty, int endx, int endy, bool inverted)
{
    float sx = (float)startx / m_Texture->GetWidth();
    float ex = (float)endx / m_Texture->GetWidth();
    float sy = (float)starty / m_Texture->GetHeight();
    float ey = (float)endy / m_Texture->GetHeight();

    m_Shader->Bind();
    m_Texture->Bind();
    m_Shader->setUniformMat4f("u_Projection", m_projection);
    m_Shader->setUniformMat4f("u_View", m_view);

    glm::mat4 model = glm::translate(glm::vec3(pos.x, pos.y, 0)) * glm::scale(glm::vec3(m_width, m_height, 0));
    m_Shader->setUniformMat4f("u_Model", model);

    m_Shader->setUniform2f("u_TexScale", glm::vec2(ex - sx, ey - sy));
    m_Shader->setUniform2f("u_TexOffset", glm::vec2(sx, sy));
    m_Shader->setUniform1i("u_Mirror", inverted);

    r.Draw(*m_VAO, *m_Shader);
}

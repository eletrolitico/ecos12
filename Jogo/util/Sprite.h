#ifndef SPRITE_H
#define SPRITE_H

#include <memory>
#include <glm/glm.hpp>
#include "Texture.h"

class Sprite
{
public:
    Sprite(const std::string &file, float width, float height);
    ~Sprite();

    static glm::mat4 m_projection;

    void Draw(Renderer r, glm::vec2 pos, bool inverted = false, float scale = 1.0f);

    /*! @brief Desenha parte da textura na área da sprite
     * o sx,sy,ex,ey definem um retângulo em píxeis
     *
     * @param[in] r renderer
     * @param[in] pos posição a ser desenhada, medida em tiles
     * @param[in] sx x do começo do retângulo
     * @param[in] sy y do começo do retângulo
     * @param[in] ex x do fim do retângulo
     * @param[in] ey y do fim do retângulo
     */
    void DrawPartial(Renderer r, glm::vec2 pos, int sx, int sy, int ex, int ey, bool inverted = false, float scale = 1.0f);

    void setWidth(float w) { m_width = w; }
    void setHeight(float h) { m_height = h; }

private:
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    glm::mat4 m_view{glm::mat4(1)};

protected:
    std::unique_ptr<Texture> m_Texture;
    float m_width, m_height;
};

#endif
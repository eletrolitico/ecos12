#ifndef TIRO_H
#define TIRO_H

#include <glm/glm.hpp>
#include <memory>
#include "Sprite.h"

class Tiro
{
public:
    Tiro(glm::vec2 pos, glm::vec2 speed, bool isSelf);
    ~Tiro();

    void draw(Renderer r);
    void update(float fElapsedTime);

    glm::vec2 m_pos, m_speed;
    float m_width = 3 * 0.68f, m_height = 3 * 0.09f;
    bool m_isSelf;

private:
    static Sprite *m_sprite;
    int m_vFrame = 0, m_hFrame = 0;
    float m_acum_time = 0.0f;
};

#endif
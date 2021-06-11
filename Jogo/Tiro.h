#ifndef TIRO_H
#define TIRO_H

#include <glm/glm.hpp>
#include <memory>
#include "Sprite.h"

class Tiro
{
public:
    Tiro(glm::vec2 pos, glm::vec2 speed);
    ~Tiro();

    void draw(Renderer r);
    bool update(float fElapsedTime);

private:
    glm::vec2 m_pos, m_speed;
    static Sprite *m_sprite;
};

#endif
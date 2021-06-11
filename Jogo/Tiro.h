#ifndef TIRO_H
#define TIRO_H

#include <glm/glm.hpp>

class Tiro
{
public:
    Tiro(glm::vec2 pos, glm::vec2 speed);
    ~Tiro();

    void Render();

private:
    glm::vec2 m_pos, m_speed;
};

#endif
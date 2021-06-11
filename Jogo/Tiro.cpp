#include "Tiro.h"

Sprite *Tiro::m_sprite = nullptr;

Tiro::Tiro(glm::vec2 pos, glm::vec2 speed) : m_pos(pos), m_speed(speed)
{
    if (m_sprite == nullptr)
    {
        m_sprite = new Sprite("res/textures/banana.png", 0.5f, 0.5f);
    }
}

Tiro::~Tiro() {}

void Tiro::draw(Renderer r)
{
    m_sprite->Draw(r, m_pos);
}

bool Tiro::update(float fElapsedTime)
{
    m_pos += fElapsedTime * m_speed;
    if (m_pos.x < 0 || m_pos.x > 32 || m_pos.y < 0 || m_pos.y > 18)
        return true;

    return false;
}

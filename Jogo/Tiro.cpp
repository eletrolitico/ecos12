#include "Tiro.h"

Sprite *Tiro::m_sprite = nullptr;

Tiro::Tiro(glm::vec2 pos, glm::vec2 speed) : m_pos(pos), m_speed(speed)
{
    if (m_sprite == nullptr)
    {
        m_sprite = new Sprite("res/textures/fireball.png", scale * 0.68f, scale * 0.09f);
    }
}

Tiro::~Tiro() {}

void Tiro::draw(Renderer r)
{
    m_sprite->DrawPartial(r, m_pos, m_hFrame * 68, (5 - m_vFrame) * 9, (m_hFrame + 1) * 68, (6 - m_vFrame) * 9, m_speed.x > 0);
}

bool Tiro::update(float fElapsedTime)
{
    m_acum_time += fElapsedTime;
    if (m_acum_time > 0.03333f)
    {
        m_hFrame++;
        if (m_hFrame > 9)
        {
            m_hFrame = 0;
            m_vFrame++;
            if (m_vFrame > 5)
            {
                m_vFrame = 0;
            }
        }
        m_acum_time = 0;
    }
    m_pos += fElapsedTime * m_speed;
    if (m_pos.x < 0 || m_pos.x > 32 || m_pos.y < 0 || m_pos.y > 18)
        return true;

    return false;
}

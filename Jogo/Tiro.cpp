#include "Tiro.h"

Sprite *Tiro::m_sprite = nullptr;

Tiro::Tiro(glm::vec2 pos, glm::vec2 speed, bool isSelf) : m_pos(pos), m_speed(speed), m_isSelf(isSelf)
{
    if (m_sprite == nullptr)
    {
        m_sprite = new Sprite("res/textures/fireball.png", m_width, m_height);
    }
}

Tiro::~Tiro() {}

void Tiro::draw(Renderer r)
{
    m_sprite->DrawPartial(r, m_pos, m_hFrame * 68, (5 - m_vFrame) * 9, (m_hFrame + 1) * 68, (6 - m_vFrame) * 9, m_speed.x > 0);
}

void Tiro::update(float fElapsedTime)
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
}

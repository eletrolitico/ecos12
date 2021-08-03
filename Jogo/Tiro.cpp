#include "Tiro.h"

std::unordered_map<int, Sprite *> Tiro::m_sprite;

Tiro::Tiro(glm::vec2 pos, glm::vec2 speed, uint32_t pID) : m_pos(pos), m_speed(speed), m_playerID(pID)
{
    std::string vet[] = {"fire_red", "fire_green", "fire_purple", "fire_blue", "fire_yellow"};
    if (m_sprite.count(pID % 5) == 0)
    {
        m_sprite[pID % 5] = new Sprite("res/textures/" + vet[pID % 5] + ".png", m_width, m_height);
    }
}

Tiro::~Tiro() {}

void Tiro::draw(Renderer r)
{
    m_sprite[m_playerID % 5]->DrawPartial(r, m_pos, m_hFrame * 68, (5 - m_vFrame) * 9, (m_hFrame + 1) * 68, (6 - m_vFrame) * 9, m_speed.x > 0);
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

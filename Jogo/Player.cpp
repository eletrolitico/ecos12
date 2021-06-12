#include "Player.h"

#include "VertexBufferLayout.h"

#include <glm/gtx/transform.hpp>
float ts = 32;

Player::Player(int texture) : m_PlayerPos(glm::vec3(2, 7, 0)), m_PlayerSpeed(glm::vec2(0, 0))
{
    std::string vet[] = {"player_sprite", "player_sprite"};
    m_sprite = std::make_unique<Sprite>("res/textures/" + vet[texture] + ".png", 1.0f, 1.0f);
    this->texture = texture;
}

Player::~Player()
{
}

void Player::draw(Renderer r)
{
    /*if (texture == 0)
    {
        m_sprite->DrawPartial(r, m_PlayerPos, (m_State)*32, 504 - ((m_Frame + 1) * 48 - 8), (m_State + 1) * 32, 504 - ((m_Frame)*48 - 8), m_Mirror);
    }
    if (texture == 1)
    {*/
    m_sprite->DrawPartial(r, m_PlayerPos, m_Frame * 32, (3 - m_State) * 32, (m_Frame + 1) * 32, (4 - m_State) * 32, m_Mirror);
    //}
}

float accumTime = 0.0f;
void Player::update(float fElapsedTime, const Map &map)
{
    float g = 50 * fElapsedTime;
    float x = m_PlayerPos.x;
    float y = m_PlayerPos.y;
    if (map.isInverted(m_PlayerPos))
    {
        g = -g;
        m_IsInverted = true;
        m_Rotation += 500 * fElapsedTime;
    }
    else
    {
        m_IsInverted = false;
        m_Rotation -= 500 * fElapsedTime;
    }
    if (m_Rotation > 180.0f)
        m_Rotation = 180.0f;
    if (m_Rotation < 0.0f)
        m_Rotation = 0.0f;

    if (m_State != 3)
    {

        m_PlayerSpeed.y -= g;

        glm::vec2 mv = (float)fElapsedTime * m_PlayerSpeed;

        if (map.getCollide(x, y, glm::vec2(0, mv.y)))
        {
            m_PlayerPos.y += mv.y;
            m_Ground = false;
        }
        else
        {
            if ((mv.y < 0 && !m_IsInverted) || (mv.y > 0 && m_IsInverted))
                m_Ground = true;
            else
                m_Ground = false;
            m_PlayerSpeed.y = 0;

            if (mv.y > 0)
                m_PlayerPos.y = floor(y) + 0.999f;
            else
                m_PlayerPos.y = floor(y);
        }
        auto dd = map.getDanger();
        if ((dd.find(map.getMap(x, (int)(m_PlayerPos.y - 0.1f))) != dd.end() || dd.find(map.getMap(x + 1, (int)(m_PlayerPos.y - 0.1f))) != dd.end()) && mv.y < 0)
        {
            m_State = 3;
            m_PlayerSpeed = {0.0f, 0.0f};
            update_frame(fElapsedTime);
            return;
        }

        y = m_PlayerPos.y;

        if (map.getCollide(x, y, glm::vec2(mv.x, 0)))
        {
            m_PlayerPos.x += mv.x;
        }
        else
        {
            // o personagem tem 0.5 de largura
            m_PlayerSpeed.x = 0;
            if (mv.x > 0)
                // um pouquinho menos que 0.25
                m_PlayerPos.x = floor(x) + 0.249f;
            else
                // um pouquinho mais que 0.75
                m_PlayerPos.x = floor(x) + 0.751f;
        }

        if (!m_Ground)
            m_State = 2;
    }
    update_frame(fElapsedTime);
}

void Player::update_frame(float fElapsedTime)
{
    accumTime += fElapsedTime;

    if (accumTime > 0.1)
    {
        accumTime = 0;
        m_Frame++;
    }

    /*if (texture == 0)
    {
        switch (m_State)
        {
        case 0:
            if (m_Frame > 5)
                m_Frame = 0;
            break;
        case 1:
            if (m_Frame > 7)
                m_Frame = 0;
            break;
        case 2:
            if (m_Frame > 7)
                m_Frame = 0;
            break;
        case 3:
            if (m_Frame > 11)
                m_Frame = 7;
            break;
        }
    }

    if (texture == 1)
    {*/
    switch (m_State)
    {
    case 0:
        if (m_Frame > 3)
            m_Frame = 0;
        break;
    case 1:
        if (m_Frame > 5)
            m_Frame = 0;
        break;
    case 2:
        if (m_Frame > 7)
            m_Frame = 0;
        break;
    case 3:
        if (m_Frame > 3)
            m_Frame = 0;
        break;
    }
    //}
}

void Player::moveLeft()
{
    if (m_PlayerSpeed.x > -10)
        m_PlayerSpeed.x -= 1;
    m_State = 1;
    m_Mirror = !m_IsInverted;
}

void Player::moveRight()
{
    if (m_PlayerSpeed.x < 10)
        m_PlayerSpeed.x += 1;
    m_State = 1;
    m_Mirror = m_IsInverted;
}

void Player::stop()
{
    if (m_PlayerSpeed.x > 0)
        m_PlayerSpeed.x -= 1;
    else
        m_PlayerSpeed.x += 1;

    if (abs(m_PlayerSpeed.x) < 5)
        m_PlayerSpeed.x = 0;
    m_State = 0;
}

void Player::jump()
{
    if (m_Ground)
    {
        m_PlayerSpeed.y += (1 - 2 * m_IsInverted) * sqrt(2 * 50 * 3.5);
        m_Frame = 0;
    }
}
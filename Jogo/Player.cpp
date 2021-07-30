#include "Player.h"

#include "VertexBufferLayout.h"
#include "Text.h"

#include <glm/gtx/transform.hpp>

Player::Player(int texture) : m_PlayerPos(glm::vec3(2, 7, 0)), m_PlayerSpeed(glm::vec2(0, 0))
{
    std::string vet[] = {"player_red", "player_green", "player_purple", "player_blue", "player_yellow"};
    m_sprite = new Sprite("res/textures/" + vet[texture] + ".png", 1.0f, 1.0f);
    this->texture = texture;
}

Player::Player(int texture, uint32_t pID) : m_PlayerPos(glm::vec3(2, 7, 0)), m_PlayerSpeed(glm::vec2(0, 0)), nPlayerID(pID)
{
    std::string vet[] = {"player_red", "player_green", "player_purple", "player_blue", "player_yellow"};
    m_sprite = new Sprite("res/textures/" + vet[texture] + ".png", 1.0f, 1.0f);
    this->texture = texture;
}

void Player::swapTex(int t)
{
    delete m_sprite;

    std::string vet[] = {"player_red", "player_green", "player_purple", "player_blue", "player_yellow"};
    m_sprite = new Sprite("res/textures/" + vet[t] + ".png", 1.0f, 1.0f);
    this->texture = t;
}

Player::~Player()
{
    delete m_sprite;
}

void Player::draw(Renderer r)
{
    auto drawPos = m_PlayerPos;
    auto textPos = m_PlayerPos;
    std::string str = m_name + "|" + std::to_string(m_vida);
    drawPos.x -= 0.2f;
    textPos.y += 1.0f;
    textPos.x -= 0.25f * ((str.length() - 3) / 2.0f) - 0.25f;
    m_sprite->DrawPartial(r, drawPos, m_Frame * 32, (3 - m_State) * 32, (m_Frame + 1) * 32, (4 - m_State) * 32, m_Mirror);
    Text::GetText()->DrawString(r, textPos, 0.25f, str);
}

void Player::update_frame(float fElapsedTime)
{
    accumTime += fElapsedTime;

    if (accumTime > 0.1)
    {
        accumTime = 0;
        m_Frame++;
    }

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
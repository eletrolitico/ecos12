#ifndef PLAYER
#define PLAYER

#include <memory>

#include "Renderer.h"
#include "Sprite.h"
#include "Map.h"

class Player
{
public:
    glm::vec2 m_PlayerPos;
    glm::vec2 m_PlayerSpeed;
    bool m_Ground = false;
    // 0=idle 1=walking 2=jumping, 3 = dead, 4 = next stage
    int m_State = 0;
    float m_width = 0.5f, m_height = 1.0f;
    uint32_t nPlayerID = 0;

    Player(int texture);
    Player(int texture, uint32_t pID);
    ~Player();

    void moveLeft();
    void moveRight();
    void stop();
    void jump();
    void update_frame(float fElapsedTime);

    void draw(Renderer r);

    void swapTex(int t);

    bool m_Mirror = false;

    std::string m_name = "Player";
    uint8_t m_vida = 10;

private:
    Sprite *m_sprite;
    bool m_IsInverted = false;
    float m_Rotation = 0.0f;
    int texture;
    int m_Frame = 0;
    float accumTime = 0.0f;
};

#endif
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

    Player(int texture);
    ~Player();

    void update(float fElapsedTime, const Map &map);

    void moveLeft();
    void moveRight();
    void stop();
    void jump();
    void update_frame(float fElapsedTime);

    void draw(Renderer r);

    inline void setBig()
    {
        m_IsBig = true;
    }
    inline void setSmall()
    {
        m_IsBig = false;
    }

    bool m_Mirror = false;

private:
    std::unique_ptr<Sprite> m_sprite;
    bool m_IsBig = false;
    bool m_IsInverted = false;
    float m_Rotation = 0.0f;

    int m_Frame = 0;
};

#endif
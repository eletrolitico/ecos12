#ifndef GAME_H
#define GAME_H

#include <vector>
#include <unordered_map>

#include "Renderer.h"
#include "Player.h"
#include "SoundEngine.h"
#include "Map.h"
#include "Sprite.h"
#include "Tiro.h"

class Game
{

public:
    Game();
    ~Game();
    void draw(Renderer r);
    void update(float);
    void keyboardDown(int key);
    void keyboardUp(int key);

private:
    glm::mat4 m_Proj, m_View;
    float xScreen = 0, yScreen = 0;

    std::unordered_map<int, bool> m_keys;
    std::vector<Map *> m_Map;
    int m_CurrentMap = 0;
    int m_MapCount;
    Player m_Self;
    std::vector<Player *> m_Players;

    std::vector<Tiro *> m_tiros;

    SoundEngine *m_Sound;

    void updatePlayer(Player *p, float fElapsedTime);
    bool updateTiro(Tiro *t, float fElapsedTime);

    Tile getTile(char, int);
};

#endif
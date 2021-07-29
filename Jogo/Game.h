#ifndef GAME_H
#define GAME_H

#include <vector>
#include <unordered_map>

#include "../Rede/net_common.h"

#include "Renderer.h"
#include "Player.h"
#include "SoundEngine.h"
#include "Map.h"
#include "Sprite.h"
#include "Tiro.h"
#include "Text.h"

class Game : public olc::net::client_interface<GameMsg>
{

public:
    Game(const std::string &name);
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
    std::unordered_map<uint32_t, Player *> m_Players;

    std::vector<Tiro *> m_tiros;

    SoundEngine *m_Sound;
    Text *m_Text;

    bool bWaitingForConnection = true;
    sPlayerDescription descPlayer;

    void updatePlayer(Player *p, float fElapsedTime);
    bool updateTiro(Tiro *t, float fElapsedTime);

    Tile getTile(char, int);
};

#endif
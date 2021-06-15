#include "Game.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "Sprite.h"

Game::Game() : m_Proj(glm::ortho(0.0f, 32.0f, 0.0f, 18.0f)), m_View(glm::mat4(1)), m_Self(0)
{
    Sprite::m_projection = m_Proj;
    //R grama flutuante esquerda
    //T grama flutuante meio
    //Y grama flutuante direita
    //E grama esquerda
    //G grama meio
    //D grama direita
    //L terra esquerda
    //U terra meio
    //K terra direita
    //A fundo esquerda
    //S fundo meio
    //B fundo direita
    //O espinho
    //P espinho com back
    //F fim do mapa
    //H Tijolo
    //W parede
    Map *tempmap;
    std::string tmp = "";
    tmp += "................................";
    tmp += "................................";
    tmp += "................................";
    tmp += "................................";
    tmp += "................................";
    tmp += "................................";
    tmp += "................................";
    tmp += ".........RTTY...................";
    tmp += "................................";
    tmp += "..................RTY...........";
    tmp += "...........EGD..................";
    tmp += "...........LUK...........OOO....";
    tmp += "EGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGD";
    tmp += "LUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUK";
    tmp += "ASSSSSSSSSSSSSSSSSSSSSSSSSSSSSSB";
    tempmap = new Map(tmp, 32, 15, 2, 3, 1.0);
    //tempmap->addEntity(new Item(glm::vec3(13.0f, 9.0f, 0.0f), glm::vec2(5.0f, 0.0f), tempmap)); //<- BANANA
    m_Map.push_back(tempmap);
    m_Self.m_PlayerPos = {2, 3};

    Player *pTmp = new Player(1);
    pTmp->m_PlayerPos = {30, 3};
    m_Players.push_back(pTmp);

    m_MapCount = 1;

    // Sound
    m_Sound = SoundEngine::GetInstance();
    m_Sound->loadAudio("res/audio/jump.ogg", "jump", 0.6f, false);
    m_Sound->loadAudio("res/audio/morte.ogg", "death", 0.8f, false);
    m_Sound->loadAudio("res/audio/fireball.ogg", "fire", 0.8f, false);
    m_Sound->streamAudio("res/audio/cobblestone_village.ogg", "music", 0.1f, true);
}

Game::~Game()
{
    for (auto m : m_Map)
        delete m;
    for (auto p : m_Players)
        delete p;
}

void Game::draw(Renderer r)
{
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 mvp = m_Proj * m_View;
    glm::vec2 pp = m_Self.m_PlayerPos;

    for (auto tiro : m_tiros)
        tiro->draw(r);

    m_Self.draw(r);
    for (auto &p : m_Players)
        p->draw(r);
    m_Map[m_CurrentMap]->draw(r, mvp, glm::vec2(pp.x + 0.5f, pp.y + 0.5f));
}

void Game::update(float fElapsedTime)
{
    if (m_Self.m_State == 4)
    {
        m_CurrentMap++;
        if (m_CurrentMap < m_MapCount)
        {
            m_Self.m_PlayerPos = m_Map[m_CurrentMap]->getInitialPos();
            m_Self.m_State = 0;
        }
        else
        {
            //you win
            m_Sound->playAudio("jump");
        }
    }
    // if not dead
    if (m_Self.m_State != 3)
    {
        if ((m_keys[GLFW_KEY_SPACE] || m_keys['W']))
        {
            if (m_Self.m_Ground)
                m_Sound->playAudio("jump");
            m_Self.jump();
        }
        if (m_keys['D'])
            m_Self.moveRight();
        else if (m_keys['A'])
            m_Self.moveLeft();
        else
            m_Self.stop();

        static float accumTime = 0;
        accumTime += fElapsedTime;
        static bool pressedE = false;
        if (m_keys['E'] && m_tiros.size() < 20 && accumTime > 0.5 && !pressedE)
        {
            pressedE = true;
            glm::vec2 spd = {5.0f, 0.0f};
            accumTime = 0;
            glm::vec2 pos = m_Self.m_PlayerPos;
            if (m_Self.m_Mirror)
            {
                spd.x *= -1.0f;
                pos.x -= 1.0f;
            }
            pos.y += 0.25f;
            m_tiros.push_back(new Tiro(pos, spd));
            m_Sound->playAudio("fire");
        }

        if (!m_keys['E'])
            pressedE = false;
    }

    static bool music = true;
    if (m_keys[GLFW_KEY_ENTER] && m_Self.m_State == 3)
    {
        m_Self.m_State = 0;
        m_Self.m_PlayerPos = m_Map[m_CurrentMap]->getInitialPos();
        m_Self.setSmall();
        if (!music)
        {
            music = true;
            m_Sound->playAudio("music");
        }
    }

    static int prevState = 0;
    if (m_Self.m_State == 3 && prevState != 3)
    {
        m_Sound->pauseAudio("music");
        m_Sound->playAudio("death");
        music = false;
    }
    prevState = m_Self.m_State;

    static bool keyPrev = false;
    if (m_keys['M'] && m_keys['M'] != keyPrev)
    {
        if (music)
            m_Sound->pauseAudio("music");
        else
            m_Sound->playAudio("music");
        music = !music;
    }
    keyPrev = m_keys['M'];

    m_Self.update(fElapsedTime, *m_Map[m_CurrentMap]);

    for (auto &p : m_Players)
        p->update(fElapsedTime, *m_Map[m_CurrentMap]);
    /*
    if (m_Self.m_PlayerPos.x - xScreen > 7)
        xScreen = m_Self.m_PlayerPos.x - 7;
    if (m_Self.m_PlayerPos.x - xScreen < 1.6)
        xScreen = m_Self.m_PlayerPos.x - 1.6;

    if (xScreen < 0.01f)
        xScreen = 0.01f;

    if (xScreen > m_Map[m_CurrentMap]->m_width - 32.01f)
        xScreen = m_Map[m_CurrentMap]->m_width - 32.01f;

    yScreen = m_Self.m_PlayerPos.y - 6.5;

    if (yScreen < 0.01f)
        yScreen = 0.01f;

    if (yScreen > m_Map[m_CurrentMap]->m_height - 18.01f)
        yScreen = m_Map[m_CurrentMap]->m_height - 18.01f;
*/

    m_View = glm::translate(glm::mat4(1), glm::vec3(-xScreen, -yScreen, 0));
    m_Sound->update();
    m_Map[m_CurrentMap]->update(fElapsedTime);

    for (int i = m_tiros.size() - 1; i >= 0; --i)
        if (m_tiros[i]->update(fElapsedTime))
        {
            delete m_tiros[i];
            m_tiros.erase(m_tiros.begin() + i);
        }
}

void Game::keyboardDown(int key)
{
    m_keys[key] = true;
}

void Game::keyboardUp(int key)
{
    m_keys[key] = false;
}

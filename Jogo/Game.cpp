#include "Game.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "Sprite.h"

#include <string.h>

#define VELOCIDADE_FOGO 20.0f

Game::Game(const std::string &name) : m_Proj(glm::ortho(0.0f, 32.0f, 0.0f, 18.0f)), m_View(glm::mat4(1)), m_Self(0)
{
    if (!Connect("127.0.0.1", 60000))
    {
        std::cout << "Falha ao conectar";
        exit(1);
    }
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
    tmp += "........RTY..........RTY........";
    tmp += "................................";
    tmp += "..RTTY....................RTTY..";
    tmp += "...............RY...............";
    tmp += ".......RTY............RTY.......";
    tmp += "................................";
    tmp += "..............OOOO..............";
    tmp += "EGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGD";
    tmp += "LUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUK";
    tmp += "ASSSSSSSSSSSSSSSSSSSSSSSSSSSSSSB";
    tempmap = new Map(tmp, 32, 15, 2, 3, 1.0);
    m_Map.push_back(tempmap);
    m_Self.m_PlayerPos = {2, 3};
    m_Self.m_name = name;

    m_MapCount = 1;

    // Sound
    m_Sound = SoundEngine::GetInstance();
    m_Sound->loadAudio("res/audio/jump.ogg", "jump", 0.6f, false);
    m_Sound->loadAudio("res/audio/morte.ogg", "death", 0.8f, false);
    m_Sound->loadAudio("res/audio/fireball.ogg", "fire", 0.2f, false);
    m_Sound->streamAudio("res/audio/cobblestone_village.ogg", "music", 0.1f, true);

    // Text
    m_Text = Text::GetText();
}

Game::~Game()
{
    for (auto m : m_Map)
        delete m;
    for (auto p : m_Players)
        delete p.second;
}

void Game::draw(Renderer r)
{
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (!IsConnected())
        return;

    glm::mat4 mvp = m_Proj * m_View;
    glm::vec2 pp = m_Self.m_PlayerPos;

    m_Map[m_CurrentMap]->draw(r, mvp, glm::vec2(pp.x + 0.5f, pp.y + 0.5f));

    for (auto tiro : m_tiros)
        tiro->draw(r);

    m_Self.draw(r);
    for (auto &p : m_Players)
        p.second->draw(r);

    m_Text->DrawString(r, {1.0f, 8.0f}, 0.2f, "String de teste! 1234567890");
    m_Text->DrawString(r, {1.3f, 14.0f}, 0.2f, "String 2");
}

void Game::update(float fElapsedTime)
{
    if (!IsConnected())
        return;

    while (!Incoming().empty())
    {
        auto msg = Incoming().pop_front().msg;

        switch (msg.header.id)
        {
        case (GameMsg::Client_Accepted):
        {
            std::cout << "Server accepted client - you're in!\n";
            olc::net::message<GameMsg> msg;
            msg.header.id = GameMsg::Client_RegisterWithServer;
            descPlayer.vPos = m_Self.m_PlayerPos;
            msg << descPlayer;
            Send(msg);
            break;
        }

        case (GameMsg::Client_AssignID):
        {
            // Server is assigning us OUR id
            msg >> m_Self.nPlayerID;
            std::cout << "Assigned Client ID = " << m_Self.nPlayerID << "\n";
            break;
        }

        case (GameMsg::Game_AddPlayer):
        {
            sPlayerDescription desc;
            msg >> desc;

            if (desc.nUniqueID == m_Self.nPlayerID)
            {
                // Now we exist in game world
                bWaitingForConnection = false;
                m_Self.swapTex(desc.nUniqueID % 5);
            }
            else
            {
                m_Players[desc.nUniqueID] = new Player(desc.nUniqueID % 5, desc.nUniqueID);
            }
            break;
        }

        case (GameMsg::Game_RemovePlayer):
        {
            uint32_t nRemovalID = 0;
            msg >> nRemovalID;
            m_Players.erase(nRemovalID);
            break;
        }

        case (GameMsg::Game_UpdatePlayer):
        {
            sPlayerDescription desc;
            msg >> desc;
            if (m_Players.count(desc.nUniqueID) == 0)
                break;
            m_Players[desc.nUniqueID]->m_PlayerPos = desc.vPos;
            m_Players[desc.nUniqueID]->m_PlayerSpeed = desc.vVel;
            m_Players[desc.nUniqueID]->m_State = desc.state;
            m_Players[desc.nUniqueID]->m_name = desc.nome;

            break;
        }
        case (GameMsg::Game_Fire):
        {
            sFireBall fire;
            msg >> fire;
            m_tiros.push_back(new Tiro(fire.vPos, fire.vVel, fire.playerID));
            m_Sound->playAudio("fire");
            break;
        }
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
            glm::vec2 spd = {VELOCIDADE_FOGO, 0.0f};
            accumTime = 0;
            glm::vec2 pos = m_Self.m_PlayerPos;
            if (m_Self.m_Mirror)
            {
                spd.x *= -1.0f;
                pos.x -= 1.0f;
            }
            pos.y += 0.25f;
            m_tiros.push_back(new Tiro(pos, spd, m_Self.nPlayerID));
            m_Sound->playAudio("fire");

            olc::net::message<GameMsg> msg;
            msg.header.id = GameMsg::Game_Fire;
            struct sFireBall fire;
            fire.playerID = m_Self.nPlayerID;
            fire.vPos = pos;
            fire.vVel = spd;
            msg << fire;
            Send(msg);
        }

        if (!m_keys['E'])
            pressedE = false;
    }

    static bool music = true;
    if (m_keys[GLFW_KEY_ENTER] && m_Self.m_State == 3)
    {
        m_Self.m_State = 0;
        m_Self.m_PlayerPos = m_Map[m_CurrentMap]->getInitialPos();
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

    updatePlayer(&m_Self, fElapsedTime);

    for (auto p : m_Players)
        updatePlayer(p.second, fElapsedTime);
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
        if (updateTiro(m_tiros[i], fElapsedTime))
        {
            delete m_tiros[i];
            m_tiros.erase(m_tiros.begin() + i);
        }

    // Send player description
    olc::net::message<GameMsg> msg;
    msg.header.id = GameMsg::Game_UpdatePlayer;
    descPlayer.nUniqueID = m_Self.nPlayerID;
    descPlayer.vPos = m_Self.m_PlayerPos;
    descPlayer.vVel = m_Self.m_PlayerSpeed;
    descPlayer.state = m_Self.m_State;
    strcpy(descPlayer.nome, m_Self.m_name.c_str());
    msg << descPlayer;
    Send(msg);
}

void Game::updatePlayer(Player *p, float fElapsedTime)
{
    if (p == nullptr)
        return;
    float g = 50 * fElapsedTime;
    float x = p->m_PlayerPos.x;
    float y = p->m_PlayerPos.y;

    if (p->m_State == 3)
    {
        p->update_frame(fElapsedTime);
        return;
    }

    p->m_PlayerSpeed.y -= g;

    glm::vec2 mv = (float)fElapsedTime * p->m_PlayerSpeed;

    if (m_Map[m_CurrentMap]->getCollide(x, y + mv.y, x + p->m_width, y + p->m_height + mv.y))
    {
        if (mv.y < 0)
            p->m_Ground = true;
        else
            p->m_Ground = false;
        p->m_PlayerSpeed.y = 0;

        if (mv.y > 0)
            p->m_PlayerPos.y = floor(y) + 0.999f;
        else
            p->m_PlayerPos.y = floor(y);
    }
    else
    {
        p->m_PlayerPos.y += mv.y;
        p->m_Ground = false;
    }

    if (m_Map[m_CurrentMap]->getDanger(x, y + mv.y, x + p->m_width, y + mv.y) && mv.y < 0)
    {
        p->m_State = 3;
        p->m_PlayerSpeed = {0.0f, 0.0f};
        p->update_frame(fElapsedTime);
        m_Sound->playAudio("death");
        return;
    }

    y = p->m_PlayerPos.y;

    if (m_Map[m_CurrentMap]->getCollide(x + mv.x, y, x + p->m_width + mv.x, y + p->m_height))
    {
        // o personagem tem 0.5 de largura
        p->m_PlayerSpeed.x = 0;
        if (mv.x > 0)
            p->m_PlayerPos.x = floor(x) + p->m_width - 0.001f;
        else
            p->m_PlayerPos.x = floor(x);
    }
    else
    {
        p->m_PlayerPos.x += mv.x;
        if (p->m_PlayerSpeed.x > 0.01)
            p->m_Mirror = false;
        else if (p->m_PlayerSpeed.x < -0.01)
            p->m_Mirror = true;
    }

    if (abs(p->m_PlayerSpeed.x) > 0.01)
        p->m_State = 1;
    else
        p->m_State = 0;

    if (!p->m_Ground)
        p->m_State = 2;

    p->update_frame(fElapsedTime);
}

bool Game::updateTiro(Tiro *t, float fElapsedTime)
{
    t->update(fElapsedTime);
    t->m_pos += fElapsedTime * t->m_speed;
    if (t->m_pos.x < 0 || t->m_pos.x > 32 || t->m_pos.y < 0 || t->m_pos.y > 18)
        return true;

    if (m_Map[m_CurrentMap]->getCollide(t->m_pos.x, t->m_pos.y, t->m_pos.x + t->m_width, t->m_pos.y + t->m_height))
    {
        m_Sound->stopAudio("fire");
        return true;
    }

    if (m_Self.m_State != 3 && t->m_playerID != m_Self.nPlayerID)
        if (((t->m_pos.x > m_Self.m_PlayerPos.x && t->m_pos.x < m_Self.m_PlayerPos.x + m_Self.m_width) ||
             (t->m_pos.x + t->m_width > m_Self.m_PlayerPos.x && t->m_pos.x + t->m_width < m_Self.m_PlayerPos.x + m_Self.m_width)) &&
            ((t->m_pos.y > m_Self.m_PlayerPos.y && t->m_pos.y < m_Self.m_PlayerPos.y + m_Self.m_height) ||
             (t->m_pos.y + t->m_height > m_Self.m_PlayerPos.y && t->m_pos.y + t->m_height < m_Self.m_PlayerPos.y + m_Self.m_height)))
        {
            m_Self.m_State = 3;
            m_Self.m_PlayerSpeed = {0.0f, 0.0f};
            m_Sound->playAudio("death");
            return true;
        }

    for (auto p : m_Players)
        if (p.second->m_State != 3 && p.second->nPlayerID != t->m_playerID)
            if (((t->m_pos.x > p.second->m_PlayerPos.x && t->m_pos.x < p.second->m_PlayerPos.x + p.second->m_width) ||
                 (t->m_pos.x + t->m_width > p.second->m_PlayerPos.x && t->m_pos.x + t->m_width < p.second->m_PlayerPos.x + p.second->m_width)) &&
                ((t->m_pos.y > p.second->m_PlayerPos.y && t->m_pos.y < p.second->m_PlayerPos.y + p.second->m_height) ||
                 (t->m_pos.y + t->m_height > p.second->m_PlayerPos.y && t->m_pos.y + t->m_height < p.second->m_PlayerPos.y + p.second->m_height)))
            {
                p.second->m_State = 3;
                p.second->m_PlayerSpeed = {0.0f, 0.0f};
                m_Sound->playAudio("death");
                return true;
            }

    return false;
}

void Game::keyboardDown(int key)
{
    m_keys[key] = true;
}

void Game::keyboardUp(int key)
{
    m_keys[key] = false;
}

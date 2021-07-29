#ifndef NET_COMMON_H
#define NET_COMMON_H

#include <cstdint>
#include <glm/glm.hpp>

#define OLC_PGEX_NETWORK
#include "lib/network.h"

enum class GameMsg : uint32_t
{
    Server_GetStatus,
    Server_GetPing,

    Client_Accepted,
    Client_AssignID,
    Client_RegisterWithServer,
    Client_UnregisterWithServer,

    Game_AddPlayer,
    Game_RemovePlayer,
    Game_UpdatePlayer,
    Game_Fire,
};

struct sPlayerDescription
{
    uint32_t nUniqueID = 0;
    char nome[30];
    uint8_t vida = 10;

    glm::vec2 vPos;
    glm::vec2 vVel;
    uint8_t state;
};

struct sFireBall
{
    uint32_t playerID = 0;

    glm::vec2 vPos;
    glm::vec2 vVel;
};

#endif
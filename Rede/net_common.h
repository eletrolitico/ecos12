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
};

struct sPlayerDescription
{
    uint32_t nUniqueID = 0;
    uint32_t nAvatarID = 0;

    uint32_t nHealth = 100;
    uint32_t nAmmo = 20;
    uint32_t nKills = 0;
    uint32_t nDeaths = 0;

    float fRadius = 0.5f;

    glm::vec2 vPos;
    glm::vec2 vVel;
};

#endif
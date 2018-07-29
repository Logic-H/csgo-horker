#include "iclient.h"
#include "../patterns.h"
#include "../sdk/cglowobjectmanager.h"
#include <iostream>
#include <unistd.h>

IClient::IClient(MemoryManager &mem) : IBase(mem)
{
    constexpr char sMod[] = "client_panorama_client.so";

    uintptr_t localPlayerLea = mem.FindInModule(sMod,
        PAT_LOCALPLAYER, PAT_LOCALPLAYER_OFF);
    m_aLocalPlayer = mem.GetCallAddress(localPlayerLea);

    uintptr_t glowFunCall = mem.FindInModule(sMod,
        PAT_GLOWFUNCALL, PAT_GLOWFUNCALL_OFF);
    uintptr_t glowFun = mem.GetCallAddress(glowFunCall);
    m_aGlowPtr = mem.GetCallAddress(glowFun + 0xC);

    uintptr_t playerResourcesMov = mem.FindInModule(sMod,
        "48 8B 05 ?? ?? ?? ?? 55 48 89 E5 48 85 C0 74 10 48", 0x3);
    m_aPlayerResource = mem.GetCallAddress(playerResourcesMov);
    /*
    uintptr_t pr = 0;
    if (mem.Read(aPlayerResources, pr)) {
        uintptr_t kills = 0;
        for (int i = 0; i < 10; ++i) {
            mem.Read(pr + 0xf78 + i * 4, kills);
            std::cout << kills << '\n';
        }
    }
    */

    uintptr_t foundAttackMov = mem.FindInModule(sMod,
        "F7 ?? 83 ?? ?? 45 ?? ?? 74", 0xE);
    m_aForceAttack = mem.GetCallAddress(foundAttackMov);

    Update();
}

void IClient::Update()
{
    uintptr_t localPlayer = 0;
    if (m_mem.Read(m_aLocalPlayer, localPlayer)) {
        m_pLocalPlayer = localPlayer;
    }
}

CGlowObjectManager IClient::GetGlowObjectManager()
{
    CGlowObjectManager manager;
    if (!m_mem.Read(m_aGlowPtr, manager)) {
        throw 1;
    }
    return manager;
}

CBaseEntity IClient::GetLocalPlayerEntity()
{
    CBaseEntity ent;
    if (!m_mem.Read(m_pLocalPlayer, ent)) {
        throw 1;
    }
    return ent;
}

void IClient::SetForceAttack(bool enabled)
{
    int toggleOn = enabled ? 5 : 4;
    m_mem.Write(m_aForceAttack, toggleOn);
}

uintptr_t IClient::GetLocalPlayer()
{
    return m_pLocalPlayer;
}

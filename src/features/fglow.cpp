#include "fglow.h"

#include "../sdk/cglowobjectmanager.h"
#include "../offsets.h"

#include <cstring>
#include <iostream>
#include <thread>

void FGlow::WaitWarn(const std::string &warning)
{
    std::cout << warning << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void FGlow::Run(MemoryManager &mem, IClient &client)
{
    std::cout << "Started glow thread" << std::endl;

    while (!StopRequested()) {
        constexpr size_t szGlowDef = sizeof(GlowObjectDefinition_t);

        struct iovec g_remote[1024];
        struct iovec g_local[1024];
        struct GlowObjectDefinition_t g_glow[1024];

        memset(g_remote, 0, sizeof(g_remote));
        memset(g_local, 0, sizeof(g_local));
        memset(g_glow, 0, sizeof(g_glow));

        CGlowObjectManager manager;
        if (!client.GetGlowManager(manager)) {
            WaitWarn("Failed to get GlowObjectManager");
            continue;
        }

        size_t count = manager.Count();
        void *data_ptr = manager.Data();

        if (!mem.Read(data_ptr, g_glow, szGlowDef * count)) {
            WaitWarn("Failed to read m_GlowObjectDefinitions");
            continue;
        }

        uintptr_t localPlayer = 0;
        if (!client.GetLocalPlayer(localPlayer)) {
            WaitWarn("Failed to get local player address");
            continue;
        }

        int myTeam = 0;
        if (!mem.Read(localPlayer + OFF_TEAM, myTeam)) {
            WaitWarn("Failed to get local team number");
            continue;
        }

        size_t writeCount = 0;
        for (size_t i = 0; i < count; ++i) {
            if (g_glow[i].m_pEntity != NULL) {
                CBaseEntity ent;
                if (mem.Read(g_glow[i].m_pEntity, ent)) {
                    if (ent.GetTeam() < TEAM_T) {
                        g_glow[i].SetRender(true, false);
                        continue;
                    }

                    if (ent.GetHealth() < 1) {
                        continue;
                    }

                    if (g_glow[i].m_bRenderWhenOccluded) {
                        continue;
                    }

                    g_glow[i].SetRender(true, false);

                    if (ent.GetTeam() > TEAM_SPEC) {
                        if (ent.GetTeam() == myTeam) {
                            g_glow[i].SetColor(0.f, 0.8f, 0.8f, 0.8f);
                        } else {
                            g_glow[i].SetColor(0.8f, 0.8f, 0.f, 0.8f);
                        }
                    }
                }
            }

            size_t bytesToCutOffEnd = szGlowDef - g_glow[i].writeEnd();
            size_t bytesToCutOffBegin = (size_t)g_glow[i].writeStart();
            size_t totalWriteSize = (szGlowDef - (bytesToCutOffBegin + bytesToCutOffEnd));
            g_remote[writeCount].iov_base =
                ((uint8_t*)data_ptr + (szGlowDef * i)) + bytesToCutOffBegin;
            g_local[writeCount].iov_base = ((uint8_t*)&g_glow[i]) + bytesToCutOffBegin;
            g_remote[writeCount].iov_len = g_local[writeCount].iov_len = totalWriteSize;

            writeCount++;
        }
        mem.WriteMulti(g_local, g_remote, writeCount);
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
    std::cout << "Stopping glow thread" << std::endl;
}

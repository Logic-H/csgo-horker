#include "fglow.h"

#include "../sdk/cglowobjectmanager.h"
#include "../offsets.h"

#include <cstring>
#include <iostream>
#include <thread>

FGlow::FGlow(MemoryManager &mem, IClient &client) : 
    m_mem(mem), m_client(client)
{

}

FGlow::~FGlow()
{
    Stop();
}

void FGlow::Start()
{
    Stop();
    m_stop = false;
    m_thread = std::thread(&FGlow::Run, this);
}

void FGlow::Stop()
{
    m_stop = true;
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

void FGlow::WaitWarn(const std::string &warning)
{
    std::cout << "WARN: " << warning << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void FGlow::Run()
{
    std::cout << "Started glow thread" << std::endl;

    while (!m_stop) {
        constexpr size_t szGlowDef = sizeof(GlowObjectDefinition_t);

        struct iovec g_remote[1024];
        struct iovec g_local[1024];
        struct GlowObjectDefinition_t g_glow[1024];

        memset(g_remote, 0, sizeof(g_remote));
        memset(g_local, 0, sizeof(g_local));
        memset(g_glow, 0, sizeof(g_glow));

        CGlowObjectManager manager;
        if (!m_client.GetGlowManager(manager)) {
            WaitWarn("Failed to get GlowObjectManager");
            continue;
        }

        size_t count = manager.Count();
        void *data_ptr = manager.Data();

        if (!m_mem.Read(data_ptr, g_glow, szGlowDef * count)) {
            WaitWarn("Failed to read m_GlowObjectDefinitions");
            continue;
        }

        uintptr_t localPlayer = 0;
        if (!m_client.GetLocalPlayer(localPlayer)) {
            WaitWarn("Failed to get local player address");
            continue;
        }

        int myTeam = 0;
        if (!m_mem.Read(localPlayer + OFF_TEAM, myTeam)) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }

        size_t writeCount = 0;
        for (size_t i = 0; i < count; ++i) {
            if (g_glow[i].m_pEntity != NULL) {
                CBaseEntity ent;
                if (m_mem.Read(g_glow[i].m_pEntity, ent)) {
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
        m_mem.WriteMulti(g_local, g_remote, writeCount);
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
    std::cout << "Stopping glow thread" << std::endl;
}

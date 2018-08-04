#include "fglow.h"

#include "../sdk/cglowobjectmanager.h"
#include "../offsets.h"

typedef struct GlowObjectDefinition_t glow_t;

FGlow::FGlow(MemoryManager &mem, IClient &client) : 
    m_mem(mem), m_client(client)
{

}

void FGlow::Run()
{
    Log("Started Glow");

    while (!ShouldStop()) {
        std::array<struct iovec, 512> g_remote;
        std::array<struct iovec, 512> g_local;
        std::array<glow_t, 512> g_glow;

        CGlowObjectManager manager;
        if (!m_client.GetGlowManager(manager)) {
            LogWait("Failed to get GlowObjectManager");
            continue;
        }

        const size_t count = manager.Count();
        void *data_ptr = manager.Data();

        if (!m_mem.Read(data_ptr, g_glow, sizeof(glow_t) * count)) {
            LogWait("Failed to read m_GlowObjectDefinitions");
            continue;
        }

        uintptr_t localPlayer = 0;
        if (!m_client.GetLocalPlayer(localPlayer)) {
            LogWait("Failed to get local player address");
            continue;
        }

        int myTeam = 0;
        if (!m_mem.Read(localPlayer + OFF_TEAM, myTeam)) {
            Wait();
            continue;
        }

        size_t writeCount = 0;
        for (size_t i = 1; i < count; ++i) {
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

            g_remote[writeCount].iov_base =
                ((uint8_t*)data_ptr + (sizeof(glow_t) * i)) + glow_t::WriteStart();
            g_local[writeCount].iov_base = ((uint8_t*)&g_glow[i]) + glow_t::WriteStart();
            g_remote[writeCount].iov_len = g_local[writeCount].iov_len = glow_t::WriteSize();

            writeCount++;
        }
        m_mem.WriteMulti(g_local.data(), g_remote.data(), writeCount);
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
    Log("Stopped Glow");
}

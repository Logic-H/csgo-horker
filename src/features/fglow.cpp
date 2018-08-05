#include "fglow.h"

#include "../sdk/cglowobjectmanager.h"
#include "../engine.h"
#include "../offsets.h"

typedef struct GlowObjectDefinition_t glow_t;

static inline void GlowPlayer(GlowObjectDefinition_t &glowObject, int localTeam, int otherTeam)
{
    if (localTeam == otherTeam) {
        glowObject.SetRender(true, false);
        glowObject.SetColor(0.f, .8f, .8f, .8f);
    } else {
        glowObject.SetRender(true, false);
        glowObject.SetColor(.8f, .8f, 0.f, .8f);
    }
}

void FGlow::Run()
{
    auto& eng = Engine::GetInstance();
    Log("[FGlow] Started");

    while (!ShouldStop()) {
        try {
            std::array<struct iovec, 512> g_remote;
            std::array<struct iovec, 512> g_local;
            std::array<glow_t, 512> g_glow;

            CGlowObjectManager *manager = eng.GetGlowObjectManager();

            const size_t count = manager->Count();
            void *data_ptr = manager->Data();

            if (!m_mem.Read(data_ptr, g_glow, sizeof(glow_t) * count)) {
                LogWait("[Glow] Failed to read m_GlowObjectDefinitions");
                continue;
            }

            int myTeam = eng.GetLocalPlayerTeam();
            
            size_t writeCount = 0;
            for (size_t i = 1; i < count; ++i) {
                if (g_glow[i].m_pEntity != NULL) {
                    CBaseEntity ent;
                    if (m_mem.Read(g_glow[i].m_pEntity, ent)) {
                        if (g_glow[i].m_bRenderWhenOccluded) {
                            continue;
                        } else if (ent.m_iHealth < 1 /* && !config::GlowWeapons */) {
                            continue;
                        }
                        if (ent.m_iTeamNum == TEAM_T || ent.m_iTeamNum == TEAM_CT) {
                            GlowPlayer(g_glow[i], myTeam, ent.GetTeam());
                        }
                    }
                }

                g_remote[writeCount].iov_base =
                    ((uint8_t*)data_ptr + (sizeof(glow_t) * i)) + glow_t::WriteStart();
                g_local[writeCount].iov_base = ((uint8_t*)&g_glow[i]) + glow_t::WriteStart();
                g_remote[writeCount].iov_len = glow_t::WriteSize();
                g_local[writeCount].iov_len = glow_t::WriteSize();

                writeCount++;
            }
            m_mem.WriteMulti(g_local.data(), g_remote.data(), writeCount);
        } catch (std::runtime_error &e) {
            LogWait(e.what());
        }
        WaitMs(2);
    }
    Log("[FGlow] Stopped");
}

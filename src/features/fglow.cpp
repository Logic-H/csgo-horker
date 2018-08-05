#include "fglow.h"

#include "../sdk/cglowobjectmanager.h"
#include "../config.h"
#include "../engine.h"
#include "../offsets.h"

typedef struct GlowObjectDefinition_t glow_t;

void FGlow::Run()
{
    auto& eng = Engine::GetInstance();

    const float clrE[4] = {
        Config::Glow::EnemyR,
        Config::Glow::EnemyG,
        Config::Glow::EnemyB,
        Config::Glow::EnemyA
    };

    const float clrA[4] = {
        Config::Glow::AllyR,
        Config::Glow::AllyG,
        Config::Glow::AllyB,
        Config::Glow::AllyA
    };

    const float clrO[4] { .9f, .5f, .9f, .4f };

    const bool bGlowAllies = Config::Glow::GlowAllies;
    const bool bGlowEnemies = Config::Glow::GlowEnemies;
    const bool bGlowOther = Config::Glow::GlowOther;
    const bool bGlowWeapons = Config::Glow::GlowWeapons;
    const bool bLegitGlow = Config::Glow::LegitGlow;

    if (!Config::Glow::Enabled) {
        return;
    }

    Log("[FGlow] Started");

    while (!ShouldStop()) {
        try {
            std::array<struct iovec, 1024> g_remote;
            std::array<struct iovec, 1024> g_local;
            std::array<glow_t, 1024> g_glow;

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
                        }
                        if (bLegitGlow) {
                            g_glow[i].m_nGlowStyle = 2;
                        }
                        if (ent.m_iTeamNum == TEAM_SPEC || ent.m_iTeamNum == TEAM_NONE) {
                            if (!bGlowOther) {
                                continue;
                            }
                            g_glow[i].SetRender(true, false);
                            g_glow[i].SetColor(clrO);
                        } else if (ent.m_iTeamNum == TEAM_T || ent.m_iTeamNum == TEAM_CT) {
                            if (bGlowAllies && ent.m_iTeamNum == myTeam) {
                                if (ent.m_iHealth < 1 && !bGlowWeapons) {
                                    continue;
                                }
                                g_glow[i].SetRender(true, false);
                                g_glow[i].SetColor(clrA);
                            } else if (bGlowEnemies && ent.m_iTeamNum != myTeam) {
                                if (ent.m_iHealth < 1 && !bGlowWeapons) {
                                    continue;
                                }
                                g_glow[i].SetRender(true, false);
                                g_glow[i].SetColor(clrE);
                            } else {
                                continue;
                            }
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

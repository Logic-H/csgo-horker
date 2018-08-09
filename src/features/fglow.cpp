#include "fglow.h"

#include "../sdk/cglowobjectmanager.h"
#include "../config.h"
#include "../engine.h"
#include "../offsets.h"

typedef struct GlowObjectDefinition_t glow_t;

void FGlow::Run()
{
    if (!Config::Glow::Enabled) {
        return;
    }

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

    const float clrO[4] {.9f, .5f, .9f, .4f};

    const bool bGlowAllies = Config::Glow::GlowAllies;
    const bool bGlowEnemies = Config::Glow::GlowEnemies;
    const bool bGlowOther = Config::Glow::GlowOther;
    const bool bGlowWeapons = Config::Glow::GlowWeapons;
    const bool bLegitGlow = Config::Glow::LegitGlow;

    CGlowObjectManager manager;

    Log("[FGlow] Started");

    while (!ShouldStop()) {
        std::array<struct iovec, 1024> g_remote = {};
        std::array<struct iovec, 1024> g_local = {};
        std::array<glow_t, 1024> g_glow = {};

        if (!m_mem.Read(Offset::Client::GlowObjectManager, &manager)) {
            LogWait("[Glow] Failed to read GlowObjectManager");
            continue;
        }

        const size_t count = manager.Count();
        const uintptr_t aGlowData = manager.Data();

        if (!m_mem.Read(aGlowData, g_glow.data(), sizeof(glow_t) * count)) {
            LogWait("[Glow] Failed to read m_GlowObjectDefinitions");
            continue;
        }

        uintptr_t localPlayer;
        if (!m_mem.Read(Offset::Client::LocalPlayer, &localPlayer)) {
            LogWait("[Glow] Failed to read local player address");
            continue;
        }

        // This one is non-fatal... no reason to output here.
        int myTeam;
        if (!m_mem.Read(localPlayer + Netvar::CBaseEntity::m_iTeamNum, &myTeam)) {
            Wait();
            continue;
        }
        
        size_t writeCount = 0;
        for (size_t i = 1; i < count; ++i) {
            if (g_glow[i].m_pEntity == 0 || g_glow[i].m_bRenderWhenOccluded)
                continue;
            CBaseEntity ent;
            if (!m_mem.Read(g_glow[i].m_pEntity, &ent))
                continue;
            if (bLegitGlow) {
                g_glow[i].m_nGlowStyle = 2;
            }
            switch(ent.m_iTeamNum) {
                case TEAM_SPEC:
                case TEAM_NONE:
                    if (!bGlowOther)
                        continue;
                    g_glow[i].SetRender(true, false);
                    g_glow[i].SetColor(clrO);
                    break;
                case TEAM_T:
                case TEAM_CT:
                    if (bGlowAllies && ent.m_iTeamNum == myTeam) {
                        if (ent.m_iHealth < 1 && !bGlowWeapons)
                            continue;
                        g_glow[i].SetRender(true, false);
                        g_glow[i].SetColor(clrA);
                    } else if (bGlowEnemies && ent.m_iTeamNum != myTeam) {
                        if (ent.m_iHealth < 1 && !bGlowWeapons)
                            continue;
                        g_glow[i].SetRender(true, false);
                        g_glow[i].SetColor(clrE);
                    } else {
                        continue;
                    }
                    break;
            }

            g_remote[writeCount].iov_base =
                ((uint8_t*)aGlowData + (sizeof(glow_t) * i)) + glow_t::WriteStart();
            g_local[writeCount].iov_base = ((uint8_t*)&g_glow[i]) + glow_t::WriteStart();
            g_remote[writeCount].iov_len = glow_t::WriteSize();
            g_local[writeCount].iov_len = glow_t::WriteSize();

            writeCount++;
        }
        m_mem.WriteMulti(g_local.data(), g_remote.data(), writeCount);
        WaitMs(2);
    }
    Log("[FGlow] Stopped");
}

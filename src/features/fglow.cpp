#include "fglow.h"

#include "../sdk/cglowobjectmanager.h"
#include "../config.h"
#include "../engine.h"
#include "../offsets.h"

typedef struct GlowObjectDefinition_t glow_t;

void FGlow::Radar(uintptr_t entPtr, int lTeam, int rTeam)
{
    if (rTeam == TEAM_T || rTeam == TEAM_CT) {
        if (rTeam != lTeam) {
            m_mem.Write(entPtr + Netvar::CBasePlayer::m_bSpotted, true);
        }
    }
}

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
        struct iovec g_local[1024];
        struct iovec g_remote[1024];
        glow_t g_glow[1024];

        memset(g_local, 0, sizeof(g_local));
        memset(g_remote, 0, sizeof(g_remote));
        memset(g_glow, 0, sizeof(g_glow));

        if (!m_mem.Read(Offset::Client::GlowObjectManager, &manager)) {
            LogWait("[Glow] Failed to read GlowObjectManager");
            continue;
        }

        const size_t count = manager.Count();
        const uintptr_t aGlowData = manager.Data();

        if (!m_mem.Read(aGlowData, g_glow, sizeof(glow_t) * count)) {
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
        for (size_t i = 0; i < count; ++i) {
            if (g_glow[i].m_pEntity == 0)
                continue;
            CBaseEntity ent;
            if (!m_mem.Read(g_glow[i].m_pEntity, &ent))
                continue;
            if (bLegitGlow) {
                g_glow[i].m_nGlowStyle = 2;
            }

            if (Config::Glow::Radar) {
                this->Radar(g_glow[i].m_pEntity, myTeam, ent.m_iTeamNum);
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
        m_mem.WriteMulti(g_local, g_remote, writeCount);
        WaitMs(2);
    }
    Log("[FGlow] Stopped");
}

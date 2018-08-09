#include "faim.h"

#include "../config.h"
#include "../engine.h"
#include "../offsets.h"

#include <cstdlib>
#include <cstdio>

void FAim::Run()
{
    const int triggerKey = Engine::StringToKeycode(Config::AimBot::TriggerKey);
    
    Log("[FAim] Started");
    
    auto& eng = Engine::GetInstance();
    
    while (!ShouldStop()) {
        uintptr_t localPlayer;
        if (!m_mem.Read(Offset::Client::LocalPlayer, &localPlayer)) {
            LogWait("[FAim] Failed to read local player address.");
            continue;
        }

        if (!eng.IsKeyDown(triggerKey) && Config::AimBot::UseTriggerKey) {
            WaitMs(20);
            continue;
        }
        if (Config::AimBot::Trigger) {
            int myTeam;
            if (!m_mem.Read(localPlayer + Netvar::CBaseEntity::m_iTeamNum, &myTeam)) {
                WaitMs(20);
                continue;
            }

            int inCrossID;
            if (!m_mem.Read(localPlayer + OFF_INCROSSHAIRID, &inCrossID)) {
                LogWait("[FAim/Trigger] Failed to read crosshair ID");
                continue;
            }

            if (inCrossID > 0 && inCrossID < 65) {
                CBaseEntity ent;
                if (!eng.GetEntityById(inCrossID, &ent)) {
                    WaitMs(20);
                    continue;
                }
                if (ent.m_iHealth > 0) {
                    if (ent.m_iTeamNum != myTeam) {
                        size_t delay = 10 + (rand() % 90);
                        eng.ForceAttack(true);
                        WaitMs(10);
                        eng.ForceAttack(false);
                        WaitMs(delay);
                    }
                }
            }
        }
        WaitMs(1);
    }
    Log("[FAim] Stopped");
}

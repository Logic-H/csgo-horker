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
        if (!eng.IsKeyDown(triggerKey) && Config::AimBot::UseTriggerKey) {
            WaitMs(20);
            continue;
        }
        if (Config::AimBot::Trigger) {
            try {
                int myTeam = eng.GetLocalPlayerTeam();
                int inCrossID = eng.GetLocalPlayerVariable<int>(OFF_INCROSSHAIRID);

                if (inCrossID > 0) {
                    CBaseEntity ent = eng.GetEntityById(inCrossID);
                    if (ent.index == -1) {
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
            } catch(std::runtime_error &e) {
                LogWait(e.what());
            }
        }
        WaitMs(1);
    }
    Log("[FAim] Stopped");
}

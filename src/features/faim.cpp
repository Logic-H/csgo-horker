#include "faim.h"

#include "../engine.h"
#include "../offsets.h"

#include <cstdlib>
#include <cstdio>

void FAim::Run()
{
    Log("[FAim] Started");
    int triggerKey = Engine::StringToKeycode("F");
    auto& eng = Engine::GetInstance();
    
    while (!ShouldStop()) {
        if (eng.IsKeyDown(triggerKey)) {
            try {
                int myTeam = eng.GetLocalPlayerTeam();
                int inCrossID = eng.GetLocalPlayerVariable<int>(OFF_INCROSSHAIRID);

                if (inCrossID > 0) {
                    CBaseEntity ent = eng.GetEntityById(inCrossID);
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
            WaitMs(1);
        } else {
            WaitMs(20);
        }
    }
    Log("[FAim] Stopped");
}

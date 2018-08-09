#include "fvisual.h"

#include "../config.h"
#include "../engine.h"
#include "../offsets.h"

void FVisual::Run()
{
    Log("[FVisual] Started");

    while (!ShouldStop()) {
        uintptr_t localPlayer;
        if (!m_mem.Read(Offset::Client::LocalPlayer, &localPlayer)) {
            LogWait("[FVisual] Failed to get local player address");
            continue;
        }

        if (Config::Visual::NoFlash) {
            float fDuration;
            if (!m_mem.Read(localPlayer + Netvar::CBasePlayer::m_flFlashDuration, &fDuration)) {
                Wait();
                continue;
            }
            if (fDuration > 0.f) {
                m_mem.Write(localPlayer + Netvar::CBasePlayer::m_flFlashDuration, 0.f);
            }
        }
        WaitMs(20);
    }
    Log("[FVisual] Stopped");
}

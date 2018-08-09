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
            float fFlashDuration;
            if (!m_mem.Read(localPlayer + OFF_FLASHDURATION, &fFlashDuration)) {
                Wait();
                continue;
            }
            if (fFlashDuration > 0.f) {
                m_mem.Write(localPlayer + OFF_FLASHDURATION, 0.f);
            }
        }
        WaitMs(20);
    }
    Log("[FVisual] Stopped");
}

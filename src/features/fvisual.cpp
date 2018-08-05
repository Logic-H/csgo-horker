#include "fvisual.h"

#include "../engine.h"
#include "../offsets.h"

void FVisual::Run()
{
    auto& eng = Engine::GetInstance();
    Log("[FVisual] Started");

    while (!ShouldStop()) {
        try {
            float fFlashDuration = eng.GetLocalPlayerVariable<float>(OFF_FLASHDURATION);
            if (fFlashDuration > 0.f) {
                eng.SetLocalPlayerVariable(OFF_FLASHDURATION, 0.f);
            }
        } catch (std::runtime_error &e) {
            LogWait(e.what());
        }
        WaitMs(20);
    }
    Log("[FVisual] Stopped");
}

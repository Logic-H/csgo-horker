#include "fvisual.h"

#include "../offsets.h"

FVisual::FVisual(MemoryManager &mem, IClient &client) : m_mem(mem), m_client(client)
{

}

void FVisual::Run()
{
    Log("Started visual thread");
    while (!ShouldStop()) {
        uintptr_t localPlayer = 0;
        if (!m_client.GetLocalPlayer(localPlayer)) {
            LogWait("[Noflash] Failed to get local player address");
            continue;
        }

        float fFlashDuration = 0.0f;

        m_mem.Read(localPlayer + OFF_FLASHDURATION, fFlashDuration);
        if (fFlashDuration > 0.f) {
            m_mem.Write(localPlayer + OFF_FLASHDURATION, 0.f);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
    Log("Stopped visual thread");
}

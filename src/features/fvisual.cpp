#include "fvisual.h"

#include "../offsets.h"

#include <cstring>
#include <iostream>
#include <thread>

FVisual::FVisual(MemoryManager &mem, IClient &client) :
        m_mem(mem), m_client(client) {

}

FVisual::~FVisual() {
    Stop();
}

void FVisual::Start() {
    Stop();
    m_stop = false;
    m_thread = std::thread(&FVisual::Run, this);
}

void FVisual::Stop() {
    m_stop = true;
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

void FVisual::WaitWarn(const std::string &warning) {
    std::cout << "WARN: " << warning << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void FVisual::Run() {
    std::cout << "Started visual thread" << std::endl;

    while (!m_stop) {
        uintptr_t localPlayer = 0;
        if (!m_client.GetLocalPlayer(localPlayer)) {
            WaitWarn("[Noflash] Failed to get local player address");
            continue;
        }

        float fFlashAlphaFromGame = 0.0f;

        m_mem.Read(localPlayer + OFF_FLASH, fFlashAlphaFromGame);
        if (fFlashAlphaFromGame > 70)
            m_mem.Write(localPlayer + OFF_FLASH, 70);

        std::this_thread::sleep_for(std::chrono::milliseconds(2));

        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
    std::cout << "Stopping visual thread" << std::endl;
}

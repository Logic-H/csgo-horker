#pragma once

#include "../interfaces/iclient.h"
#include "../memorymanager.h"

#include <thread>
class FVisual {
public:
    FVisual(MemoryManager &mem, IClient &client);
    ~FVisual();
    void Start();
    void Stop();
private:
    void Run();
    void WaitWarn(const std::string &warning);

    std::thread m_thread;
    bool m_stop = false;
    MemoryManager &m_mem;
    IClient &m_client;
};

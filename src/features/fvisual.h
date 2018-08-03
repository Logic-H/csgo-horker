#pragma once

#include "../interfaces/iclient.h"
#include "../memorymanager.h"
#include "fbase.h"

class FVisual final : public FBase {
    MemoryManager &m_mem;
    IClient &m_client;
    public:
        FVisual(MemoryManager &mem, IClient &client);
    protected:
        void Run();
};

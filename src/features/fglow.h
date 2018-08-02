#pragma once

#include "fbase.h"
#include "../interfaces/iclient.h"
#include "../memorymanager.h"

class FGlow final : public FBase {
    MemoryManager &m_mem;
    IClient &m_client;
    public:
        FGlow(MemoryManager &mem, IClient &client);
    protected:
        void Run();
};

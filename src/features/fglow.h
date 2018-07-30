#pragma once

#include "fbase.h"
#include "../interfaces/iclient.h"
#include "../memorymanager.h"

class FGlow : public FBase {
    public:
        void Run(MemoryManager &mem, IClient &client);
        void WaitWarn(const std::string &warning);
};

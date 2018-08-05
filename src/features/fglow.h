#pragma once

#include "../process.h"
#include "fbase.h"

class FGlow final : public FBase {
    Process &m_mem;
    public:
        FGlow(Process &mem) : m_mem(mem) {};
    protected:
        void Run();
};

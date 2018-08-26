#pragma once

#include "../process.h"
#include "fbase.h"

class FVisual final : public FBase {
    Process &m_mem;
    public:
        FVisual(Process &mem) : m_mem(mem) {};
    protected:
        void Run();
        size_t m_nLoopCount = 0;
};

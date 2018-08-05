#pragma once

#include "../process.h"
#include "fbase.h"
/**
 * Description: Provides features such as aimbot/triggerbot/etc.
 */

class FAim : public FBase {
    Process &m_mem;
    public:
        FAim(Process &mem) : m_mem(mem) {}
    protected:
        void Run();
    private:
        void DoTrigger();
};

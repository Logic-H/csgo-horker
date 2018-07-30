#pragma once

#include <future>

#include "../interfaces/iclient.h"
#include "../memorymanager.h"
class FBase
{
    public:
        FBase() : m_future(m_sigStop.get_future()) 
        {

        }

        FBase(FBase &&o) : m_sigStop(std::move(o.m_sigStop)), m_future(std::move(o.m_future))
        {

        }

        FBase &operator=(FBase &&o)
        {
            m_sigStop = std::move(o.m_sigStop);
            m_future = std::move(o.m_future);
            return *this;
        }

        virtual void Run(MemoryManager &mem, IClient &client) = 0;

        void operator()(MemoryManager &mem, IClient &client)
        {
            Run(mem, client);
        }

        bool StopRequested()
        {
            if (m_future.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout) {
                return false;
            }
            return true;
        }

        void Stop() {
            m_sigStop.set_value();
        }


    private:
        std::promise<void> m_sigStop;
        std::future<void> m_future;
};

#include "fbase.h"

#include <chrono>
#include <iostream>

FBase::FBase()
{
    m_stop = false;
}

FBase::~FBase()
{
    Stop();
}

void FBase::Start()
{
    Stop();
    m_stop = false;
    m_thread = std::thread(&FBase::Run, this);
}

void FBase::Stop()
{
    m_stop = true;
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

void FBase::Log(const std::string &msg)
{
    std::cout << msg << std::endl;
}

void FBase::LogWait(const std::string &msg, size_t timeout)
{
    Log(msg);
    Wait(timeout);
}

void FBase::Wait(size_t timeout)
{
    std::this_thread::sleep_for(std::chrono::seconds(timeout));
}

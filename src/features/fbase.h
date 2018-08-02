#pragma once

#include <atomic>
#include <thread>

class FBase {
    public:
        FBase();
        virtual ~FBase();
        FBase(FBase const &) = delete;
        void operator=(FBase const &) = delete;
        void Start();
        void Stop();
    protected:
        virtual void Run() = 0;
        void Log(const std::string &msg);
        void LogWait(const std::string &msg, size_t timeout = 1);
        void Wait(size_t timeout = 1);
        std::thread m_thread;
        std::atomic<bool> m_stop;
};

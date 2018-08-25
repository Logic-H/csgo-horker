#pragma once

#include "sdk/cbaseentity.h"
#include "sdk/centitylist.h"
#include "sdk/cglowobjectmanager.h"
#include "process.h"

#include <iostream>
#include <mutex>
#include <thread>

class Engine final {
    public:
        static Engine &GetInstance();
        void Update(bool force = false);
        void SetProcessManager(Process *proc);
        bool IsConnected();

        // Entity List
        const CBaseEntityList &GetEntityList() const;
        bool GetEntityById(int id, CBaseEntity* ent);
        bool GetEntityPtrById(int id, uintptr_t* out);

        // Useful things
        void ForceAttack(bool enabled);
    private:
        Engine() = default;
        ~Engine();
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(Engine&&) = delete;
        void UpdateEntityList();

        Process *m_proc = nullptr;
        CBaseEntityList m_entitylist;
        size_t m_updateTick = 0;
        std::mutex m_entitymutex;
};

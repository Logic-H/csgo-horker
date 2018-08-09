#pragma once

#include "sdk/cbaseentity.h"
#include "sdk/centitylist.h"
#include "sdk/cglowobjectmanager.h"
#include "process.h"

#include <iostream>

class Engine final {
    public:
        static Engine &GetInstance();
        void Update(bool force = false);
        void SetProcessManager(Process *proc);

        // User Input
        bool IsKeyDown(int keyCode);
        static int StringToKeycode(std::string keyString);

        // Entity List
        const CBaseEntityList &GetEntityList() const;
        bool GetEntityById(int id, CBaseEntity* ent);

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

        CGlowObjectManager m_glowmanager;
        uintptr_t m_localplayer;
};

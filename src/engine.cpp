#include "engine.h"

#include "offsets.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>

static Display *internDpy = nullptr;

Engine &Engine::GetInstance()
{
    if (internDpy == nullptr) {
        internDpy = XOpenDisplay(NULL);
    }
    if (internDpy == nullptr) {
        throw std::runtime_error("Failed to open display");
    }
    static Engine instance;
    return instance;
}

Engine::~Engine() {
    if (internDpy) {
        XCloseDisplay(internDpy);
    }
}

int Engine::StringToKeycode(std::string keyString)
{
    KeySym ks = XStringToKeysym(keyString.data());
    return XKeysymToKeycode(internDpy, ks);
}

bool Engine::IsKeyDown(int keyCode)
{
    char keys[32];
    XQueryKeymap(internDpy, keys);
    if (keys[keyCode/8] & (1<<(keyCode%8))) {
        return true;
    }
    return false;
}

void Engine::SetProcessManager(Process *proc)
{
    m_proc = proc;
}

bool Engine::GetEntityById(int id, CBaseEntity* ent)
{
    std::lock_guard<std::mutex> guard(m_entitymutex);
    uintptr_t aEntity = m_entitylist.GetEntityPtrById(id);
    if (aEntity != 0) {
        if (m_proc->Read(aEntity, ent)) {
            return true;
        }
    }
    return false;
}

bool Engine::GetEntityPtrById(int id, uintptr_t* out)
{
    std::lock_guard<std::mutex> guard(m_entitymutex);
    *out = m_entitylist.GetEntityPtrById(id);
    return (*out != 0);
}

void Engine::UpdateEntityList()
{
    std::lock_guard<std::mutex> guard(m_entitymutex);
    m_entitylist.Reset();
    CEntInfo info;
    m_proc->Read(Offset::Client::EntityList, &info);
    if (info.m_pPrev != NULL) {
        std::cout << "Skipped elements!\n";
    }
    while (info.m_pNext != NULL) {
        CBaseEntity ent;
        m_proc->Read(info.m_pEntity, &ent);
        m_entitylist.AddEntInfo(ent.index, info);
        m_proc->Read(info.m_pNext, &info);
    }
}

void Engine::Update(bool force)
{
    if (m_proc == nullptr) {
        return;
    }
    m_updateTick++;

    if (force || m_updateTick >= 20) {
        m_updateTick = 0;
        UpdateEntityList();
        m_proc->Read(Offset::Client::LocalPlayer, &m_localplayer);
    }
}

void Engine::ForceAttack(bool enabled)
{
    if (!m_proc->Write(Offset::Client::ForceAttack, enabled)) {
        throw std::runtime_error("Failed to set ForceAttack value.");
    }
}

#include "memorymanager.h"
#include "offsets.h"
#include "interfaces/iclient.h"
#include "interfaces/iengine.h"
#include "sdk/cbaseentity.h"
#include "sdk/cglowobjectmanager.h"
#include "sdk/types.h"

#include <chrono>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <thread>

#include <signal.h>

bool shouldQuit = false;

#define LOG(X) std::cout << X << std::flush

void doGlow(MemoryManager *mem, IClient *client, IEngine *engine)
{
    while (!shouldQuit) {
        if (!engine->IsConnected()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }
        constexpr size_t szGlowDef = sizeof(GlowObjectDefinition_t);

        struct iovec g_remote[1024], g_local[1024];
        struct GlowObjectDefinition_t g_glow[1024];
        
        memset(g_glow, 0, sizeof(g_glow));
        memset(g_local, 0, sizeof(g_local));
        memset(g_remote, 0, sizeof(g_remote));

        CGlowObjectManager manager = client->GetGlowObjectManager();

        size_t count = manager.m_GlowObjectDefinitions.Count();
        void* data_ptr = manager.m_GlowObjectDefinitions.Data();
        
        if (!mem->Read(data_ptr, g_glow, szGlowDef * count)) {
            std::cout << "Failed to read m_GlowObjectDefinitions\n";
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
        
        CBaseEntity lpEnt = client->GetLocalPlayerEntity();
        
        size_t writeCount = 0;
        for (unsigned int i = 0; i < count; ++i) {
            if (g_glow[i].m_pEntity != NULL) {
                CBaseEntity ent;
                if (mem->Read(g_glow[i].m_pEntity, ent)) {
                    if (ent.GetTeam() < TEAM_T) {
                        g_glow[i].m_bRenderWhenOccluded = 1;
                        g_glow[i].m_bRenderWhenUnoccluded = 0;
                        continue;
                    }

                    if (ent.GetHealth() < 1) {
                        continue;
                    }

                    if (g_glow[i].m_bRenderWhenOccluded == 1) {
                        continue;
                    }
                    
                    g_glow[i].m_bRenderWhenOccluded = 1;
                    g_glow[i].m_bRenderWhenUnoccluded = 0;
                    
                    if (ent.GetTeam() == TEAM_T || ent.GetTeam() == TEAM_CT) {
                        if (ent.GetTeam() == lpEnt.GetTeam()) {
                            g_glow[i].set(0.f, 0.8f, 0.8f, 0.8f);
                        } else {
                            g_glow[i].set(0.8f, 0.8f, 0.f, 0.8f);
                        }
                    }
                }
            }
            size_t bytesToCutOffEnd = szGlowDef - g_glow[i].writeEnd();
            size_t bytesToCutOffBegin = (size_t)g_glow[i].writeStart();
            size_t totalWriteSize = (szGlowDef - (bytesToCutOffBegin + bytesToCutOffEnd));
            g_remote[writeCount].iov_base = 
                ((uint8_t*)data_ptr + (szGlowDef * i)) + bytesToCutOffBegin;
            g_local[writeCount].iov_base = ((uint8_t*)&g_glow[i]) + bytesToCutOffBegin;
            g_remote[writeCount].iov_len = g_local[writeCount].iov_len = totalWriteSize;

            writeCount++;
        }
        mem->WriteMulti(g_local, g_remote, writeCount);
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}

void exitHandle(int s)
{
    std::cout << "\nCaught signal: " << s << '\n';
    shouldQuit = true;
}

int main()
{
    LOG("Attaching interrupt signal handler...");
    struct sigaction ccHandle;
    ccHandle.sa_handler = exitHandle;
    sigemptyset(&ccHandle.sa_mask);
    ccHandle.sa_flags = 0;
    sigaction(SIGINT, &ccHandle, NULL);
    LOG("Done.\n");

    MemoryManager mem("csgo_linux64");
    LOG("Waiting for process...");
    while (!mem.Attach() && !shouldQuit) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    LOG("Done.\n");
    LOG("Waiting for client library...");
    while (!shouldQuit) {
        mem.ParseModules();
        if (mem.GetModuleStart("client_panorama_client.so") != 0) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    if (shouldQuit) {
        return 0;
    }

    LOG("Done.\n");
    LOG("Initializing client interface...");
    IClient client(mem);
    LOG("Done.\n");
    LOG("Initializing engine interface...");
    IEngine engine(mem);
    LOG("Done.\n");

    LOG("Starting glow thread...");
    std::thread tGlow(doGlow, &mem, &client, &engine);
    LOG("Done.\n");
    LOG("> Main Start <\n");
    while (!shouldQuit) {
        if (!mem.IsValid()) {
            shouldQuit = true;
            LOG("Lost connection to process... Exiting\n");
            break;
        }
        engine.Update();
        if (engine.IsConnected()) {
            client.Update();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    LOG("Stopping glow...");
    tGlow.join();
    LOG("Done.\n");
    return 0;
}

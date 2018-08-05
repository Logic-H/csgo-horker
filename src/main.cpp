#include "features/faim.h"
#include "features/fglow.h"
#include "features/fvisual.h"
#include "sdk/cbaseentity.h"
#include "sdk/cglowobjectmanager.h"
#include "sdk/types.h"

#include "engine.h"
#include "globals.h"
#include "helper.h"
#include "offsets.h"
#include "process.h"

#include <chrono>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <thread>

#include <signal.h>
#include <unistd.h>

#define LOG(X) std::cout << X << std::flush

bool shouldQuit = false;

void exitHandle(int s)
{
    std::cout << "\nCaught signal: " << s << '\n';
    shouldQuit = true;
}

void connectSignals(struct sigaction &handle)
{
    handle.sa_handler = exitHandle;
    sigemptyset(&handle.sa_mask);
    handle.sa_flags = 0;
    sigaction(SIGINT, &handle, NULL);
    sigaction(SIGQUIT, &handle, NULL);
}

int main()
{
    if (getuid() != 0) {
        LOG("This program must be ran as root.\n");
        return 0;
    }
    
    struct sigaction ccHandle;
    connectSignals(ccHandle);
    
    Process proc(PROCESS_NAME);
    
    LOG("Waiting for process...");
    
    while (!proc.Attach() && !shouldQuit) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    LOG("Done.\nWaiting for client and engine library...");

    while (!shouldQuit) {
        proc.ParseModules();
        if (proc.HasModule(CLIENT_SO) && proc.HasModule(ENGINE_SO)) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    if (shouldQuit) {
        return 0;
    }

    LOG("Done.\n");
    FindOffsets(proc);
    PrintOffsets(proc);

    auto& eng = Engine::GetInstance();
    eng.SetProcessManager(&proc);
    eng.Update(true);
    // Feature handlers
    FAim faim(proc);
    FGlow fglow(proc);
    FVisual fvisual(proc);

    const uintptr_t aIsConnected = Offset::Engine::IsConnected;
    bool isConnected = false;

    while (!shouldQuit) {
        if (!proc.IsValid() || !proc.Read(aIsConnected, isConnected)) {
            shouldQuit = true;
            LOG("Lost connection to process... Exiting.\n");
            break;
        }

        // ### BEGIN MENU HACKS ###
        
        // ### END MENU HACKS ###
        
        // ### BEGIN IN-GAME HACKS ###
        if (isConnected) {
            faim.Start();
            fglow.Start();
            fvisual.Start();

            while (isConnected && !shouldQuit) {
                proc.Read(aIsConnected, isConnected);
                eng.Update();
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }

            faim.Stop();
            fglow.Stop();
            fvisual.Stop();
        }
        // ### END IN-GAME HACKS ###
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}

#include "features/faim.h"
#include "features/fglow.h"
#include "features/fvisual.h"
#include "sdk/cbaseentity.h"
#include "sdk/cglowobjectmanager.h"
#include "sdk/types.h"

#include "config.h"
#include "engine.h"
#include "globals.h"
#include "helper.h"
#include "hwctrl.h"
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

void exitHandle(int)
{
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

    if (!Helper::Init()) {
        LOG("Failed to initialize input handling.\n");
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

    ReadConfig("config.ini");

    LOG("Done.\n");
    Signatures::Find(proc);
    Signatures::Print(proc);

    auto& eng = Engine::GetInstance();
    eng.SetProcessManager(&proc);
    eng.Update(true);
    // Feature handlers
    FAim faim(proc);
    FGlow fglow(proc);
    FVisual fvisual(proc);

    HWCtrl::SetContrast(Config::Visual::Contrast);

    while (!shouldQuit) {
        if (!proc.IsValid()) {
            shouldQuit = true;
            LOG("Lost connection to process... Exiting.\n");
            break;
        }

        // ### BEGIN MENU HACKS ###
        
        // ### END MENU HACKS ###
        
        // ### BEGIN IN-GAME HACKS ###
        if (eng.IsConnected()) {
            faim.Start();
            fglow.Start();
            fvisual.Start();

            while (eng.IsConnected() && !shouldQuit) {
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
    Helper::Finalize();
    HWCtrl::SetContrast(0);
    return 0;
}

#pragma once

#include <string>

class Helper {
    public:
        static bool Init();
        static void Finalize();
        static bool IsKeyDown(int key);
        static bool IsMouseDown(unsigned int mask);
        static int StringToKeycode(std::string keyString);
        static unsigned int StringToMouseMask(std::string buttonString);
};


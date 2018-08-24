#include "helper.h"
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>

static Display* m_dpy = nullptr;
bool Helper::Init()
{
    if (m_dpy == nullptr) {
        m_dpy = XOpenDisplay(NULL);
    }
    return (m_dpy != NULL);
}

void Helper::Finalize()
{
    if (m_dpy != NULL) {
        XCloseDisplay(m_dpy);
    }
}

bool Helper::IsKeyDown(int key)
{
    char keys[32];
    XQueryKeymap(m_dpy, keys);
    return (keys[key/8] & (1<<(key%8)));
}

bool Helper::IsMouseDown(unsigned int buttonMask)
{
    Window root = RootWindow(m_dpy, 0);
    Window rootWindow, childWindow;
    int rootX, rootY, winX, winY;
    unsigned int pointerMask;
    bool result = XQueryPointer(m_dpy, root,
            &rootWindow, &childWindow,
            &rootX, &rootY,
            &winX, &winY,
            &pointerMask);
    return (result && (pointerMask & buttonMask));
}

int Helper::StringToKeycode(std::string keyString)
{
    KeySym ks = XStringToKeysym(keyString.data());
    return XKeysymToKeycode((Display*)m_dpy, ks);
}

unsigned int Helper::StringToMouseMask(std::string buttonString)
{
    if (buttonString == "Mouse1")
        return Button1Mask;
    if (buttonString == "Mouse2")
        return Button2Mask;
    if (buttonString == "Mouse3")
        return Button3Mask;
    return 0;
}

#include "hwctrl.h"

#include "defines.h"

#include <X11/Xlib.h>
#ifdef USE_NVIDIA
#include <NVCtrl.h>
#include <NVCtrlLib.h>
#endif

#ifdef USE_NVIDIA

static inline int GetNvXScreen(Display *dpy)
{
    int defaultScreen, screen;

    defaultScreen = DefaultScreen(dpy);

    if (XNVCTRLIsNvScreen(dpy, defaultScreen)) {
        return defaultScreen;
    }

    for (screen = 0; screen < ScreenCount(dpy); screen++) {
        if (XNVCTRLIsNvScreen(dpy, screen)) {
            return screen;
        }
    }

    return -1;
}

int HWCtrl::SetContrast(int value)
{
    Display* dpy = NULL;
    Bool ret;
    int screen;
    NVCTRLAttributeValidValuesRec valid_values;
    int* data;
    int len;

    dpy = XOpenDisplay(NULL);
    if (!dpy) {
        return 1; // Display open failure
    }

    screen = GetNvXScreen(dpy);

    ret = XNVCTRLQueryTargetBinaryData(dpy,
            NV_CTRL_TARGET_TYPE_X_SCREEN,
            screen,
            0,
            NV_CTRL_BINARY_DATA_DISPLAYS_ENABLED_ON_XSCREEN,
            (unsigned char**)&data,
            &len);

    if (!ret) {
        XCloseDisplay(dpy);
        return 2; // Display list failure
    }

    for (int i = 0; i <= data[0]; i++) {
        int dpyId = data[i];

        ret = XNVCTRLQueryValidTargetAttributeValues(dpy,
                NV_CTRL_TARGET_TYPE_DISPLAY,
                dpyId,
                0,
                NV_CTRL_DIGITAL_VIBRANCE,
                &valid_values);
        if (!ret) {
            XCloseDisplay(dpy);
            return 3; // Failed to query valid values
        }

        if (valid_values.type != ATTRIBUTE_TYPE_RANGE) {
            XCloseDisplay(dpy);
            return 4; // valid values not range type
        }

        if (value > valid_values.u.range.max) {
            value = valid_values.u.range.max;
        } else if (value < valid_values.u.range.min) {
            value = valid_values.u.range.min;
        }

        XNVCTRLSetTargetAttribute(dpy,
                NV_CTRL_TARGET_TYPE_DISPLAY,
                dpyId,
                0,
                NV_CTRL_DIGITAL_VIBRANCE,
                value);

        XFlush(dpy);

    }

    XCloseDisplay(dpy);
    return 0;
}

#else
int HWCtrl::SetContrast(int /*value*/)
{
    return 0;
}
#endif

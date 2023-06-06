
#include "context.h"


HWND global_window_handle;

int IsInFocus()
{
    HWND focused = GetForegroundWindow();

    return (focused == global_window_handle);
}
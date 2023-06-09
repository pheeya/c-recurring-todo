// somehow overwrote the calculus project with this project 🤔

#include <windows.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <profileapi.h>

#include "events/events.h"
#include "events/keyboard/keyboardEvent.h"
#include "renderer.h"
#include "Task.h"
#include "context.h"
struct
{
    int width;
    int height;
    uint32_t *pixels;
} frame = {0};

#if RAND_MAX == 32767
#define Rand32() ((rand() << 16) + (rand() << 1) + (rand() & 1))
#else
#define Rand32() rand()
#endif

bool quit = false;

LRESULT CALLBACK WindowProcessMessage(HWND, UINT, WPARAM, LPARAM);

static BITMAPINFO frame_bitmap_info;
static HBITMAP frame_bitmap = 0;
static HDC frame_device_context = 0;

static void AppOnUpdate();
void RegisterAppListeners()
{
    RegisterEventListener(RendererUpdateListener, EventUpdate);
    RegisterEventListener(PaintEventListener, EventPaint);
    RegisterEventListener(KeyboardEventListener, EventKeyboard);
    RegisterEventListener(TaskStartEventListener, EventAppStart);
    RegisterEventListener(TaskUpdateEventListener, EventUpdate);
}

long long ApplicationElapsedTime;
long long ApplicationStartTime;
int ApplicationFPS;

static BOOL s_use_qpc;
static LARGE_INTEGER s_frequency;

long long milliseconds_now()
{
    s_use_qpc = QueryPerformanceFrequency(&s_frequency);
    if (s_use_qpc)
    {
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        return (1000LL * now.QuadPart) / s_frequency.QuadPart;
    }
    else
    {
        return GetTickCount();
    }
}


int WINAPI WinMain(HINSTANCE _hinstance, HINSTANCE _hprevInstance, PSTR _pCmdLine, int _nCmdShow)
{

    ApplicationStartTime = milliseconds_now();

    RegisterAppListeners();
    const wchar_t window_class_name[] = L"Window Class";
    WNDCLASS window_class = {0};

    window_class.lpszClassName = (PCSTR)window_class_name;
    window_class.lpfnWndProc = WindowProcessMessage;
    window_class.hInstance = _hinstance;

    RegisterClass(&window_class);

    frame_bitmap_info.bmiHeader.biSize = sizeof(frame_bitmap_info.bmiHeader);
    frame_bitmap_info.bmiHeader.biPlanes = 1;

    // bytes rgb and one 'filler' byte, whatever that is
    frame_bitmap_info.bmiHeader.biBitCount = 3 * 8 + 1 * 8;
    frame_bitmap_info.bmiHeader.biCompression = BI_RGB;
    frame_device_context = CreateCompatibleDC(0);

    global_window_handle = CreateWindow((PCSTR)window_class_name, "Drawing Pixels", WS_POPUP | WS_VISIBLE,
                                 1600, 20, 300, 400, NULL, NULL, _hinstance, NULL);

    if (global_window_handle == NULL)
    {
        return -1;
    };
    Event _appstart = {EventAppStart, NULL};
    DispatchEvent(&_appstart);

    HCURSOR cursor = LoadCursor(NULL, IDC_ARROW);
    SetCursor(cursor);
    SetWindowPos(global_window_handle, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    while (!quit)
    {
        ApplicationFPS = 1 / (((milliseconds_now() - ApplicationStartTime) - ApplicationElapsedTime) / (double)1000);
        ApplicationElapsedTime = milliseconds_now() - ApplicationStartTime;
        static MSG message = {0};
        SetCursor(cursor);
        while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&message);
        }

        if (FRAME_DIRTY)
        {
            InvalidateRect(global_window_handle, NULL, FALSE);
            UpdateWindow(global_window_handle);
            FRAME_DIRTY = 0;
        }

        Event UpdateEvent = {EventUpdate, NULL};
        DispatchEvent(&UpdateEvent);
        AppOnUpdate();
    }

    return 0;
}

LRESULT CALLBACK WindowProcessMessage(HWND _window_handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_QUIT:
    case WM_DESTROY:
    {
        quit = true;
    }
    break;

    case WM_KEYDOWN:
    {
    }
    break;

    case WM_PAINT:
    {
        static PAINTSTRUCT paint;

        static HDC device_context;
        device_context = BeginPaint(_window_handle, &paint);

        PaintArgs pargs = {_window_handle, device_context};
        Event paintEvent = {EventPaint, &pargs};
        DispatchEvent(&paintEvent);
        EndPaint(_window_handle, &paint);
    }
    break;

    case WM_SIZE:
    {
        frame_bitmap_info.bmiHeader.biWidth = LOWORD(lParam);
        frame_bitmap_info.bmiHeader.biHeight = HIWORD(lParam);

        if (frame_bitmap)
            DeleteObject(frame_bitmap);
        frame_bitmap = CreateDIBSection(NULL, &frame_bitmap_info, DIB_RGB_COLORS, (void *)&frame.pixels, 0, 0);
        SelectObject(frame_device_context, frame_bitmap);

        frame.width = LOWORD(lParam);
        frame.height = HIWORD(lParam);
    }
    break;

    default:
    { // Message not handled; pass on to default message handling function
        return DefWindowProc(_window_handle, message, wParam, lParam);
    }
    break;
    }
    return 0;
}

// static bool td_window_hidden = false;
// static bool canToggleWindow = true;
static void AppOnUpdate()
{
    // if (GetKeyDown(TD_KEY_LSHIFT) && GetKeyDown(TD_KEY_LCTRL) && canToggleWindow)
    // {
    //     if (td_window_hidden)
    //     {

    //         SetWindowPos(window_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    //         td_window_hidden = false;
    //     }
    //     else
    //     {
    //         SetWindowPos(window_handle, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    //         td_window_hidden = true;
    //     }
    //     canToggleWindow = false;
    // }

    // if (!(GetKeyDown(TD_KEY_LSHIFT) && GetKeyDown(TD_KEY_LCTRL)))
    // {
    //     canToggleWindow = true;
    // }

    // if (GetKeyDown(TD_KEY_ESC))
    // {
    //     DestroyWindow(window_handle);
    // }
}

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

void RegisterAppListeners()
{
    RegisterEventListener(UpdateEventListener, EventUpdate);
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
    static HWND window_handle;
    window_handle = CreateWindow((PCSTR)window_class_name, "Drawing Pixels", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                 500, 10, 1000, 1000, NULL, NULL, _hinstance, NULL);

    if (window_handle == NULL)
    {
        return -1;
    };
    Event _appstart = {EventAppStart, NULL};
    DispatchEvent(&_appstart);
    while (!quit)
    {
        ApplicationFPS = 1 / (((milliseconds_now() - ApplicationStartTime) - ApplicationElapsedTime) / (double)1000);
        ApplicationElapsedTime = milliseconds_now() - ApplicationStartTime;
        static MSG message = {0};
        while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&message);
        }

        if (FRAME_DIRTY)
        {
            InvalidateRect(window_handle, NULL, FALSE);
            UpdateWindow(window_handle);
            FRAME_DIRTY = 0;
        }

        Event UpdateEvent = {EventUpdate, NULL};
        DispatchEvent(&UpdateEvent);
    }

    return 0;
}

LRESULT CALLBACK WindowProcessMessage(HWND window_handle, UINT message, WPARAM wParam, LPARAM lParam)
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

        enum KeyCode code = TD_KEY_ARROW_UP;

        switch (wParam)
        {
        case VK_DOWN:
        {
            code = TD_KEY_ARROW_DOWN;
            break;
        }
        case VK_UP:
        {
            code = TD_KEY_ARROW_UP;
            break;
        }
        case VK_LEFT:
        {
            code = TD_KEY_ARROW_LEFT;
            break;
        }

        case VK_RIGHT:
        {
            code = TD_KEY_ARROW_RIGHT;
            break;
        }
        case VK_SPACE:
        {
            code = TD_KEY_SPACE;
            break;
        }
        };

        KeyEventData data = {TD_Pressed, code};

        Event keydown = {
            EventKeyboard,
            &data};

        DispatchEvent(&keydown);
    }
    break;

    case WM_PAINT:
    {
        static PAINTSTRUCT paint;

        static HDC device_context;
        device_context = BeginPaint(window_handle, &paint);

        PaintArgs pargs = {window_handle, device_context};
        Event paintEvent = {EventPaint, &pargs};
        DispatchEvent(&paintEvent);
        EndPaint(window_handle, &paint);
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
        return DefWindowProc(window_handle, message, wParam, lParam);
    }
    break;
    }
    return 0;
}
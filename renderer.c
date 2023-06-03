#include "Renderer.h"
#include <stdio.h>
#include <time.h>
#include "./Task.h"
void UpdateEventListener(Event *_event)
{
    OnUpdate();
}

void OnUpdate()
{
}

void PaintEventListener(Event *_event)
{
    PaintArgs *args = (PaintArgs *)_event->data;
    OnPaint(*args);
}
void OnPaint(PaintArgs _args)
{

    PAINTSTRUCT ps;

    HBRUSH hBrush = CreateSolidBrush(RGB(50, 50, 50));
    RECT bg;
    GetClientRect(_args.handle, &bg);
    FillRect(_args.context, &bg, hBrush);
    DeleteObject(hBrush);

    HFONT hFontOriginal, hFont1, hFont2, hFont3;

    hFont1 = CreateFont(26, 0, 0, 0, FW_HEAVY, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Calibri"));

    hFont2 = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Calibri"));
    hFontOriginal = (HFONT)SelectObject(_args.context, hFont1);

    SetBkMode(_args.context, TRANSPARENT);
    SetTextColor(_args.context, RGB(180, 180, 180));
    TextOut(_args.context, 150, 150, "Daily", 5);

    int num = 0;
    Task *daily = GetTasks("daily.txt", &num);

    int margin = 30;

    SelectObject(_args.context, hFont2);
    for (int i = 0; i < num; i++)
    {
        // add margin additionally to skip first line

        if (daily[i].completed)
        {
            SetTextColor(_args.context, RGB(10, 170, 10));
        }
        else
        {
            SetTextColor(_args.context, RGB(180, 180, 180));
        }

        TextOut(_args.context, 150, 150 + (40) + (margin * i), daily[i].description, sizeof(daily[i].description));
    }
}

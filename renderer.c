#include "Renderer.h"
#include <stdio.h>
#include <time.h>
#include "./Task.h"

int FRAME_DIRTY = 0;
static int WasInFocus;
static COLORREF bg_color = RGB(50, 50, 50);
void RendererUpdateListener(Event *_event)
{
    OnUpdate();
}

static void OnUpdate()
{
    if (IsInFocus() && !WasInFocus)
    {
        bg_color = RGB(10, 10, 10);
        FRAME_DIRTY = true;
        WasInFocus = true;
    }
    else if (!IsInFocus() && WasInFocus)
    {
        bg_color = RGB(30, 30, 30);
        FRAME_DIRTY = true;
        WasInFocus = false;
    }
}

void PaintEventListener(Event *_event)
{
    PaintArgs *args = (PaintArgs *)_event->data;
    OnPaint(*args);
}

void OnPaint(PaintArgs _args)
{

    PAINTSTRUCT ps;

    HBRUSH hBrush = CreateSolidBrush(bg_color);
    RECT bg;
    GetClientRect(_args.handle, &bg);
    FillRect(_args.context, &bg, hBrush);
    DeleteObject(hBrush);

    HFONT hFontOriginal, hFont1, hFont2, hFont3;

    hFont1 = CreateFont(26, 0, 0, 0, FW_HEAVY, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Calibri"));

    hFont2 = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Calibri"));
    hFont3 = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, TRUE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Calibri"));
    hFontOriginal = (HFONT)SelectObject(_args.context, hFont1);

    SetBkMode(_args.context, TRANSPARENT);
    SetTextColor(_args.context, RGB(180, 180, 180));

    int pageMarginX = 50;
    int pageMarginY = 50;

    TextOut(_args.context, pageMarginX, pageMarginY, "Daily", 5);

    int margin = 30;

    SelectObject(_args.context, hFont2);

    Task *daily = GetDailyTasks();

    for (int i = 0; i < td_num_daily_tasks; i++)
    {
        // add margin additionally to skip first line

        if (i == td_selected)
        {
            SelectObject(_args.context, hFont3);
        }
        else
        {
            SelectObject(_args.context, hFont2);
        }
        if (daily[i].completed)
        {
            SetTextColor(_args.context, RGB(10, 170, 10));
        }
        else
        {
            SetTextColor(_args.context, RGB(180, 180, 180));
        }

        TextOut(_args.context, pageMarginX, pageMarginY + (40) + (margin * i), daily[i].description, sizeof(daily[i].description));
    }
}

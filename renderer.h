#ifndef RENDERER
#define RENDERER

#include <windows.h>
#include "events/events.h"



typedef struct PaintArgs {
HWND handle;
HDC context;
} PaintArgs;


void UpdateEventListener(Event *_event);

void OnUpdate();



void PaintEventListener(Event *_event);

void OnPaint(PaintArgs _args);

#endif
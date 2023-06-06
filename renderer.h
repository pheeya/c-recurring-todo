#ifndef RENDERER
#define RENDERER

#include <windows.h>
#include "events/events.h"

extern int FRAME_DIRTY;


typedef struct PaintArgs {
HWND handle;
HDC context;
} PaintArgs;


void RendererUpdateListener(Event *_event);

static void OnUpdate();



void PaintEventListener(Event *_event);

void OnPaint(PaintArgs _args);


#endif
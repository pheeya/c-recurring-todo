#include "keyboardEvent.h"

void KeyboardEventListener(Event *_event)
{
    KeyEventData *_data = (KeyEventData *)_event->data;

    OnKey(*_data);
}

void OnKey(KeyEventData _data)
{
    printf("%d", (int)_data.key);
}
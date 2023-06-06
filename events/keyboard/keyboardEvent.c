#include "keyboardEvent.h"

void KeyboardEventListener(Event *_event)
{
    KeyEventData *_data = (KeyEventData *)_event->data;

    OnKey(*_data);
}

void OnKey(KeyEventData _data)
{

    // if (_data.key == TD_KEY_ARROW_DOWN)
    //     printf("%d", (int)_data.key);
}

int GetKeyDown(enum KeyCode _code)
{
    if (!IsInFocus())
        return 0;

        
    switch (_code)
    {
    case TD_KEY_ARROW_DOWN:
    {

        return (GetKeyState(VK_DOWN) & 0x80000);
    }
    case TD_KEY_ARROW_UP:
    {
        return (GetKeyState(VK_UP) & 0x80000);
    }
    case TD_KEY_SPACE:
    {
        return (GetKeyState(VK_SPACE) & 0x80000);
    }
    case TD_KEY_LCTRL:
    {
        return (GetKeyState(VK_LCONTROL) & 0x80000);
    }
    case TD_KEY_LSHIFT:
    {
        return (GetKeyState(VK_LSHIFT) & 0x80000);
    }
    case TD_KEY_ESC:
    {
        return (GetKeyState(VK_ESCAPE) & 0x80000);
    }
    default:
    {
        return 0;
    }
    }
}

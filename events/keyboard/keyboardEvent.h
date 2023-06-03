#ifndef TD_KEYBOARD
#define TD_KEYBOARD

#include <windows.h>
#include "../../events/events.h"
#include "stdlib.h"
#include <stdio.h>



// TD is for to do, to easily identify app consts

enum KeyCode {
TD_KEY_ARROW_UP=0,
TD_KEY_ARROW_DOWN,
TD_KEY_ARROW_RIGHT,
TD_KEY_ARROW_LEFT,
TD_KEY_SPACE,
};



enum KeyState
{
    TD_Pressed = 0,
    TD_Released,
    TD_Holding
};



typedef struct
{
    enum KeyState state;
    enum KeyCode key;

} KeyEventData;






void KeyboardEventListener(Event *_event);

void OnKey(KeyEventData _data);


#endif


#include <stdlib.h>

enum KeyState
{
    Pressed = 0,
    Released
};





typedef struct
{
    enum KeyState state;

} KeyEventData;
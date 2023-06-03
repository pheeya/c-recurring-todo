#include <stdint.h>

enum MouseInput
{
    LEFT = 0b1,
    MIDDLE = 0b10,
    RIGHT = 0b100,
    X1 = 0b1000,
    X2 = 0b10000
};



typedef struct
{
    int x, y;
    uint8_t buttons;
} Mouse;


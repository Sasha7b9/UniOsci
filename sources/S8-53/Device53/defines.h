#pragma once

#define S8_53

#include <stm32f2xx_hal.h>
#include "commonDefines.h"

#define NUM_VER "1.3"

// Тип цвета
typedef enum
{
    COLOR_BLACK             = 0x00,
    COLOR_WHITE             = 0x01,
    COLOR_GRID              = 0x02,
    COLOR_DATA_A            = 0x03,
    COLOR_DATA_B            = 0x04,
    COLOR_MENU_FIELD        = 0x05,
    COLOR_MENU_TITLE        = 0x06,
    COLOR_MENU_TITLE_DARK   = 0x07,
    COLOR_MENU_TITLE_BRIGHT = 0x08,
    COLOR_MENU_ITEM         = 0x09,
    COLOR_MENU_ITEM_DARK    = 0x0a,
    COLOR_MENU_ITEM_BRIGHT  = 0x0b,
    COLOR_MENU_SHADOW       = 0x0c,
    COLOR_EMPTY             = 0x0d,
    COLOR_CURSORS_A         = 0x0e,
    COLOR_CURSORS_B         = 0x0f,
    NUM_COLORS,
    COLOR_FLASH_10,
    COLOR_FLASH_01,
    SET_INVERSE
} Color;


#define DEBUG

typedef struct
{
    int16 rel;
    float abs;
} StructRelAbs;

#include "Globals.h"

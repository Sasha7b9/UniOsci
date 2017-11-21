#pragma once
#include <stm32f4xx.h>
#include <stm32f4xx_hal.h>

#define KORNET

#include "commonDefines.h"


enum Color
{
    COLOR_BACK = 0x00,
    COLOR_FILL = 0x01
};


#define TIME_UPDATE_KEYBOARD 2   ///< Время между опросами клавиатуры

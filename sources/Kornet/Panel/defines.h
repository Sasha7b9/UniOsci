#pragma once
#include <stm32f4xx.h>
#include <stm32f4xx_hal.h>


typedef unsigned int        uint;
typedef unsigned short int  uint16;
typedef signed short int    int16;
typedef unsigned char       uint8;
typedef unsigned char       uchar;
typedef signed char         int8;

#ifdef WIN32
typedef unsigned int uint32_t;
#endif

#define ERROR_HANDLER() _Error_Handler(__FILE__, __LINE__);

enum Color
{
    COLOR_BACK = 0x00,
    COLOR_FILL = 0x01
};


#define TIME_UPDATE_KEYBOARD 2   ///< Время между опросами клавиатуры


#ifdef __cplusplus
extern "C" {
#endif
void _Error_Handler(char *, int);
#ifdef __cplusplus
}
#endif

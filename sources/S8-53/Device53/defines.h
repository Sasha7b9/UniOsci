#pragma once
#include <stm32f2xx_hal.h>

#define S8_53

#include "commonDefines.h"

#define NUM_VER "1.3"


typedef signed      char        int8;
typedef signed      short   int int16;
typedef unsigned    char        uint8;
typedef unsigned    char        uint8_t;
typedef unsigned    short   int uint16;
typedef unsigned    short   int uint16_t;
typedef unsigned    int         uint;
typedef unsigned long long int  uint64;
typedef unsigned    int         uint32_t;
typedef unsigned    char        uchar;

#ifndef WIN32
typedef signed      char        int8_t;
#else
#define __ALIGN_BEGIN
#define __ALIGN_END
#define __weak
#endif


// “ип цвета
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


#define __IO volatile

typedef void    (*pFuncVV)();               // ”казатель на функцию, ничего не принимающую и ничего не возвращающую.
typedef void    (*pFuncVpV)(void*);             // ”казатель на функцию, принимающую указатель на void и ничего не возвращающую.
typedef bool    (*pFuncBV)();
typedef void    (*pFuncVB)(bool);
typedef void    (*pFuncVI)(int);
typedef void    (*pFuncVII)(int, int);
typedef void    (*pFuncVI16)(int16);
typedef bool    (*pFuncBU8)(uint8);
typedef void    (*pFuncVI16pI16pI16)(int16, int16*, int16*);
typedef float   (*pFuncFU8)(uint8);
typedef char*   (*pFuncCFB)(float, bool);
typedef char*   (*pFuncCFBC)(float, bool, char*);
typedef void    (*pFuncpU8)(uint8*);
typedef void    (*pFuncVpVIIB)(void*, int, int, bool);

// ќбъединение размером 32 бита
typedef union
{
    uint  word;
    uint8 byte[4]; //-V112
} BitSet32;

typedef struct
{
    int16 rel;
    float abs;
} StructRelAbs;

#define HARDWARE_ERROR HardwareErrorHandler(__FILE__, __FUNCTION__, __LINE__);
void HardwareErrorHandler(const char *file, const char *function, int line);

#include "Globals.h"

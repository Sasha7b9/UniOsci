#pragma once
#include <stm32f4xx_hal.h>
#include <limits.h>

#define S8_54

#include "commonDefines.h"

#define NUM_VER "1.3"

#define INTERRUPT_P2P   // Если определено, то для чтения точек используется прерывание
 
#define OLD_RECORDER

#ifdef WIN32
#define __attribute__(x)
#endif


#define DEBUG


#define __IO volatile
    
/*
typedef void    (*pFuncVV)(void);               ///< Указатель на функцию, ничего не принимающую и ничего не возвращающую.
typedef void    (*pFuncVpV)(void *);            ///< Указатель на функцию, принимающую указатель на void и ничего не возвращающую.
typedef bool    (*pFuncBV)(void);
typedef void    (*pFuncVB)(bool);
typedef void    (*pFuncVI)(int);
typedef void    (*pFuncVII)(int, int);
typedef void    (*pFuncVI16)(int16);
typedef bool    (*pFuncBU8)(uint8);
typedef void    (*pFuncVI16pI16pI16)(int16, int16 *, int16 *);
typedef float   (*pFuncFU8)(uint8);
typedef char   *(*pFuncCFB)(float, bool);
typedef char   *(*pFuncCFBC)(float, bool, char *);
typedef void    (*pFuncVpVIIB)(void *, int, int, bool);
typedef int     (*pFuncIU8I)(uint8 *, int);
*/

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4201 4310)
#else
#pragma anon_unions
#endif


// Объединение размером 16 бит
typedef union
{
    uint16 halfWord;
    uint8  byte[2];
    struct
    {
        uint8 byte0;
        uint8 byte1;
    };
} BitSet16;

// Объединение размером 32 бита
typedef union
{
    uint    word;
    uint16  halfWord[2];
    struct
    {
        uint16 halfWord0;
        uint16 halfWord1;
    };
    uint8   byte[4];
    struct
    {
        uint8 byte0;
        uint8 byte1;
        uint8 byte2;
        uint8 byte3;
    };
} BitSet32;

typedef struct
{
    int16  rel;
    float  abs;
} StructRelAbs;

#define SET_FLAG(flag)      ((flag) = 1)
#define CLEAR_FLAG(flag)    ((flag) = 0)

#define SAFE_FREE(x) if(x) free(x); (x) = 0;

#ifdef WIN32
#pragma warning(pop)
#endif
